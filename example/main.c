#include <stdio.h>
#define FOPEN fopen
#include <ayaztub.h>

int main(void) {
    COMPILE_ASSERT(sizeof(size_t) == 8, main_example);
    ASSERT(1 == 1, "%s", "assert correct");
    // assert(1 == 2);
    logger_set_options((struct logger_options){
        .log_level = WARNING,
        .show_date = true,
#ifdef __linux__
        .show_thread_id = true,
#endif // __linux__
    });
    LOG(DEBUG, "test msg: %zu", 1lu);
    LOG(WARNING, "fst warning: %s", "msg copy");

    LOGGER_SET_OUTFILE("stderr");
    LOG(WARNING, "warning on stderr: %d", 101);
    LOGGER_CLOSE_OUTFILE();

    FATAL("%s", "Fatal Error");
    return 0;
}
