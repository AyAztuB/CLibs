#include <ayaztub/logger.h>

static void segv_func(int *ptr) {
    if (!ptr)
        LOG_FATAL("ptr is NULL...");
    *ptr = 0;
}

int main(void) {
    // Initialize the logger (happens automatically via constructor)
    logger_set_format_options(true, true, true);
    logger_set_log_level(LOG_DEBUG);
    // If LOG_LEVEL env variable not found or invalid, th log_level is not
    // modified from the previous set.
    logger_set_log_level_from_env();

    // Set logging to a file
    if (!logger_set_log_file_from_env("logfile.txt")) {
        fprintf(stderr, "Failed to open log file\n");
        return 1;
    }

    // Log messages of various levels
    LOG(LOG_INFO, "This is an informational message.");
    LOG(LOG_WARN, "This is a warning message.");
    LOG(LOG_ERROR, "This is an error message.");
    LOG(LOG_DEBUG, "Debugging details: x=%d, y=%d", 69, 96);

    // Demonstrate callback usage (log to stdout)
    logger_set_callback(log_on_stdout);
    LOG(LOG_TRACE, "Trace message with callback active.");

    // Demonstrate backtrace logging on fatal errors / signals
    segv_func(NULL);

    // Clean up (happens automatically via destructor)
    // logger_close_file();

    return 0;
}
