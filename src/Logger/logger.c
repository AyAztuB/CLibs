#ifdef __linux__
#    define _GNU_SOURCE
#endif // __linux__

#include <ayaztub/logger.h>

#include <errno.h>
#include <execinfo.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*
 * Color identifiers:
 * - \033 (octal) or \e (bash) or \x1b (hex):
 *     escape character
 * - format: [X;Ym: X is the format (0: default)
 *                  Y is the color (multiple ones separeted by ';' can appear
 *                      for multiple modes (foraground / background))
 * - X values: 1 = bold, 2 = darker, 3 = italic, 4 = underline, 5/6 = blinking
 *             7 = reverse, 8 = invisible, 9 = cross-out
 * - Y values: 30 -> 37 = default foreground, 40 -> 47 = default background, ...
 *             38 (and 48) have special meaning for foreground (respectively
 *                 background). It allow to specify a raw color.
 *             -> this color is then: 38:MODE:COLOR.
 *                with MODE = 2, the COLOR is specified in RGB mode.
 *                In RGB mode, COLOR = R:G:B with R,G,B values in [0, 255] range
 *             color 0;38:2:255:0:127 is a color with 255 RED, 0 GREEN, 127 BLUE
 *                 and thus refer to PINK.
 */

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define ORANGE "\033[0;38:2:220:165:0m"
#define PINK "\033[0;38:2:255:0:127m"
#define GRAY "\033[0;2m"

#define RESET "\033[0m"

#define BUFFER_SIZE 2048

// ---------- Static Variables ---------- //
static FILE *log_file = NULL;
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
static logger_cb_t log_callback = NULL;
static enum log_level current_log_level = LOG_INFO;
static bool show_date = true;
static bool show_thread = true;
static bool log_trace_on_fatal = true;

// ---------- Utility Functions ---------- //
static const char *log_level_to_string(enum log_level level) {
    switch (level) {
        case LOG_FATAL:
            return "FATAL";
        case LOG_ERROR:
            return "ERROR";
        case LOG_TIMEOUT:
            return "TIMEOUT";
        case LOG_WARN:
            return "WARN";
        case LOG_INFO:
            return "INFO";
        case LOG_TRACE:
            return "TRACE";
        case LOG_DEBUG:
            return "DEBUG";
        default:
            return "UNKNOWN";
    }
}

static const char *log_level_to_color(enum log_level level) {
    switch (level) {
        case LOG_FATAL:
            return RED;
        case LOG_ERROR:
            return ORANGE;
        case LOG_TIMEOUT:
            return MAGENTA;
        case LOG_WARN:
            return YELLOW;
        case LOG_INFO:
            return CYAN;
        case LOG_TRACE:
            return PINK;
        case LOG_DEBUG:
            return GRAY;
        default:
            return WHITE;
    }
}

static void format_log_message(char *colored_buffer, char *raw_buffer,
                               size_t buffer_size, enum log_level level,
                               const char *const file, size_t line,
                               const char *const func, const char *const fmt,
                               va_list args) {
    char date_buffer[64] = "";
    if (show_date) {
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        strftime(date_buffer, sizeof(date_buffer) / sizeof(date_buffer[0]),
                 "%Y-%m-%d %H:%M:%S ", tm_info);
    }

    char thread_buffer[32] = "";
    if (show_thread) {
        pid_t tid = gettid();
        if (tid == getpid()) {
            snprintf(thread_buffer,
                     sizeof(thread_buffer) / sizeof(thread_buffer[0]),
                     "[main thread] ");
        } else {
            snprintf(thread_buffer,
                     sizeof(thread_buffer) / sizeof(thread_buffer[0]),
                     "[thread: %lu] ", (unsigned long)tid);
        }
    }

    char message[1024];
    size_t message_size = sizeof(message) / sizeof(message[0]);
    size_t should_write = vsnprintf(message, message_size, fmt, args);
    if (should_write >= message_size) {
        // message truncated: write '...' at the end [size-4, size-2]
        for (size_t i = message_size - 4; i < message_size - 1; i++) {
            message[i] = '.';
        }
    }

    snprintf(colored_buffer, buffer_size,
             "%s%s[%s]" RESET " [%s:%zu:%s()] %s%s%s" RESET, date_buffer,
             log_level_to_color(level), log_level_to_string(level), file, line,
             func, thread_buffer, log_level_to_color(level), message);
    snprintf(raw_buffer, buffer_size, "%s[%s] [%s:%zu:%s()] %s%s", date_buffer,
             log_level_to_string(level), file, line, func, thread_buffer,
             message);
}

static void log_backtrace(const char *const init_msg) {
    pthread_mutex_lock(&log_mutex);

    if (init_msg) {
        static char _init_msg[1024];
        size_t idx = 0;

        if (show_date) {
            time_t t = time(NULL);
            struct tm *tm_info = localtime(&t);
            strftime(_init_msg, 1024, "%Y-%m-%d %H:%M:%S ", tm_info);
            idx = strlen(_init_msg);
        }

        if (log_file) {
            fprintf(log_file, "%s[FATAL] %s\n", _init_msg, init_msg);
            fflush(log_file);
        }

        if (log_callback) {
            static char _init_raw[1024];
            strcpy(_init_raw, _init_msg);
            snprintf(_init_raw + idx, 1024 - idx, "[FATAL] %s", init_msg);
            snprintf(_init_msg + idx, 1024 - idx, "%s[FATAL]" RESET " %s",
                     log_level_to_color(LOG_FATAL), init_msg);
            log_callback(LOG_FATAL, _init_msg, _init_raw);
        }
    }

    static void *buffer[128];
    int nptrs = backtrace(buffer, sizeof(buffer) / sizeof(void *));
    char **symbols = backtrace_symbols(buffer, nptrs);
    static char one[512];

    for (int i = 1; i < nptrs; i++) {
        if (symbols) {
            if (log_file) {
                fprintf(log_file, "  %s\n", symbols[i]);
                fflush(log_file);
            }

            if (log_callback) {
                snprintf(one, 512, "  %s", symbols[i]);
                log_callback(LOG_FATAL, one, one);
            }
        }
    }

    free(symbols);

    pthread_mutex_unlock(&log_mutex);
}

static void logger_signal_handler(int signo) {
    if (log_trace_on_fatal) {
        static char init_msg[256];
        snprintf(init_msg, 256, "Caught signal %d (%s). Backtrace:", signo,
                strsignal(signo));
        log_backtrace(init_msg);
    }

    // Re-raise the signal to terminate the program with the defaut behavior
    signal(signo, SIG_DFL);
    raise(signo);
}

// ---------- Logger Functions ----------
CONSTRUCTOR void logger_init(void) {
    struct sigaction sa;
    sa.sa_handler = logger_signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    // Set signal handlers for common fatal signals
    sigaction(SIGSEGV, &sa, NULL); // Segmentation fault
    sigaction(SIGILL, &sa, NULL); // Illegal instruction
    sigaction(SIGABRT, &sa, NULL); // Aborted
    sigaction(SIGFPE, &sa, NULL); // Floating-point exception
    sigaction(SIGBUS, &sa, NULL); // Bus error
}

DESTRUCTOR void logger_deinit(void) {
    logger_close_file();
}

void logger_set_format_options(bool show_date_opt, bool show_thread_opt,
                               bool log_trace_on_fatal_opt) {
    pthread_mutex_lock(&log_mutex);
    show_date = show_date_opt;
    show_thread = show_thread_opt;
    log_trace_on_fatal = log_trace_on_fatal_opt;
    pthread_mutex_unlock(&log_mutex);
}

void logger_set_log_level(enum log_level level) {
    pthread_mutex_lock(&log_mutex);
    current_log_level = level;
    pthread_mutex_unlock(&log_mutex);
}

void logger_set_log_level_from_string(const char *const log_level) {
    const char *lvl_str = log_level;
    if (strncmp(log_level, "LOG_", 4) == 0)
        lvl_str = log_level + 4;

    pthread_mutex_lock(&log_mutex);
    if (strcmp(lvl_str, "FULL") == 0)
        current_log_level = LOG_FULL;
    else if (strcmp(lvl_str, "DEBUG") == 0)
        current_log_level = LOG_DEBUG;
    else if (strcmp(lvl_str, "TRACE") == 0)
        current_log_level = LOG_TRACE;
    else if (strcmp(lvl_str, "INFO") == 0)
        current_log_level = LOG_INFO;
    else if (strcmp(lvl_str, "WARN") == 0)
        current_log_level = LOG_WARN;
    else if (strcmp(lvl_str, "ERROR") == 0)
        current_log_level = LOG_ERROR;
    else if (strcmp(lvl_str, "FATAL") == 0)
        current_log_level = LOG_FATAL;
    else if (strcmp(lvl_str, "QUIET") == 0)
        current_log_level = LOG_QUITE;
    pthread_mutex_unlock(&log_mutex);
}

void logger_set_log_level_from_env(void) {
    const char *env_level = getenv("LOG_LEVEL");
    if (env_level) {
        logger_set_log_level_from_string(env_level);
    }
}

bool logger_set_log_file(const char *const filename) {
    FILE *file = fopen(filename, "a");
    if (!file)
        return false;

    logger_close_file();
    pthread_mutex_lock(&log_mutex);
    log_file = file;
    pthread_mutex_unlock(&log_mutex);
    return true;
}

bool logger_set_log_file_from_env(const char *const default_filename) {
    const char *env_file = getenv("LOG_FILE");
    if (env_file) {
        return logger_set_log_file(env_file);
    }

    if (default_filename) {
        return logger_set_log_file(default_filename);
    }

    return false;
}

bool logger_set_log_fileno(FILE *file) {
    logger_close_file();
    pthread_mutex_lock(&log_mutex);
    log_file = file;
    pthread_mutex_unlock(&log_mutex);
    return true;
}

void logger_close_file(void) {
    if (log_file) {
        pthread_mutex_lock(&log_mutex);
        fclose(log_file);
        log_file = NULL;
        pthread_mutex_unlock(&log_mutex);
    }
}

void logger_set_callback(logger_cb_t callback) {
    pthread_mutex_lock(&log_mutex);
    log_callback = callback;
    pthread_mutex_unlock(&log_mutex);
}

void log_message(enum log_level level, const char *const file, size_t line,
                 const char *const func, const char *const fmt, ...) {
    // LOG_FULL and LOG_QUITE are not valid log level messages and are used
    // for convenience to accept either all logs or no ones.
    if (level == LOG_FULL || level == LOG_QUITE)
        return;
    if (level > current_log_level)
        return;

    char colored_msg[BUFFER_SIZE];
    char raw_msg[BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    format_log_message(colored_msg, raw_msg, BUFFER_SIZE, level, file, line,
                       func, fmt, args);
    va_end(args);

    pthread_mutex_lock(&log_mutex);

    if (log_callback) {
        log_callback(level, colored_msg, raw_msg);
    }

    if (log_file) {
        fprintf(log_file, "%s\n", raw_msg);
        fflush(log_file);
    }

    pthread_mutex_unlock(&log_mutex);

    if (level == LOG_FATAL) {
        if (log_trace_on_fatal) {
            log_backtrace(NULL);
        }
        exit(EXIT_FAILURE);
    }
}

// ---------- Logger Callbacks ---------- //
void log_on_stdout(UNUSED enum log_level lvl, const char *const colored_message,
                   UNUSED const char *const raw_message) {
    printf("%s\n", colored_message);
    fflush(stdout);
}

void log_on_stderr(UNUSED enum log_level lvl, const char *const colored_message,
                   UNUSED const char *const raw_message) {
    fprintf(stderr, "%s\n", colored_message);
    fflush(stderr);
}

// ---------- Logger Tests ---------- //

enum log_level logger_get_log_level(void) {
    return current_log_level;
}
