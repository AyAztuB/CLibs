/**
 * @file util_macros.h
 * @brief Utility macros for low-level systems and general C development.
 *
 * This header provides a collection of utility macros for:
 * - Type aliases for fixed-width integers and floating point numbers
 * - Bitwise operations
 * - Mathematical helpers (min, max, clamp, etc.)
 * - Endianness and architecture detection
 * - Error handling and logging
 * - Debug-friendly logging and scoped cleanups
 * - Token manipulation and safe macros
 */

#ifndef __AYAZTUB__UTILS__UTIL_MACROS_H__
#define __AYAZTUB__UTILS__UTIL_MACROS_H__

/** @defgroup Endianness Architecture Detection Macros
 * Macros to determine platform endianness and bit-width (32/64-bit).
 * @{
 */
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#    ifndef __LITTLE_ENDIAN__
#        define __LITTLE_ENDIAN__ ///< Defined when compiling on little-endian
                                  ///< systems.
#    endif // __LITTLE_ENDIAN__
#    ifdef __BIG_ENDIAN__
#        undef __BIG_ENDIAN__
#    endif // __BIG_ENDIAN__
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#    ifndef __BIG_ENDIAN__
#        define __BIG_ENDIAN__ ///< Defined when compiling on big-endian
                               ///< systems.
#    endif // __BIG_ENDIAN__
#    ifdef __LITTLE_ENDIAN__
#        undef __LITTLE_ENDIAN__
#    endif // __LITTLE_ENDIAN__
#elif !defined(__BYTE_ORDER__) && !defined(__LITTLE_ENDIAN__)                  \
    && !defined(__BIG_ENDIAN__)
#    warning "Endianness cannot be determined !"
#endif
/** @} */

/** @defgroup Architecture Macros
 * Macros for determining 32-bit or 64-bit architecture.
 * @{
 */
#if defined(__x86_64__) || (defined(__WORDSIZE) && (__WORDSIZE == 64))         \
    || defined(__64BITS__)
#    ifndef __64BITS__
#        define __64BITS__ ///< Defined for 64-bit platforms.
#    endif // __64BITS__
#    ifdef __32BITS__
#        undef __32BITS__
#    endif // __32BITS__
#elif defined(__i386__) || (defined(__WORDSIZE) && (__WORDSIZE == 32))         \
    || defined(__32BITS__)
#    ifndef __32BITS__
#        define __32BITS__ ///< Defined for 32-bit platforms.
#    endif // __32BITS__
#else
#    error "Unknown architecture..."
#endif
/** @} */

/**
 * @def SOURCE_PATH_SIZE
 * @brief Macro to truncate the root file path.
 *
 * This macro is useful when the project is compiled with CMake, allowing for a
 * cleaner file path in logs by removing a fixed number of characters from the
 * start of the @c __FILE__ macro. The default value if the macro wasn't defined
 * earlier is 0.
 */
#ifndef SOURCE_PATH_SIZE
#    define SOURCE_PATH_SIZE 0
#endif // SOURCE_PATH_SIZE

/**
 * @brief Macro to adjust the file path based on SOURCE_PATH_SIZE.
 *
 * This macro modifies the @c __FILE__ macro to remove a specified number of
 * characters from the beginning, making the logged file paths more readable.
 */
#ifndef __FILENAME__
#    define __FILENAME__ ((__FILE__) + (SOURCE_PATH_SIZE))
#endif // __FILENAME__

/** @defgroup Type Aliases
 * Short and explicit type aliases for portability and clarity.
 * @{
 */
typedef signed char i8; ///< 8-bit signed integer
typedef unsigned char u8; ///< 8-bit unsigned integer
typedef signed short int i16; ///< 16-bit signed integer
typedef unsigned short int u16; ///< 16-bit unsigned integer
typedef signed int i32; ///< 32-bit signed integer
typedef unsigned int u32; ///< 32-bit unsigned integer

#ifdef __64BITS__
typedef signed long int i64; ///< 64-bit signed integer
typedef unsigned long int u64; ///< 64-bit unsigned integer
#else // __64BITS__
typedef signed long long int i64; ///< 64-bit signed integer
typedef unsigned long long int u64; ///< 64-bit unsigned integer
#endif // __64BITS__

// Floats types
typedef float f32; ///< 32-bit float
typedef double f64; ///< 64-bit float
typedef long double f80; ///< Extended precision float (usually 80-bit)

