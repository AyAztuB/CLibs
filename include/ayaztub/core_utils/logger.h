/**
 * @file logger.h
 * @brief Header file for the logger utility.
 *
 * This file provides logging functionality with various log levels and options.
 * It allows logging messages to a file or standard output and provides
 * customization options for log formatting and filtering.
 *
 * @note define NOLOG before including the header or in compile arguments to
 * remove all LOG() call
 *
 * @code
 * #include <ayaztub/core_utils/logger.h>
 *
 * int main(int argc, char **argv) {
 *     if (!LOGGER_SET_OUTFILE("file.log"))
 *         FATAL("%s", "cannot open file `file.log`");
 *
 *     LOG(INFO, "%s",
 *         "info log printed in file.log with date and thread_id if on linux");
 *
 *     LOGGER_CLOSE_OUTFILE();
 *
 *     LOG(INFO, "%s", "back on stdout/stderr");
 *
 *     logger_set_options((struct logger_options){
 *         .log_level = WARNING,
 *         .show_date = true,
 *     #ifdef __linux__
 *         .show_thread_id = true,
 *     #endif // __linux__
 *     });
 *
 *     LOG(DEBUG, "%s", "debug log not printed due to log level WARNING !");
 *     LOG(WARNING, "%s", "log printed on stdout");
 *     LOG(ERROR, "%s", "error log on stderr");
 *
 *     LOGGER_SET_OUTFILE("stderr");
 *
 *     LOG(DEBUG, "%s", "still not displayed...");
 *     LOG(WARNING, "%s", "all logs on stderr this time !");
 *
 *     LOGGER_CLOSE_OUTFILE();
 *     // this doesn't close the stderr file stream but removed it from the
 *     //   internal to log back on stdout/stderr
 *
 *     return 0;
 * }
 * @endcode
 */

#ifndef __AYAZTUB__CORE_UTILS__LOGGER_H__
#define __AYAZTUB__CORE_UTILS__LOGGER_H__

#include <stdbool.h>
#include <stddef.h>

#ifndef FPRINTF
#    include <stdio.h>
/**
 * @def FPRINTF
 * @brief Macro for formatted output to a file stream.
 *
 * This macro allow you to use your own fprintf function instead of the default
 * one from stdio.h. By default, if undefined, it is an alias of the fprintf
 * standard function.
 */
#    define FPRINTF fprintf
#endif // FPRINTF

#ifndef SNPRINTF
#    include <stdio.h>
/**
 * @def SNPRINTF
 * @brief Macro for formatted output to a buffer with size.
 *
 * This macro allow you to use your own snprintf function instead of the default
 * one from stdio.h. By default, if undefined, it is an alias of the snprintf
 * standard function.
 */
#    define SNPRINTF snprintf
#endif // SNPRINTF

#ifndef SPRINTF
#    include <stdio.h>
/**
 * @def SPRINTF
 * @brief Macro for formatted output to a buffer.
 *
 * This macro allow you to use your own sprintf function instead of the default
 * one from stdio.h. By default, if undefined, it is an alias of the sprintf
 * standard function.
 */
#    define SPRINTF sprintf
#endif // SPRINTF

#ifndef FOPEN
#    include <stdio.h>
/**
 * @def FOPEN
 * @brief Macro to open a file.
 *
 * This macro allow you to use your own fopen function instead of the default
 * one from stdio.h. By default, if undefined, it is an alias of the fopen
 * standard function.
 */
#    define FOPEN fopen
#endif // FOPEN

#ifndef FCLOSE
#    include <stdio.h>
/**
 * @def FCLOSE
 * @brief Macro to close a file.
 *
 * This macro allow you to use your own fclose function instead of the default
 * one from stdio.h. By default, if undefined, it is an alias of the fclose
 * standard function.
 */
#    define FCLOSE fclose
#endif // FCLOSE

#ifndef STDERR
#    include <stdio.h>
/**
 * @def STDERR
 * @brief Macro for the error output stream.
 *
 * This macro allow you to use your own stderr variable instead of the default
 * one from stdio.h. By default, if undefined, it is an alias of the stderr
 * standard error stream.
 */
#    define STDERR stderr
#endif // STDERR

#ifndef STDOUT
#    include <stdio.h>
/**
 * @def STDOUT
 * @brief Macro for the standard output stream.
 *
 * This macro allow you to use your own stdout variable instead of the default
 * one from stdio.h. By default, if undefined, it is an alias of the stdout
 * standard output stream.
 */
