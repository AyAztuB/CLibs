/**
 * @file logger.h
 * @brief Thread-safe logger library for debugging purposes in C99.
 *
 * This library provides logging functionality with various log levels, support
 * for callbacks, and options to log to files or standard output. It is designed
 * for debugging and includes features like backtrace logging on fatal errors.
 *
 * @note define NOLOG to remove all LOG() calls
 *
 * @warning As you can remove all LOG() calls using NOLOG, you MUST NEVER call a
 * function in the LOG() macro argument as this function call will also be
 * removed.
 *
 * @note define NOLOG will not remove backtrace logs on fatal signals (SIGSEGV,
 * SIGILL, SIGABRT, ...)
 *
 * @warning This logger is optimized for debugging and may not be suitable for
 * high-performance logging in production. For release builds, use restrictive
 * log levels or disable logging using NOLOG.
 *
 * @warning This library is thread-safe but the logger callback registered by
 * the user is called IN the critical section (to ensure logs order consitancy
 * between with the logger core). To avoid deadlocks, the user MUST NEVER do
 * recursive logging and thus MUST NEVER call any logger core functions/macros
 * in the callback provided.
 *
 * @code
 * // usage example
 * #include <ayaztub/logger.h>
 *
 * int main(int argc, char **argv) {
 * #ifndef __GNUC__
 *     logger_init();
 * #endif // __GNUC__
 *
 *     logger_set_format_options(true, true, true);
 *     logger_set_log_level(LOG_DEBUG);
 *
 *     if (!logger_set_log_file("logfile.txt")) {
 *         // error on logger setup...
 *         fprintf(stderr, "Failed to setup logger:"
 *             " open logfile.txt failed !\n");
 *         exit(1);
 *     }
 *
 *     // Log messages of various levels
 *     LOG(LOG_INFO, "This is and informational message.");
 *     LOG(LOG_WARN, "This is a warning message.");
 *     LOG(LOG_ERROR, "This is an error message.");
 *     LOG(LOG_DEBUG, "Debugging details: x = %d, y = %d", 69, 96);
 *
 *     // Demonstarte callback usage (log to stdout)
 *     logger_set_callback(log_on_stdout);
 *     LOG(LOG_TRACE, "Trace message with callback active"
 *         " (logged on both stdout and logfile.txt).");
 *
 *     // cleanup: logger_close_file() autonatically called by
 *     // logger_deinit() destructor
 *
 * #ifndef __GNUC__
 *     logger_deinit();
 * #endif // __GNUC__
 *
 *     return 0;
 * }
 * @endcode
 *
 * @note On fatal error (or signal SIGSEGV, SIGABRT, SIGILL, SIGFPE, SIGBUS),
 * the backtrace may be logged as FATAL.
 * For example, the backtrace log `./a.out(+0x21ae) [0x5620024571ae]`
 * can be retrieved using addr2line:
 * `addr2line -Cfspe ./a.out +0x21ae` gives us the function, file and line
 * (with debug flags -g)
 *
 * @todo Integrate the addr2line tool in the log_backtrace() function, without
 * any allocation (fatal error may be due to corrupted memory...).
 */

#ifndef __AYAZTUB__LOGGER_H__
#define __AYAZTUB__LOGGER_H__

#include <ayaztub/utils/util_attributes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @def SOURCE_PATH_SIZE
 * @brief Macro to truncate the root file path.
 *
 * This macro is useful when the project is compiled with CMake, allowing for a
 * cleaner file path in logs by removing a fixed number of characters from the
 * start of the @c __FILE__ macro. The default value if the macro wasn't defined
 * earlier is 0.
 */
#ifndef SOURCE_PATH_SIZE
#    define SOURCE_PATH_SIZE 0
#endif // SOURCE_PATH_SIZE

/**
 * @brief Macro to adjust the file path based on SOURCE_PATH_SIZE.
 *
 * This macro modifies the @c __FILE__ macro to remove a specified number of
 * characters from the beginning, making the logged file paths more readable.
 */
#ifndef __FILENAME__
#define __FILENAME__ ((__FILE__) + (SOURCE_PATH_SIZE))
#endif // __FILENAME__

/**
 * @enum log_level
 * @brief Log levels supported by the logger.
 */
enum log_level {
    LOG_QUITE, /**< No output */
    LOG_FATAL, /**< Fatal errors (program will terminate) */
    LOG_ERROR, /**< Errors */
    LOG_TIMEOUT, /**< Timeout warnings */
    LOG_WARN, /**< Warnings */
    LOG_INFO, /**< Informational messages */
    LOG_TRACE, /**< Trace-level debugging */
    LOG_DEBUG, /**< Debug-level messages */
    LOG_FULL, /**< All messages */
};

/**
 * @typedef logger_cb_t
 * @brief Logger callback type.
 *
 * This function is called whenever a log message is generated.
 * It must not call logger functions/macros directly to avoid recursion.
 *
 * @param lvl Log level of the message.
 * @param colored_message Log message with formatting (for TTY outputs like
 * stdout).
 * @param raw_message Log message without formatting (for file outputs).
 */
typedef void (*logger_cb_t)(enum log_level lvl,
                            const char *const colored_message,
                            const char *const raw_message);

/**
 * @brief Initializes the logger.
 *
 * This function is called automatically at program startup (constructor
 * attribute). It must not be called directly.
 *
 * @warning CONSTRUCTOR is not defined without __GNUC__. If working with a non
 * GNUC complient compiler, please call it directly at main start.
 * @code
 * int main(void) {
 * #ifndef __GNUC__
 *     logger_init();
 * #endif // __GNUC__
 *
 *     // program code
 *
 * #ifndef __GNUC__
 *     logger_deinit();
 * #endif // __GNUC__
 *
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 */
CONSTRUCTOR void logger_init(void);

