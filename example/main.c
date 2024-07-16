#include <ayaztub.h>

DBG_ARRAY_FUNC_DECL(const char **, string, "\"%s\"")

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

    int a = 5;
    float b = 3.14f;
    double c = 2.718281828;
    char d = 'x';
    char *e = "Hello, world!";

    dbg(a);
    dbg(b);
    dbg(c);
    dbg(d);
    dbg(e);
    bool f = dbg((bool)(a == 0));
    dbg(f);

    char *g[] = { "This", "is", "a", "test", "!" };
    dbg(g);
    CALL_DBG_ARRAY(dbg_array_string, (const char **)g, sizeof(g) / sizeof(*g));

    set_breakpoint();

    LOGGER_SET_OUTFILE("stderr");
    LOG(WARNING, "warning on stderr: %d", 101);
    LOGGER_CLOSE_OUTFILE();

    FATAL("%s", "Fatal Error");
    return 0;
}
