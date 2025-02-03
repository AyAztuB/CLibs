#ifndef __AYAZTUB__UTILS__UTIL_MACROS_H__
#define __AYAZTUB__UTILS__UTIL_MACROS_H__

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#    ifndef __LITTLE_ENDIAN__
#        define __LITTLE_ENDIAN__
#    endif // __LITTLE_ENDIAN__
#    ifdef __BIG_ENDIAN__
#        undef __BIG_ENDIAN__
#    endif // __BIG_ENDIAN__
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#    ifndef __BIG_ENDIAN__
#        define __BIG_ENDIAN__
#    endif // __BIG_ENDIAN__
#    ifdef __LITTLE_ENDIAN__
#        undef __LITTLE_ENDIAN__
#    endif // __LITTLE_ENDIAN__
#elif !defined(__BYTE_ORDER__) && !defined(__LITTLE_ENDIAN__)                  \
    && !defined(__BIG_ENDIAN__)
#    warning "Endianness cannot be determined !"
#endif

#if defined(__x86_64__) || (defined(__WORDSIZE) && (__WORDSIZE == 64))         \
    || defined(__64BITS__)
#    ifndef __64BITS__
#        define __64BITS__
#    endif // __64BITS__
#    ifdef __32BITS__
#        undef __32BITS__
#    endif // __32BITS__
#elif defined(__i386__) || (defined(__WORDSIZE) && (__WORDSIZE == 32))         \
    || defined(__32BITS__)
#    ifndef __32BITS__
#        define __32BITS__
#    endif // __32BITS__
#else
#    error "Unknown architecture..."
#endif

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

// Integers types

typedef signed char i8;
typedef unsigned char u8;
typedef signed short int i16;
typedef unsigned short int u16;
typedef signed int i32;
typedef unsigned int u32;
#ifdef __64BITS__
typedef signed long int i64;
typedef unsigned long int u64;
#else // __64BITS__
typedef signed long long int i64;
typedef unsigned long long int u64;
#endif // __64BITS__

// Litteral 64 bits integers

#ifdef __64BITS__
#    define LITERAL_I64(i) i##L
#    define LITERAL_U64(i) i##UL
#else // __64BITS__
#    define LITERAL_I64(i) i##LL
#    define LITERAL_U64(i) i##ULL
#endif // __64BITS__

// Interger limits

// For signed numbers, max(iX) = (((iX)(~0)) >> 1)
//                     min(iX) = -max(iX) - 1 = (1 << (X-1))
// For unsigned numbers, max(uX) = ((uX)(-1)) = ((uX)(~0)) = ((max(iX) << 1) +
// 1)

#define I8_MAX (((i8)(~0)) >> 1)
#define I16_MAX (((i16)(~0)) >> 1)
#define I32_MAX (((i32)(~0)) >> 1)
#define I64_MAX (((i64)(~0)) >> 1)

#define I8_MIN ((i8)(1 << (7)))
#define I16_MIN ((i16)(1 << (15)))
#define I32_MIN ((i32)(1 << (31)))
#define I64_MIN ((i64)(1 << (63)))

#define U8_MAX ((u8)(~0))
#define U16_MAX ((u16)(~0))
#define U32_MAX ((u32)(~0))
#define U64_MAX ((u64)(~0))

// Macros to print format types
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

// Some integer macros
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

// Some floating points macros to round and to compare with precision errors

#define PI 3.14159265358979323846
#define DEG2RAD(deg) ((deg) * (PI) / 180.0)
#define RAD2DEG(rad) ((rad)*180.0 / (PI))
// NaN is the only value that is not equal to itself
#define IS_NAN(x) ((x) != (x))

#define FLOAT_EPSILON (1e-6f)
#define DOUBLE_EPSILON (1e-12)
#define ROUND(x) (((x) < 0.0) ? ((i64)((x)-0.5)) : ((i64)((x) + 0.5)))
#define FLOOR(x) (((x) < 0.0) ? ((i64)(x)-1) : ((i64)(x)))
#define CEIL(x)                                                                \
    (((x) == (i64)(x)) ? (i64)(x) : (((x) < 0.0) ? (i64)(x) : ((i64)(x) + 1)))
#define FLOAT_ABS(x) (((x) < 0.0) ? -(x) : (x))
#define FLOAT_ALMOST_EQUALS(x, y) (FLOAT_ABS((x) - (y)) <= FLOAT_EPSILON)
#define DOUBLE_ALMOST_EQUALS(x, y) (FLOAT_ABS((x) - (y)) <= DOUBLE_EPSILON)
#define FLOAT_RELATIVE_EQUAL(x, y)                                             \
    (FLOAT_ABS((x) - (y)) <= (FLOAT_EPSILON * MAX(FLOAT_ABS(x), FLOAT_ABS(y))))
#define DOUBLE_RELATIVE_EQUAL(x, y)                                            \
    (FLOAT_ABS((x) - (y)) <= (DOUBLE_EPSILON * MAX(FLOAT_ABS(x), FLOAT_ABS(y))))

// Bits

#define BIT(x) (1 << (x))
#define SETBIT(x, p) ((x) | (1 << (p)))
#define CLEARBIT(x, p) ((x) & (~(1 << (p))))
#define GETBIT(x, p) (((x) >> (p)) & 1)
#define TOGGLEBIT(x, p) ((x) ^ (1 << (p)))

// Tokens

#define __STR(s) #s
#define STR(s) __STR(s)
#define __GLUE(a, b) a##b
#define GLUE(a, b) __GLUE(a, b)
#define UNIQUE_TOKEN(name) GLUE(GLUE(__, name##_), __LINE__)

// Loops

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

// Error checking

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

/* DEFER, useful to init and clean a block of scoped code */
/* Please see https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2895.htm for a
 * more in depth guide explaining non-standard cleanup functions */

#define DEFER(head, tail)                                                      \
    for (int UNIQUE_TOKEN(defer_var) = (head, 0); !UNIQUE_TOKEN(defer_var);    \
         tail, UNIQUE_TOKEN(defer_var)++)

/* STMT, useful for creating multiple statements macros */

#define STMT(stuff)                                                            \
    do {                                                                       \
        stuff                                                                  \
    } while (0)

#define ONCE(stmts)                                                            \
    do {                                                                       \
        static int UNIQUE_TOKEN(once_var) = 1;                                 \
        if (UNIQUE_TOKEN(once_var)) {                                          \
            STMT(stmts);                                                       \
            UNIQUE_TOKEN(once_var) = 0;                                        \
        }                                                                      \
    } while (0)

#endif // __AYAZTUB__UTILS__UTIL_MACROS_H__
