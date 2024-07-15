#include <ayaztub.h>
#include <stdio.h>

int main(void) {
    COMPILE_ASSERT(sizeof(size_t) == 8, main_example);
    ASSERT(1 == 1, "assert correct");
    ASSERT(1 == 2, "assert failed");
    logger_set_options((struct logger_options){
        .log_level = WARNING,
        .show_date = true,
#ifdef __linux__
        .show_thread_id = true,
#endif // __linux__
    });
    LOG(DEBUG, "test msg: %zu", 1lu);
    LOG(WARNING, "fst warning: %s", "msg copy");
    FATAL("%s", "Fatal Error");
    return 0;
}