#if (defined(__GNUC__)                                                         \
     && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3)))
typedef __float128 f128; ///< 128-bit float (GCC 4.3+ extension)
#endif // GNUC 4.3+
/** @} */

/** @defgroup Integer Literals
 * Macros for defining 64-bit literals portably.
 * @{
 */
#ifdef __64BITS__
#    define LITERAL_I64(i) i##L
#    define LITERAL_U64(i) i##UL
#else // __64BITS__
#    define LITERAL_I64(i) i##LL
#    define LITERAL_U64(i) i##ULL
#endif // __64BITS__
/** @} */

/** @defgroup Integer Limits
 * Maximum and minimum values for integer types.
 *
 * @note
 * Signed Max(iX) = (iX)(((uX)(~0)) >> 1) and Min(iX) = -Max(iX)-1 = 1 << (X-1)
 *
 * @note
 * Unsigned Max(uX) = (uX)(-1) = (uX)(~0) = (((uX)(Max(iX))) << 1) + 1
 *
 * @{
 */
#define I8_MAX ((i8)(((u8)(~0)) >> 1))
#define I16_MAX ((i16)(((u16)(~0)) >> 1))
#define I32_MAX ((i32)(((u32)(~0)) >> 1))
#define I64_MAX ((i64)(((u64)(~0)) >> 1))

#define I8_MIN ((i8)(1 << (7)))
#define I16_MIN ((i16)(1 << (15)))
#define I32_MIN ((i32)(1 << (31)))
#define I64_MIN ((i64)(1 << (63)))

#define U8_MAX ((u8)(~0))
#define U16_MAX ((u16)(~0))
#define U32_MAX ((u32)(~0))
#define U64_MAX ((u64)(~0))
/** @} */

/** @defgroup Format Specifiers
 * Macros for `printf` and `scanf` format strings.
 * @{
 */
#define PRI_I8 "hhi"
#define PRI_U8 "hhu"
#define PRI_I16 "hi"
#define PRI_U16 "hu"
#define PRI_I32 "i"
#define PRI_U32 "u"
#ifdef __64BITS__
#    define PRI_I64 "li"
#    define PRI_U64 "lu"
#else
#    define PRI_I64 "lli"
#    define PRI_U64 "llu"
#endif // __64BITS__
/** @} */

/** @defgroup Math Macros
 * Common math operations.
 * @{
 */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CLAMP(val, min, max) (MIN(MAX((val), (min)), (max)))
#define ABS(x) (((x) < 0) ? -(x) : (x))
#define DIFF(a, b) ABS((a) - (b))
#define SWAP(a, b)                                                             \
    do {                                                                       \
        a ^= b;                                                                \
        b ^= a;                                                                \
        a ^= b;                                                                \
    } while (0)
/** @} */

/** @defgroup Floating Point Utilities
 * Macros for comparing and rounding floating point numbers.
 * @{
 */
#define PI 3.14159265358979323846
#define DEG2RAD(deg) ((deg) * (PI) / 180.0)
#define RAD2DEG(rad) ((rad) * 180.0 / (PI))
// NaN is the only value that is not equal to itself
#define IS_NAN(x) ((x) != (x))

#define FLOAT_EPSILON (1e-6f)
#define DOUBLE_EPSILON (1e-12)

#define ROUND(x) (((x) < 0.0) ? ((i64)((x) - 0.5)) : ((i64)((x) + 0.5)))
#define FLOOR(x) (((x) < 0.0) ? ((i64)(x) - 1) : ((i64)(x)))
#define CEIL(x)                                                                \
    (((x) == (i64)(x)) ? (i64)(x) : (((x) < 0.0) ? (i64)(x) : ((i64)(x) + 1)))

#define FLOAT_ABS(x) (((x) < 0.0) ? -(x) : (x))
#define FLOAT_ALMOST_EQUALS(x, y) (FLOAT_ABS((x) - (y)) <= FLOAT_EPSILON)
#define DOUBLE_ALMOST_EQUALS(x, y) (FLOAT_ABS((x) - (y)) <= DOUBLE_EPSILON)
#define FLOAT_RELATIVE_EQUAL(x, y)                                             \
    (FLOAT_ABS((x) - (y)) <= (FLOAT_EPSILON * MAX(FLOAT_ABS(x), FLOAT_ABS(y))))
