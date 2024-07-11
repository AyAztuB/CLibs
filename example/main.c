#include <ayaztub.h>
#include <ayaztub/core_utils/logger.h>
#include <stdio.h>

int main(void) {
    logger_set_options((struct logger_options) {
                           .log_level = WARNING,
                           .show_date = true,
                           .show_thread_id = true,
                       });
    LOG(DEBUG, "test msg: %zu", 1lu);
    LOG(WARNING, "fst warning: %s", "msg copy");
    FATAL("%s", "Fatal Error");
    return 0;
}
