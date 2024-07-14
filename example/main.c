#include <ayaztub.h>
#include <stdio.h>

int main(void) {
    COMPILE_ASSERT(sizeof(size_t) == 8, main_example);
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