#define DOUBLE_RELATIVE_EQUAL(x, y)                                            \
    (FLOAT_ABS((x) - (y)) <= (DOUBLE_EPSILON * MAX(FLOAT_ABS(x), FLOAT_ABS(y))))
/** @} */

/** @defgroup Bit Manipulation Macros
 * Simple macros for setting, clearing, and querying bits.
 * @{
 */
#define BIT(x) (1 << (x))
#define SETBIT(x, p) ((x) | (1 << (p)))
#define CLEARBIT(x, p) ((x) & (~(1 << (p))))
#define GETBIT(x, p) (((x) >> (p)) & 1)
#define TOGGLEBIT(x, p) ((x) ^ (1 << (p)))
/** @} */

/** @defgroup Token and Loop Helpers
 * Useful macros for tokens and safer loop constructs.
 * @{
 */
#define __STR(s) #s
#define STR(s) __STR(s)
#define __GLUE(a, b) a##b
#define GLUE(a, b) __GLUE(a, b)
#define UNIQUE_TOKEN(name) GLUE(GLUE(__, name##_), __LINE__)

#define FOREVER for (;;)
#define RANGE(begin, end)                                                      \
    for (i64 UNIQUE_TOKEN(i) = (begin); UNIQUE_TOKEN(i) != (end);              \
         UNIQUE_TOKEN(i) += (((begin) < (end)) ? (+1) : (-1)))
#define RANGE_STEP(begin, end, step)                                           \
    for (i64 UNIQUE_TOKEN(i) = (begin);                                        \
         (((end) > (begin)) && (UNIQUE_TOKEN(i) < (end)))                      \
         || (((end) <= (begin)) && (UNIQUE_TOKEN(i) > (end)));                 \
         UNIQUE_TOKEN(i) +=                                                    \
         ((((begin) < (end)) && ((step) > 0)) ? (+(step)) : (-(step))))
/** @} */

/** @defgroup Error Handling Macros
 * Macros to simplify conditional error handling.
 * @{
 */
#define TRY_GOTO(pred, label)                                                  \
    do {                                                                       \
        if (!(pred)) {                                                         \
            goto label;                                                        \
        }                                                                      \
    } while (0)
#define TRY_THROW(pred)                                                        \
    do {                                                                       \
        if (!(pred)) {                                                         \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    } while (0)
#define TRY_LOG(pred, msg)                                                     \
    do {                                                                       \
        if (!(pred)) {                                                         \
            fprintf(stderr, "Error catched at %s:%s[%d]: %s", __FILENAME__,    \
                    __func__, __LINE__, msg);                                  \
        }                                                                      \
    } while (0)
#define TRY_EXEC(pred, block)                                                  \
    do {                                                                       \
        if (!(pred)) {                                                         \
            do                                                                 \
                block while (0);                                               \
        }                                                                      \
    } while (0)
/** @} */

/** @def DEFER
 * @brief Run `tail` after `head` in a single lexical scope (cleanup emulation).
 *
 * @code
 * DEFER(open_file(), close_file()) {
 *     // do work
 * }
 * @endcode
 *
 * @note
 * Use @c continue instead of @c break when the scope need to be exited before
 * (in error checking context)
 *
 * @see
 * Please see https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2895.htm for a
 * more in depth guide explaining non-standard cleanup functions
 */
#define DEFER(head, tail)                                                      \
    for (int UNIQUE_TOKEN(defer_var) = (head, 0); !UNIQUE_TOKEN(defer_var);    \
         tail, UNIQUE_TOKEN(defer_var)++)

/** @def STMT
 * @brief Multi-statement macro wrapper.
 */
#define STMT(stuff)                                                            \
    do {                                                                       \
        stuff                                                                  \
    } while (0)

/** @def SCOPE
 * @brief Same as STMT but uses `{}` explicitly.
 */
#define SCOPE(block)                                                           \
    do                                                                         \
    block while (0)

/** @def ONCE
 * @brief Execute `stmts` once, ever.
 *
 * @code
 * ONCE(
 *     printf("This happens once.\n");
 * );
 * @endcode
 */
#define ONCE(stmts)                                                            \
    do {                                                                       \
        static int UNIQUE_TOKEN(once_var) = 1;                                 \
        if (UNIQUE_TOKEN(once_var)) {                                          \
            STMT(stmts);                                                       \
            UNIQUE_TOKEN(once_var) = 0;                                        \
        }                                                                      \
    } while (0)

#endif // __AYAZTUB__UTILS__UTIL_MACROS_H__
