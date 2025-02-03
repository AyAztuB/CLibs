#include <signal.h>
#include <stdlib.h>

void set_breakpoint(void) {
    raise(SIGTRAP);
}
