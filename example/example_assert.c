#include <ayaztub/utils/assert.h>

int main(void) {
    COMPILE_ASSERT(sizeof(size_t) == 8, main_example);
    ASSERT(1 == 1, "%s", "assert correct");

    ASSERT(1 == 0, "%s", "assert should fail");

    // The following example failed to compile !
    // COMPILE_ASSERT(sizeof(char) == sizeof(long), static_assert_fail_example);

    return 0;
}
