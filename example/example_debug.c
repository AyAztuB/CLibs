#include <ayaztub/utils/debug.h>

DBG_ARRAY_FUNC_DECL(const char **, string, "\"%s\"")

int main(void) {
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

    return 0;
}
