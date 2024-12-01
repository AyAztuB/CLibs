/**
 * @file assert.h
 * @brief Header file for assertions.
 *
 * This header provides macros for creating compile-time and runtime assertions.
 * Compile-time assertions are useful for validating conditions known during
 * compilation (such as sizeof) and stop the compilation with an error if the
 * conditions are not met.
 *
 * @note You can define NOASSERT to remove assertions.
 */

#ifndef __AYAZTUB__CORE_UTILS__ASSERT_H__
#define __AYAZTUB__CORE_UTILS__ASSERT_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef NOASSERT
#    define COMPILE_ASSERT(predicate, file) ((void)0);
#    define ASSERT(predicate, message) ((void)0);
#else // NOASSERT
#    define __GLUE(a, b) a##b
#    define __CASSERT_IMPL(predicate, file, line)                              \
        typedef char __attribute__((unused))                                   \
        __GLUE(assertion_failed_##file##_, line)[2 * !!(predicate)-1]

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
 * @note You can define NOASSERT to remove the COMPILE_ASSERT block.
 *
 * Example usage:
 * @code
 * COMPILE_ASSERT(sizeof(int) == 4, int_size_check);
 * // Asserts that int is 4 bytes
 * @endcode
 */
#    define COMPILE_ASSERT(predicate, file)                                    \
        __CASSERT_IMPL(predicate, file, __LINE__)

/**
 * @def ASSERT(predicate, fmt, ...)
 * @brief Asserts a runtime condition with a formatted message.
 *
 * This macro checks a runtime condition (predicate). If the condition
 * is false, it prints an error message to stderr with details about the
 * failed assertion, including the file name, line number, and function name,
 * and then aborts the program. The error message is formatted according to
 * the provided format string and arguments.
 *
 * @param predicate The condition to assert. Must be a runtime expression.
 * @param fmt The format string for the error message.
 * @param ... Variadic arguments for the format string.
 *
 * Example usage:
 * @code
 * int value = 10;
 * ASSERT(value == 10, "Value should be %d", 10);
 * // No output, as the condition is true.
 * ASSERT(value == 0, "Value should not be %d", value);
 * // Outputs an assertion failure message and aborts.
 * @endcode
 */
#    define ASSERT(predicate, fmt, ...)                                        \
        do {                                                                   \
            if (!(predicate)) {                                                \
                fprintf(stderr,                                                \
                        "\033[0;31mAssertion failed\033[0m: `%s`, with "       \
                        "message " fmt " (%s:%d in %s())\n",                   \
                        #predicate, __VA_ARGS__, __FILE__, __LINE__,           \
                        __func__);                                             \
                abort();                                                       \
            }                                                                  \
        } while (0)

#endif // NOASSERT

#ifndef assert
/**
 * @def assert(predicate)
 * @brief Asserts a runtime condition with a default message.
 *
 * This macro checks a runtime condition (predicate). If the condition
 * is false, it prints an error message to stderr with details about the
 * failed assertion, including the file name, line number, and function name,
 * and then aborts the program. The error message is set to "failed".
 *
 * @param predicate The condition to assert. Must be a runtime expression.
 *
 * Example usage:
 * @code
 * int value = 10;
 * assert(value == 10); // No output, as the condition is true.
 * assert(value == 0);  // Outputs an assertion failure message and aborts.
 * @endcode
 *
 * @note This macro is only defined if `assert` is not already defined
 * and is an alias to the ASSERT() macro with "failed" as default message.
 *
 * @note Defining NOASSERT will remove the ASSERT block and thus the assert also
 */
#    define assert(predicate) ASSERT(predicate, "%s", "failed")
#endif // assert

#endif // __AYAZTUB__CORE_UTILS__ASSERT_H__