#    define STDOUT stdout
#endif // STDOUT

#ifndef MALLOC
#    include <stdlib.h>
/**
 * @def MALLOC
 * @brief Macro to allocate memory.
 *
 * This macro allow you to use your own allocator function instead of the
 * default one from stdlib.h. By default, if undefined, it is an alias of the
 * malloc standard function.
 */
#    define MALLOC malloc
#endif // MALLOC

#ifndef EXIT
#    include <stdlib.h>
/**
 * @def EXIT
 * @brief Macro to exit the program.
 *
 * This macro allow you to use your own exit function instead of the default
 * one from stdlib.h. By default, if undefined, it is an alias of the exit
 * standard function.
 */
#    define EXIT exit
#endif // EXIT

#ifndef EXIT_FAILURE
/**
 * @def EXIT_FAILURE
 * @brief Macro for exit failure code.
 *
 * This macro defines the exit code for program failure. By default, it's 1.
 */
#    define EXIT_FAILURE 1
#endif // EXIT_FAILURE

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
#define __FILENAME__ ((__FILE__) + (SOURCE_PATH_SIZE))

#define LOG_LVL(X)                                                             \
    X(DEBUG)                                                                   \
    X(INFO)                                                                    \
    X(WARNING)                                                                 \
    X(ERROR)                                                                   \
    X(TIMEOUT)                                                                 \
    X(FATAL)

/**
 * @enum log_level
 * @brief Enumeration of log levels.
 *
 * These log levels can be used to control the verbosity of the logger.
 * In order, there are: DEBUG, INFO, WARNING, ERROR, TIMEOUT, FATAL
 */
enum log_level {
#define X(A) A,
    LOG_LVL(X)
#undef X
};

/**
 * @struct logger_options
 * @brief Structure for customizing default logger output.
 *
 * This structure allows customization of the logger's behavior when no log file
 * is specified. It must be passed to logger_set_options to take effect. When a
 * log file is provided, these options are automatically adjusted to print as
 * mush information as possible. To do so, if a log file is provided, the
 * logger_options::log_level will be DEBUG, the logger_options::show_date will
 * be true and the logger_options::show_thread_id will be true (Linux only).
 *
 * @var logger_options::log_level
 * Log level threshold; logs with a level lower than this will not be printed.
 * DEBUG by default.
 * @var logger_options::show_date
 * Flag to show the date in logs. False by default.
 * @var logger_options::show_thread_id
 * Flag to show the thread ID in logs. Only available on Linux; false by
 * default.
 * @note Only available on Linux
 */
struct logger_options {
    enum log_level log_level;
    bool show_date;
#ifdef __linux__
    bool show_thread_id;
#endif // __linux__
};

/**
 * @def LOG
 * @brief Macro to log messages with variadic arguments.
 *
 * This macro logs messages with a specified log level and format string. If a
 * log file is provided, logs are written to the file. Otherwise, logs are
 * written to stderr if the log level is greater than WARNING, and to stdout if
 * the log level is less than or equal to WARNING. If the log level is FATAL,
 * logs are written to both the file (if provided) and stderr. This macro can be
 * disabled defining NOLOG.
 *
 * @warning Disable logs defining the macro NOLOG will result in the suppression
 * of all LOG() macro call in the source code. Thus, the arguments will no
 * longer be computed and this can lead to undefined behavior if arguments can
 * make side effects.
 *
 * @param LEVEL Log level.
 * @param FMT Format string.
 * @param ... Variadic arguments for the format string.
 *
 * Example usage:
 * @code
 * LOG(INFO, "Application started with %d arguments", argc);
 * LOG(ERROR, "Failed to open file: %s", filename);
 * @endcode
 */
#ifdef NOLOG
#    define LOG(LEVEL, FMT, ...) (void)LEVEL
#else // NOLOG
#    define LOG(LEVEL, FMT, ...)                                               \
        do {                                                                   \
            int __n = SNPRINTF(NULL, 0, (FMT), __VA_ARGS__);                   \
            if (__n < 0) {                                                     \
                break;                                                         \
            }                                                                  \
            size_t __size = (size_t)__n + 1;                                   \
            char *__buff = MALLOC(__size * sizeof(*__buff));                   \
            if (__buff == NULL) {                                              \
                break;                                                         \
            }                                                                  \
            SNPRINTF(__buff, __size, (FMT), __VA_ARGS__);                      \
            logger_log((LEVEL), __FILENAME__, __LINE__, __func__, __buff,      \
                       SPRINTF, FPRINTF, STDOUT, STDERR);                      \
        } while (0)
