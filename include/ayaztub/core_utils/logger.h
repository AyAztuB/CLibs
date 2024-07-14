/**
 * @file logger.h
 * @brief Header file for the logger utility.
 *
 * This file provides logging functionality with various log levels and options. 
 * It allows logging messages to a file or standard output and provides customization 
 * options for log formatting and filtering.
 * @note define NOLOG before including the header or in compile arguments to remove all LOG() call
 *
 * @code
 * #include <ayaztub/core_utils/logger.h>
 * int main(int argc, char **argv) {
 *     if (!logger_set_outfile("file.log"))
 *         FATAL("%s", "cannot open file `file.log`");
 *
 *     LOG(INFO, "%s", "info log printed in file.log with date and thread_id if on linux");
 *
 *     logger_close_outfile();
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
 *     LOG(DEBUG, "%s", "debug log not printed due to log level WARNING in options");
 *     LOG(WARNING, "%s", "log printed on stdout");
 *     LOG(ERROR, "%s", "error log on stderr");
 *
 *     logger_set_outfile("stderr");
 *
 *     LOG(DEBUG, "%s", "still not displayed...");
 *     LOG(WARNING, "%s", "all logs on stderr this time !");
 *
 *     logger_close_outfile();
 *     // this doesn't close the stderr file stream but removed it from the internal to log back on stdout/stderr
 *
 *     return 0;
 * }
 * @endcode
 */

#ifndef __AYAZTUB__CORE_UTILS__LOGGER_H__
#define __AYAZTUB__CORE_UTILS__LOGGER_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @def SOURCE_PATH_SIZE
 * @brief Macro to truncate the root file path.
 *
 * This macro is useful when the project is compiled with CMake, allowing for a cleaner
 * file path in logs by removing a fixed number of characters from the start of the __FILE__ macro.
 * The default value if the macro wasn't defined earlier is 0.
 */
#ifndef SOURCE_PATH_SIZE
    #define SOURCE_PATH_SIZE 0
#endif // SOURCE_PATH_SIZE

/**
 * @brief Macro to adjust the file path based on SOURCE_PATH_SIZE.
 *
 * This macro modifies the __FILE__ macro to remove a specified number of characters from the beginning,
 * making the logged file paths more readable.
 */
#define __FILENAME__ ((__FILE__) + (SOURCE_PATH_SIZE))

#define LOG_LVL(X) \
    X(DEBUG) \
    X(INFO) \
    X(WARNING) \
    X(ERROR) \
    X(TIMEOUT) \
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
 * This structure allows customization of the logger's behavior when no log file is specified.
 * It must be passed to logger_set_options to take effect.
 * When a log file is provided, these options are automatically adjusted to print as mush information as possible.
 * To do so, if a log file is provided, the logger_options::log_level will be DEBUG,
 * the logger_options::show_date will be true and the logger_options::show_thread_id will be true (Linux only).
 *
 * @var logger_options::log_level
 * Log level threshold; logs with a level lower than this will not be printed. DEBUG by default.
 * @var logger_options::show_date
 * Flag to show the date in logs. False by default.
 * @var logger_options::show_thread_id
 * Flag to show the thread ID in logs. Only available on Linux; false by default.
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
 * This macro logs messages with a specified log level and format string. If a log file is provided,
 * logs are written to the file. Otherwise, logs are written to stderr if the log level is greater
 * than WARNING, and to stdout if the log level is less than or equal to WARNING.
 * If the log level is FATAL, logs are written to both the file (if provided) and stderr.
 * This macro can be disabled defining NOLOG.
 *
 * @warning Disable logs defining the macro NOLOG will result in the suppression of all LOG macro call in the source code.
 * Thus, the arguments will no longer be computed and this can lead to undefined behavior if arguments can make side effects.
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
    #define LOG(LEVEL, FMT, ...) (void)LEVEL
#else // NOLOG
    #define LOG(LEVEL, FMT, ...) \
        do { \
            int __n = snprintf(NULL, 0, (FMT), __VA_ARGS__); \
            if (__n < 0) { \
                break; \
            } \
            size_t __size = (size_t) __n + 1; \
            char* __buff = malloc(__size * sizeof(*__buff)); \
            if (__buff == NULL) { \
                break; \
            } \
            snprintf(__buff, __size, (FMT), __VA_ARGS__); \
            logger_log((LEVEL), __FILENAME__, __LINE__, __func__, __buff); \
        } while(0)
#endif // NOLOG

/**
 * @def FATAL
 * @brief Macro to log fatal messages and crash the program.
 *
 * This macro logs a message with the FATAL log level and then crashes the program.
 * Even if logging is disabled, this macro will cause the program to exit.
 *
 * @param FMT Format string.
 * @param ... Variadic arguments for the format string.
 *
 * Example usage:
 * @code
 * FATAL("Critical error: %s", error_message);
 * @endcode
 */
#define FATAL(FMT, ...) \
    do { \
        LOG(FATAL, (FMT), __VA_ARGS__); \
        exit(1); \
    } while(0)

/**
 * @brief Set options to override the default logger settings.
 *
 * This function allows the user to customize the logger's behavior by setting options
 * defined in the logger_options structure.
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
 * @brief Provide a log file to the logger.
 *
 * This function sets the log file where all logs will be written. If the filename "stderr" is provided,
 * all logs will be printed to stderr instead of stdout/stderr.
 *
 * @param filename Name of the file to log into.
 * @return true on success, false on failure.
 *
 * Example usage:
 * @code
 * if (!logger_set_outfile("logfile.txt")) {
 *     fprintf(stderr, "Failed to set log file\n");
 * }
 * @endcode
 */
bool logger_set_outfile(const char* filename);

/**
 * @brief Close the currently set log file.
 *
 * This function closes the log file that was previously set using logger_set_outfile.
 *
 * Example usage:
 * @code
 * logger_close_outfile();
 * @endcode
 */
void logger_close_outfile(void);

/**
 * @brief Log a message.
 *
 * This function logs a message with a specified log level, file name, line number, and function name.
 * It should never be used directly; use the LOG macro instead.
 *
 * @param level Log level.
 * @param file_name Name of the file where the log is coming from.
 * @param line Line number where the log is coming from.
 * @param func_name Function name where the log is coming from.
 * @param message Log message.
 */
void logger_log(enum log_level level, const char* file_name, size_t line, const char* func_name, char* message);

#endif // __AYAZTUB__CORE_UTILS__LOGGER_H__
