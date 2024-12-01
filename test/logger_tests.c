#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <ayaztub/core_utils/logger.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Declare tests functions
enum log_level logger_get_log_level(void);

// Helper function to check file contents
static int file_contains(const char *filename, const char *expected) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return 0;
    }

    char buffer[1024];
    int found = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (strstr(buffer, expected)) {
            found = 1;
            break;
        }
    }

    fclose(file);
    return found;
}

// Test log to file
Test(logger, log_to_file) {
    const char *test_file = "test_log_to_file.log";
    remove(test_file);

    cr_assert(logger_set_log_file(test_file), "Failed to set log file.");

    LOG(LOG_INFO, "Test logging to file");

    cr_assert(file_contains(test_file, "Test logging to file"), "Log file does not contain the expected message.");

    logger_close_file();
    remove(test_file);
}

// Test log level filtering
Test(logger, log_level_filtering) {
    const char *test_file = "test_log_level_filtering.log";
    remove(test_file);

    cr_assert(logger_set_log_file(test_file), "Failed to set log file.");
    logger_set_log_level(LOG_WARN);

    LOG(LOG_DEBUG, "Debug message (should not appear)");
    LOG(LOG_INFO, "Info message (should not appear)");
    LOG(LOG_WARN, "Warning message (should appear)");

    cr_assert_not(file_contains(test_file, "Debug message"), "Debug message should not be logged.");
    cr_assert_not(file_contains(test_file, "Info message"), "Info message should not be logged.");
    cr_assert(file_contains(test_file, "Warning message"), "Warning message was not logged.");

    logger_close_file();
    remove(test_file);
}

// Test callback logging
Test(logger, callback_logging) {
    static int callback_called = 0;

    void callback(enum log_level lvl, const char *colored, const char *raw) {
        (void)lvl;
        cr_assert(strstr(raw, "Callback test message"), "Callback did not receive the expected message.");
        callback_called = 1;
    }

    logger_set_callback(callback);

    LOG(LOG_INFO, "Callback test message");

    cr_assert(callback_called, "Callback was not called.");

    logger_set_callback(NULL);
}

// Test log level from environment variable
Test(logger, log_level_from_env) {
    setenv("LOG_LEVEL", "ERROR", 1);

    logger_set_log_level_from_env();

    cr_assert(logger_get_log_level() == LOG_ERROR, "Log level was not set correctly from the environment variable.");

    unsetenv("LOG_LEVEL");
}

// Test log format options
Test(logger, format_options) {
    const char *test_file = "test_format_options.log";
    remove(test_file);

    cr_assert(logger_set_log_file(test_file), "Failed to set log file.");
    logger_set_format_options(true, true, false);

    LOG(LOG_INFO, "Testing format options");

    FILE *file = fopen(test_file, "r");
    cr_assert(file, "Failed to open log file.");

    char buffer[1024];
    char date[512];

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(date, 512, "%Y-%m-%d ", tm);

    cr_assert(fgets(buffer, sizeof(buffer), file), "Failed to read log file.");
    cr_assert(strstr(buffer, date), "Timestamp is missing from log message.");
    cr_assert(strstr(buffer, "thread"), "Thread ID is missing from log message.");

    fclose(file);
    logger_close_file();
    remove(test_file);
}
