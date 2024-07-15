/**
 * @file assert.h
 * @brief Header file for compile-time assertions.
 *
 * This header provides macros for creating compile-time assertions.
 * Compile-time assertions are useful for validating conditions during
 * compilation and generating errors if the conditions are not met.
 *
 * @note You can define NOASSERT to remove assertions.
 */

#ifndef __AYAZTUB__CORE_UTILS__ASSERT_H__
#define __AYAZTUB__CORE_UTILS__ASSERT_H__

#define __GLUE(a, b) a##b
#define __CASSERT_IMPL(predicate, file, line)                                  \
    typedef char __attribute__((unused))                                       \
    __GLUE(assertion_failed_##file##_, line)[2 * !!(predicate)-1]

#ifdef NOASSERT
#    define COMPILE_ASSERT(predicate, file) ((void)0);
#    define ASSERT(predicate, message) ((void)0);
#else // NOASSERT
/**
 * @def COMPILE_ASSERT(predicate, file)
 * @brief Asserts a compile-time condition.
 *
 * This macro asserts a compile-time condition. If the condition is false,
 * it generates a compilation error.
 *
 * @param predicate The condition to assert. Must be a compile-time constant.
 * @param file A unique identifier for the assertion, typically the file name.
 *
 * You can define NOASSERT to remove the COMPILE_ASSERT block.
 *
 * Example usage:
 * @code
 * COMPILE_ASSERT(sizeof(int) == 4, int_size_check);
 * // Asserts that int is 4 bytes
 * @endcode
 */
#    define COMPILE_ASSERT(predicate, file)                                    \
        __CASSERT_IMPL(predicate, file, __LINE__)

#    define ASSERT(predicate, message)                                         \
        do {                                                                   \
            if (!(predicate)) {                                                \
                fprintf(stderr,                                                \
                        "\033[0;31mAssertion failed\033[0m: `%s`, with "       \
                        "message `%s` (%s:%d in %s())\n",                      \
                        #predicate, message, __FILE__, __LINE__, __func__);    \
                abort();                                                       \
            }                                                                  \
        } while (0)

#endif // NOASSERT

#endif // __AYAZTUB__CORE_UTILS__ASSERT_H__
