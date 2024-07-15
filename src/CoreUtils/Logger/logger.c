#ifdef __linux__
#    define _GNU_SOURCE
#endif // __linux__

#include <ayaztub/core_utils/logger.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define TURQUOISE "\033[0;36m"
#define WHITE "\033[0m"
#define ORANGE "\033[0;38:2:220:165:0m"
#define GRAY "\033[0;2m"

static const char *log_level_str[] = {
#define X(A) [A] = #A,
    LOG_LVL(X)
#undef X
};

// Default logger: <= WARNING = stdout, > WARNING = stderr
// if log file provided: all logs printed (even if log level > DEBUG)
// if log file provided: all logs in this file + FATAL level still on stderr...

static FILE *outfile = NULL;
static struct logger_options logger_options = {
    .log_level = DEBUG,
    .show_date = false,
#ifdef __linux__
    .show_thread_id = false,
#endif // __linux__
};

void logger_set_options(struct logger_options options) {
    logger_options = options;
}

bool logger_set_outfile(const char *filename,
                        FILE *(*open_file)(const char *, const char *),
                        FILE *stderr_file) {
    if (outfile != NULL)
        return false;
    if (!strcmp(filename, "stderr")) {
        outfile = stderr_file;
        return true;
    }
    FILE *f = open_file(filename, "w");
    if (!f)
        return false;
    outfile = f;
    return true;
}

void logger_close_outfile(int (*close_file)(FILE *), FILE *stderr_file) {
    if (outfile && outfile != stderr_file)
        close_file(outfile);
    outfile = NULL;
}

void logger_log(enum log_level level, const char *file_name, size_t line,
                const char *func_name, char *message,
                int (*_sprintf)(char *, const char *, ...),
                int (*_fprintf)(FILE *, const char *, ...), FILE *_stdout,
                FILE *_stderr) {
#ifdef __linux__
    pid_t id = gettid();
#endif // __linux__
    time_t t = time(NULL);
    struct tm *tt = localtime(&t);
    char date[100] = { 0 };
    _sprintf(date, "[%d-%d-%d %d:%d:%d]", tt->tm_year + 1900, tt->tm_mon + 1,
             tt->tm_mday, tt->tm_hour, tt->tm_min, tt->tm_sec);
    if (outfile && outfile != _stderr) {
        _fprintf(outfile, "[%s] %s ", log_level_str[level], date);
#ifdef __linux__
        if (id == getpid())
            _fprintf(outfile, "[main thread] ");
        else
            _fprintf(outfile, "[thread: %d] ", id);
#endif // __linux__
        _fprintf(outfile, "%s:%zu in %s(): %s\n", file_name, line, func_name,
                 message);
    }
    if ((level >= logger_options.log_level && (!outfile || outfile == _stderr))
        || level == FATAL) {
        const char *color;
        FILE *out;
        switch (level) {
            case DEBUG:
                color = GRAY;
                out = _stdout;
                break;
            case INFO:
                color = TURQUOISE;
                out = _stdout;
                break;
            case WARNING:
                color = YELLOW;
                out = _stdout;
                break;
            case ERROR:
                color = ORANGE;
                out = _stderr;
                break;
            case TIMEOUT:
                color = BLUE;
                out = _stderr;
                break;
            case FATAL:
                color = RED;
                out = _stderr;
                break;
        }
        if (outfile)
            out = outfile;
        _fprintf(out, "%s[%s]%s ", color, log_level_str[level], WHITE);
        if (logger_options.show_date)
            _fprintf(out, "%s ", date);
#ifdef __linux__
        if (logger_options.show_thread_id) {
            if (id == getpid())
                _fprintf(out, "[main thread] ");
            else
                _fprintf(out, "[thread: %d] ", id);
        }
#endif // __linux__
        _fprintf(out, "%s:%zu in %s(): %s%s%s\n", file_name, line, func_name,
                 color, message, WHITE);
    }
}