#endif // NOLOG

/**
 * @def FATAL
 * @brief Macro to log fatal messages and crash the program.
 *
 * This macro logs a message with the FATAL log level and then crashes the
 * program. Even if logging is disabled, this macro will cause the program to
 * exit.
 *
 * @param FMT Format string.
 * @param ... Variadic arguments for the format string.
 *
 * Example usage:
 * @code
 * FATAL("Critical error: %s", error_message);
 * @endcode
 */
#define FATAL(FMT, ...)                                                        \
    do {                                                                       \
        LOG(FATAL, (FMT), __VA_ARGS__);                                        \
        EXIT(EXIT_FAILURE);                                                    \
    } while (0)

/**
 * @brief Set options to override the default logger settings.
 *
 * This function allows the user to customize the logger's behavior by setting
 * options defined in the logger_options structure.
 *
 * @param options Structure containing the logger options.
 *
 * Example usage:
 * @code
 * logger_set_options((struct logger_options) {
 *   .log_level = ERROR,
 *   .show_date = true,
 * #ifdef __linux__
 *   .show_thread_id = true,
 * #endif // __linux__
 * });
 * @endcode
 */
void logger_set_options(struct logger_options options);

/**
 * @def LOGGER_SET_OUTFILE(filename)
 * @brief Macro to set the log output file.
 *
 * This macro sets the log output file using the specified filename.
 * It is a wrapper around logger_set_outfile().
 *
 * @param filename The name of the log output file.
 * @return true if the file was successfully set, false otherwise.
 *
 * Example usage:
 * @code
 * if (!LOGGER_SET_OUTFILE("logfile.txt")) {
 *     fprintf(stderr, "Failed to set log file\n");
 * }
 * @endcode
 */
#define LOGGER_SET_OUTFILE(filename) logger_set_outfile(filename, FOPEN, STDERR)

/**
 * @brief Sets the log output file.
 *
 * This function sets the log output file using the specified filename and file
 * opening function. It should not be used directly; use LOGGER_SET_OUTFILE()
 * instead.
 *
 * @param filename The name of the log output file.
 * @param open_file Function pointer to open a file.
 * @param stderr_file Standard error file stream.
 * @return true if the file was successfully set, false otherwise.
 */
bool logger_set_outfile(const char *filename,
                        FILE *(*open_file)(const char *, const char *),
                        FILE *stderr_file);

/**
 * @def LOGGER_CLOSE_OUTFILE()
 * @brief Macro to close the log output file.
 *
 * This macro closes the log file that was previously set using
 * LOGGER_SET_OUTFILE(). This macro closes the log output file. It is a wrapper
 * around logger_close_outfile().
 *
 * Example usage:
 * @code
 * LOGGER_CLOSE_OUTFILE();
 * @endcode
 */
#define LOGGER_CLOSE_OUTFILE() logger_close_outfile(FCLOSE, STDERR)

/**
 * @brief Closes the log output file.
 *
 * This function closes the log output file using the specified file closing
 * function. It should not be used directly; use LOGGER_CLOSE_OUTFILE() instead.
 *
 * @param close_file Function pointer to close a file.
 * @param stderr_file Standard error file stream.
 */
void logger_close_outfile(int (*close_file)(FILE *), FILE *stderr_file);

/**
 * @brief Logs a message with the specified log level.
 *
 * This function logs a message with the specified log level, file name, line
 * number, function name, and message. It should not be used directly; use the
 * LOG() macro instead.
 *
 * @param level The log level.
 * @param file_name The name of the source file where the log is generated.
 * @param line The line number where the log is generated.
 * @param func_name The name of the function where the log is generated.
 * @param message The log message.
 * @param _sprintf Function pointer for sprintf-like functionality.
 * @param _fprintf Function pointer for fprintf-like functionality.
 * @param _stdout Standard output file stream.
 * @param _stderr Standard error file stream.
 */
void logger_log(enum log_level level, const char *file_name, size_t line,
                const char *func_name, char *message,
                int (*_sprintf)(char *, const char *, ...),
                int (*_fprintf)(FILE *, const char *, ...), FILE *_stdout,
                FILE *_stderr);

#endif // __AYAZTUB__CORE_UTILS__LOGGER_H__
