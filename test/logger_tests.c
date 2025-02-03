#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <ayaztub/logger.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

// Mock implementation of exit()
void exit(int status) {
    cr_assert_eq(status, EXIT_FAILURE, "Unexpected exit status: %d", status);
    // Prevent actual termination
}

// Declare tests functions
enum log_level logger_get_log_level(void);

static void redirect_all_stdout(void) {
    cr_redirect_stdout();
    cr_redirect_stderr();
}

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

static int file_count_lines(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file)
        return -1;

    int lines = 0;

    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n')
            lines++;
    }

    fclose(file);
    return lines;
}

TestSuite(logger, .timeout = 1);

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

Test(logger, log_level_from_string) {
    logger_set_log_level_from_string("INFO");
    cr_assert(logger_get_log_level() == LOG_INFO, "Log level was not set to INFO.");

    logger_set_log_level_from_string("DEBUG");
    cr_assert(logger_get_log_level() == LOG_DEBUG, "Log level was not set to DEBUG.");

    logger_set_log_level_from_string("INVALID");
    cr_assert(logger_get_log_level() == LOG_DEBUG, "Log level should remain unchanged on invalid input.");
}

// Test log level from environment variable
Test(logger, log_level_from_env) {
    setenv("LOG_LEVEL", "ERROR", 1);

    logger_set_log_level_from_env();
    cr_assert(logger_get_log_level() == LOG_ERROR, "Log level was not set correctly from the environment variable.");

    setenv("LOG_LEVEL", "LOG_FATAL", 1);

    logger_set_log_level_from_env();
    cr_assert(logger_get_log_level() == LOG_FATAL, "Log level was not set correctly from the environment variable.");

    setenv("LOG_LEVEL", "INVALID", 1);

    // Set log level back to default
    logger_set_log_level(LOG_INFO);

    logger_set_log_level_from_env();
    cr_assert(logger_get_log_level() == LOG_INFO, "Log level should stay on previous value (default = INFO) when invalid input is provided.");

    unsetenv("LOG_LEVEL");
}

// Test setting a file that cannot be opened
Test(logger, set_invalid_log_file) {
    // Attempt to open a directory as a file (should fail)
    cr_assert(!logger_set_log_file("/"), "Logger should fail to open a directory as a file.");
}

// Test setting a file from env
Test(logger, set_log_file_from_env) {
    const char *test_file = "test_file.log";
    const char *test_file2 = "test_file2.log";
    remove(test_file);
    remove(test_file2);
    setenv("LOG_FILE", test_file, 1);

    cr_assert(logger_set_log_file_from_env(NULL), "Cannot open log file %s.", test_file);
    LOG(LOG_WARN, "Test logging on %s", test_file);

    cr_assert(file_contains(test_file, "WARN"), "Message not written.");
    cr_assert(file_contains(test_file, "Test logging on test_file.log"), "Message not written.");

    logger_close_file();
    unsetenv("LOG_FILE");

    cr_assert(!logger_set_log_file_from_env(NULL), "Envirenment variable is not defined.");

    cr_assert(logger_set_log_file_from_env(test_file2), "Default file %s should be used.", test_file2);
    LOG(LOG_TRACE, "Not written: LEVEL is still INFO");
    LOG(LOG_TIMEOUT, "Hello, World !");

    cr_assert(!file_contains(test_file2, "TRACE"), "Cannot write TRACE message with default log level set to INFO.");
    cr_assert(!file_contains(test_file, "Hello, World !"), "Message must be written on %s instead of %s.", test_file2, test_file);
    cr_assert(file_contains(test_file2, "Hello, World !"), "Message has to be written on file %s.", test_file2);

    logger_close_file();
    remove(test_file);
    remove(test_file2);
}

// Test setting a file already opened
Test(logger, set_log_file_from_fileno) {
    const char *test_file = "test_file.log";
    remove(test_file);

    FILE *file = fopen(test_file, "w");
    cr_assert(file, "cannot open file %s.", test_file);
    cr_assert(logger_set_log_fileno(file), "cannot register file %s.", test_file);

    LOG(LOG_ERROR, "A log message...");
    cr_assert(file_contains(test_file, "ERROR"), "Log message not written...");
    cr_assert(file_contains(test_file, "A log message..."), "Log message not written...");

    logger_close_file();
    remove(test_file);
}

