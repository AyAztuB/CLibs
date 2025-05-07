/**
 * @file util_attributes.h
 * @brief Convenience macros for GCC function, variable, and type attributes.
 *
 * This header provides wrappers for various GCC `__attribute__` extensions.
 *
 * @see
 * GCC attribute reference:
 * https://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
 *
 * @see
 * In particular:
 * https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html
 */

/*
My gcc version: https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc.pdf
    Attributes:
        - of functions: page 552 (6.33)
        - of variables: page 618 (6.34)
        - of types: page 632 (6.35)
        - of labels: page 642 (6.36)
        - of enumerators: page 643 (6.37)
        - of statements: page 643 (6.38)
        - syntax: page 644 (6.39)
List of all attributes:
https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Common-Function-Attributes.html
*/

#ifndef __AYAZTUB__UTILS__UTIL_ATTRIBUTES_H__
#define __AYAZTUB__UTILS__UTIL_ATTRIBUTES_H__

#ifndef __GNUC__
#    define __attribute__(x) ///< Define empty attribute for non-GNUC compilers
#endif // __GNUC__

/**
 * @brief General-purpose attribute wrapper.
 */
#define ATTRIBUTE(X) __attribute__(X)

/**
 * @brief Specifies pointer access mode for function arguments.
 *
 * @param mode     Access mode: "read_only", "write_only", or "read_write".
 * @param ref_idx  Index of the pointer argument being accessed.
 */
#define ACCESS(mode, ref_idx) __attribute__((access(mode, ref_idx)))

/**
 * @brief Creates a symbol alias to another function or variable.
 *
 * @param target  The target symbol name (as a token).
 */
#define ALIAS(target) __attribute__((alias(#target)))

/**
 * @brief Specifies a minimum alignment (in bytes) for a variable or struct.
 */
#define ALIGNED __attribute__((aligned))

/**
 * @brief Forces a function to always be inlined.
 */
#define ALWAYS_INLINE __attribute__((always_inline))

/**
 * @brief Marks a function as having no side effects and not depending on global memory.
 */
#define CONST __attribute__((const))

/**
 * @brief Marks a function to be called automatically before main().
 */
#define CONSTRUCTOR __attribute__((constructor))

/**
 * @brief Marks a function to be called automatically after main() returns.
 */
#define DESTRUCTOR __attribute__((destructor))

/**
 * @brief Marks a function or variable as deprecated.
 */
#define DEPRECATED __attribute__((deprecated))

/**
 * @brief Marks a function or variable as deprecated, with a custom message.
 *
 * @param msg  Message explaining why it's deprecated.
 */
#define DEPRECATED_MSG(msg) __attribute__((deprecated(msg)))

/**
 * @brief Triggers a compile-time error if the function is used.
 *
 * @param msg  Error message to display.
 */
#define ERROR(msg) __attribute__((error(msg)))

/**
 * @brief Triggers a compile-time warning if the function is used.
 *
 * @param msg  Warning message to display.
 */
#define WARNING(msg) __attribute__((warning(msg)))

/**
 * @brief Indicates a function parameter is a file descriptor.
 *
 * @param n  1-based index of the parameter.
 */
#define FD_ARG(n) __attribute__((fd_arg(n)))

/**
 * @brief Indicates a file descriptor parameter is used for reading.
 *
 * @param n  1-based index of the parameter.
 */
#define FD_ARG_READ(n) __attribute__((fd_arg_read(n)))

/**
 * @brief Indicates a file descriptor parameter is used for writing.
 *
 * @param n  1-based index of the parameter.
 */
#define FD_ARG_WRITE(n) __attribute__((fd_arg_write(n)))

/**
 * @brief Specifies printf/scanf-style format checking.
 *
 * @param archetype         "printf", "scanf", etc.
 * @param string_index      1-based index of the format string.
 * @param first_to_check    1-based index of first format argument.
 */
#define FORMAT(archetype, string_index, first_to_check)                        \
    __attribute__((format(archetype, string_index, first_to_check)))

/**
 * @brief Applies format checking to a string-returning function.
 *
 * @param string_index  1-based index of the format string argument.
 */
#define FORMAT_ARG(string_index) __attribute__((format_arg(string_index)))

/**
 * @brief Marks function as returning freshly allocated memory.
 *
 * @param deallocator  Optional deallocation function name.
 */
#define MALLOC(deallocator) __attribute__((malloc(deallocator)))

/**
 * @brief Marks all pointer arguments as non-null.
 */
#define NONNULL __attribute__((nonnull))

/**
 * @brief Marks specific pointer arguments as non-null.
 *
 * @param ...  List of 1-based parameter indices.
 */
#define NONNULL_POSITIONS(...) __attribute__((nonnull(__VA_ARGS__)))

/**
 * @brief Indicates a function never returns.
 */
#define NORETURN __attribute__((noreturn))

/**
 * @brief Indicates the function does not throw exceptions.
 */
#define NOTHROW __attribute__((nothrow))

/**
 * @brief Marks a string argument as null-terminated.
 *
 * @param n  1-based index of the parameter.
 */
#define NULL_TERMINATED_STRING_ARG(n)                                          \
    __attribute__((null_terminated_string_arg(n)))

/**
 * @brief Marks a function as having no side effects except return value.
 */
#define PURE __attribute__((pure))

/**
 * @brief Declares that the function never returns a null pointer.
 */
#define RETURN_NONNULL __attribute__((return_nonnull))

/**
 * @brief Marks the last argument in a variadic function as a sentinel (e.g., NULL).
 */
#define SENTINEL __attribute__((sentinel))

/**
 * @brief Marks a function or variable as unavailable.
 */
#define UNAVAILABLE __attribute__((unavailable))

/**
 * @brief Marks a function or variable as unavailable, with a message.
 *
 * @param msg  Message shown on use.
 */
#define UNAVAILABLE_MSG(msg) __attribute__((unavailable(msg)))

/**
 * @brief Marks a variable or function as possibly unused to suppress warnings.
 */
#define UNUSED __attribute__((unused))

/**
 * @brief Marks a symbol to be emitted even if it appears unused.
 */
#define USED __attribute__((used))

/**
 * @brief Warns if the result of a function is ignored.
 */
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))

/**
 * @brief Hints to the compiler that the following code is unreachable.
 *
 * Used for optimization and to silence warnings about missing return paths.
 */
#ifdef __GNUC__
#    define UNREACHABLE __builtin_unreachable()
#else // __GNUC__
#    define UNREACHABLE (void)0
#endif // __GNUC__

#endif // __AYAZTUB__UTILS__UTIL_ATTRIBUTES_H__