/**
 * @brief Deinitializes the logger.
 *
 * This function is called automatically at program termination (destructor
 * attribute). It must not be called directly.
 *
 * @warning If __GNUC__ is not defined, please refer to the example in
 * logger_init().
 */
DESTRUCTOR void logger_deinit(void);

/**
 * @brief Configures log formatting options.
 *
 * @param show_date Whether to include the current date in log messages.
 * @param show_thread Whether to include thread identifiers in log messages.
 * @param log_trace_on_fatal Whether to log a backtrace on fatal errors or
 * signals.
 *
 * @note All options are true by default.
 */
void logger_set_format_options(bool show_date, bool show_thread,
                               bool log_trace_on_fatal);

/**
 * @brief Sets the current log level.
 *
 * Messages below the specified level will not be logged.
 *
 * @param level The desired log level.
 */
void logger_set_log_level(enum log_level level);

/**
 * @brief Sets the log level based on a string.
 *
 * @param log_level String representation of the log level (e.g., "INFO",
 * "DEBUG", "LOG_INFO", "LOG_DEBUG").
 */
void logger_set_log_level_from_string(const char *const log_level) NONNULL
    NULL_TERMINATED_STRING_ARG(1);

/**
 * @brief Sets the log level based on an environment variable.
 *
 * The logger will look for the environment variable `LOG_LEVEL` and configure
 * the level accordingly. The environment variable `LOG_LEVEL` value must
 * follow the same format as the string parameter accepted by
 * logger_set_log_level_from_string().
 */
void logger_set_log_level_from_env(void);

/**
 * @brief Sets the output file for log messages.
 *
 * @param filename Path to the log file.
 * @return `true` if the file was successfully opened, `false` otherwise.
 */
bool logger_set_log_file(const char *const filename) NONNULL
    NULL_TERMINATED_STRING_ARG(1);

/**
 * @brief Sets the output file for log messages based on an environment
 * variable.
 *
 * The logger will look for the environment variable `LOG_FILE` and configure
 * the output file accordingly.
 *
 * @param default_filename Path to the log path if the environment variable was
 * not found. (can be NULL)
 * @return `true` if environment variable `LOG_FILE` exists and was successfully
 * opened OR if default_filename is not NULL and was successfully opened,
 * `false` otherwise (1- LOG_FILE env variable exists but file cannot be opened,
 * 2- LOG_FILE env variable does not exists and default_filename is NULL, 3-
 * LOG_FILE env variable does not exists and default_filename cannot be opened)
 */
bool logger_set_log_file_from_env(const char *const default_filename);

/**
 * @brief Sets the file descriptor for log output.
 *
 * @param file A valid file pointer for output.
 * @return `true` if the file descriptor is valid, `false` otherwise.
 *
 * @warning The file descriptor must not be closed by the user after this
 * function call. If you want to close it, please refer to the
 * logger_close_file() function.
 */
bool logger_set_log_fileno(FILE *file) NONNULL FD_ARG_WRITE(1);

/**
 * @brief Closes the current log file, if any.
 *
 * @note Automatically called by logger_deinit().
 */
void logger_close_file(void);

/**
 * @brief Sets a callback function to handle log messages.
 *
 * @param callback Pointer to the callback function.
 */
void logger_set_callback(logger_cb_t callback);

/**
 * @brief Logs a message with a specified log level.
 *
 * @param level Log level for the message.
 * @param file Source file name (__FILE__).
 * @param line Source line number (__LINE__).
 * @param func Source function name (__func__).
 * @param fmt Format string for the message.
 * @param ... Additional arguments for the format string.
 *
 * @note Please use the user friendly LOG() macro insteed.
 */
FORMAT(printf, 5, 6)
void log_message(enum log_level level, const char *const file, size_t line,
                 const char *const func, const char *const fmt, ...) NONNULL
    NULL_TERMINATED_STRING_ARG(2) NULL_TERMINATED_STRING_ARG(4);

/**
 * @brief Logs a message using the default log macro.
 *
 * Usage:
 * @code
 * LOG(LOG_INFO, "This is an info message with value: %d", value);
 * @endcode
 *
 * @param lvl Log level.
 * @param ... Format string and arguments.
 */
#ifdef NOLOG
#    define LOG(lvl, ...) (void)0
#else // NOLOG
#    define LOG(lvl, ...)                                                      \
        log_message((lvl), __FILENAME__, __LINE__, __func__, __VA_ARGS__)
#endif // NOLOG

/**
 * @brief Logs a fatal message and exits the program.
 *
 * This macro is intended to exit the program even if logging is disabled
 * (with NOLOG defined).
 *
 * @param ... Format string and arguments.
 */
#define LOG_FATAL(...)                                                         \
    do {                                                                       \
        LOG(LOG_FATAL, __VA_ARGS__);                                           \
        exit(1);                                                               \
    } while (0)

// ---------- logger callback to log on stdout/stderr ---------- //

/**
 * @brief Logs a message to stdout using the logger callback.
 */
void log_on_stdout(enum log_level, const char *const, const char *const) NONNULL
    NULL_TERMINATED_STRING_ARG(2) NULL_TERMINATED_STRING_ARG(3);

/**
 * @brief Logs a message to stderr using the logger callback.
 */
void log_on_stderr(enum log_level, const char *const, const char *const) NONNULL
    NULL_TERMINATED_STRING_ARG(2) NULL_TERMINATED_STRING_ARG(3);

#endif // __AYAZTUB__LOGGER_H__