// Test log on stdout callback
Test(logger, log_on_stdout, .init = redirect_all_stdout) {
    logger_set_callback(log_on_stdout);

    LOG(LOG_INFO, "log on stdout");

    cr_assert_stdout_neq_str("", "stdout must contains words...");

    logger_set_callback(NULL);
}

// Test log on stderr callback
Test(logger, log_on_stderr, .init = redirect_all_stdout) {
    logger_set_callback(log_on_stderr);

    LOG(LOG_INFO, "log on stderr");

    cr_assert_stderr_neq_str("", "stderr must contains words...");

    logger_set_callback(NULL);
}

// Test logging without a log file or callback
Test(logger, log_without_output) {
    // Ensure no crash occurs when logging without a file or callback
    LOG(LOG_INFO, "This log should not cause a crash.");

    cr_assert(true, "Logging without output should not fail.");
}

// Test thread safety (basic)
Test(logger, thread_safety) {
    const char *test_file = "test_thread_safety.log";
    remove(test_file);

    cr_assert(logger_set_log_file(test_file), "Failed to set log file.");

    #pragma omp parallel for
    for (int i = 0; i < 1000; ++i) {
        LOG(LOG_INFO, "Thread safety test %d", i);
    }

    logger_close_file();

    // Ensure file exists and has content
    FILE *file = fopen(test_file, "r");
    cr_assert(file, "Log file was not created in thread safety test.");
    fclose(file);
    cr_assert(file_count_lines(test_file) == 1000, "Log file is missing some messages");

    remove(test_file);
}

// Test log trace on fatal error (simulate fatal log)
Test(logger, log_trace_on_fatal) {
    const char *test_file = "test_fatal.log";
    remove(test_file);

    cr_assert(logger_set_log_file(test_file), "Failed to set log file.");
    logger_set_format_options(false, false, true);

    // We cannot actually crash the test suite, but we can ensure it doesn't log trace unexpectedly
    LOG(LOG_FATAL, "This is a fatal error log.");

    logger_close_file();

    cr_assert(true, "Simulating fatal log should not cause unexpected issues.");
    cr_assert(file_contains(test_file, "FATAL"), "FATAL message was not logged.");
    cr_assert(file_contains(test_file, "This is a fatal error log."), "Log message was not written correctly.");

    remove(test_file);
}


// Test logging large messages
Test(logger, log_large_message) {
    const char *test_file = "test_large_message.log";
    remove(test_file);

    cr_assert(logger_set_log_file(test_file), "Failed to set log file.");

    char large_message[2000];
    memset(large_message, 'A', sizeof(large_message) - 1);
    large_message[sizeof(large_message) - 1] = '\0';

    LOG(LOG_INFO, "%s", large_message);

    cr_assert(file_contains(test_file, "AAA"), "Large message was not logged correctly.");
    cr_assert(file_contains(test_file, "..."), "Large message was not truncated correctly.");

    logger_close_file();
    remove(test_file);
}

// Test resetting log file
Test(logger, reset_log_file) {
    const char *test_file1 = "test_reset_log_file1.log";
    const char *test_file2 = "test_reset_log_file2.log";

    remove(test_file1);
    remove(test_file2);

    cr_assert(logger_set_log_file(test_file1), "Failed to set log file 1.");
    LOG(LOG_INFO, "Message in file 1");

    cr_assert(logger_set_log_file(test_file2), "Failed to reset to log file 2.");
    LOG(LOG_INFO, "Message in file 2");

    cr_assert(!file_contains(test_file2, "Message in file 1"), "Log file 2 contains messages from file 1.");
    cr_assert(file_contains(test_file2, "Message in file 2"), "Log file 2 does not contain the expected message.");

    logger_close_file();
    remove(test_file1);
    remove(test_file2);
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
