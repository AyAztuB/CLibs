#define DBG_USER_TYPE_MAP(X) X(foo, user_dbg_foo)
#define DBG_ARRAY_USER_TYPE_MAP(X) X(foo *, user_dbg_array_foo)
#define DBG_STATIC_IMPL

#include <ayaztub/utils/debug.h>

typedef struct {
    int x, y;
} foo;

_DBG_FUNC_PREFIX _DBG_FUNC_SIG(user_dbg_foo, foo) {
    fprintf(DBG_OUTSTREAM, _DBG_PRINT_HDR_FMT "(%d, %d)\n", _DBG_PRINT_HDR_ARGS,
            value.x, value.y);
    return value;
}

_DBG_FUNC_PREFIX _DBG_ARRAY_FUNC_SIG(user_dbg_array_foo, foo *) {
    fprintf(DBG_OUTSTREAM, _DBG_ARRAY_PRINT_HDR_FMT, _DBG_ARRAY_PRINT_HDR_ARGS);
    for (size_t i = 0; i < length; i++) {
        if (i)
            fprintf(DBG_OUTSTREAM, ", ");
        fprintf(DBG_OUTSTREAM, "(%d, %d)", array[i].x, array[i].y);
    }
    _DBG_ARRAY_PRINT_FOOTER;
    return array;
}

int main(int argc, char **argv) {
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
    CALL_DBG_ARRAY(dbg_array_const_cstr, (const char **)g,
                   sizeof(g) / sizeof(*g));

    foo myfoo = {
        .x = 2,
        .y = 9,
    };

    dbg(myfoo);
    foo arr_foo[] = { myfoo, myfoo, (foo){ .x = 100, .y = -100 } };
    dbg_array(arr_foo, sizeof(arr_foo) / sizeof(*arr_foo));

    dbg_array(argv, argc);

    set_breakpoint();

    return 0;
}
