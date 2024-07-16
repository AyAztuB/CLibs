/**
 * @file debug.h
 * @brief Debug utilities for various data types and arrays.
 *
 * This header provides macros and functions to facilitate debugging by printing
 * values of various data types and arrays, along with their expressions, file
 * names, line numbers, and function names.
 *
 * @note You can define NODBG to remove dbg() and dbg_array() macro call without
 * removing the expression statement
 *
 * @warning The dbg() and dbg_array() macro are only defined for C11 or newer.
 * In version lower than C11, the comportement is like if NODBG was defined.
 * If you work on version lower than C11, you are free to use the CALL_DBG()
 * and CALL_DBG_ARRAY() macros.
 *
 * Usage example:
 * @code
 * #include <ayaztub/core_utils/debug.h>
 *
 * DBG_ARRAY_FUNC_DECL(const char **, string, "\"%s\"")
 *
 * int main(int argc, char **argv) {
 *     if (dbg((bool)(argc == 1))) {
 *          dbg(argv[0]);
 *     }
 *     CALL_DBG_ARRAY(dbg_array_string, (const char **)argv, argc);
 *
 *     set_breakpoint();
 *
 *     return 0;
 * }
 * @endcode
 */

#ifndef __AYAZTUB__CORE_UTILS__DEBUG_H__
#define __AYAZTUB__CORE_UTILS__DEBUG_H__

#include <stdbool.h>

#ifndef DBG_OUTSTREAM
#    include <stdio.h>
#    define DBG_OUTSTREAM stderr
#endif // DBG_OUTSTREAM

#ifndef FPRINTF
#    include <stdio.h>
#    define FPRINTF fprintf
#endif // FPRINTF

#ifndef NODBG
#    if __STDC_VERSION__ >= 201112L
#        define dbg(value)                                                     \
            _Generic((value),                                                  \
             char: dbg_char,                                                   \
             signed char: dbg_schar,                                           \
             unsigned char: dbg_uchar,                                         \
             short: dbg_short,                                                 \
             unsigned short: dbg_ushort,                                       \
             int: dbg_int,                                                     \
             unsigned int: dbg_uint,                                           \
             long: dbg_long,                                                   \
             unsigned long: dbg_ulong,                                         \
             long long: dbg_llong,                                             \
             unsigned long long: dbg_ullong,                                   \
             float: dbg_float,                                                 \
             double: dbg_double,                                               \
             bool: dbg_bool,                                                   \
             char *: dbg_char_p,                                               \
             const char *: dbg_const_char_p,                                   \
             signed char *: dbg_schar_p,                                       \
             const signed char *: dbg_const_schar_p,                           \
             unsigned char *: dbg_uchar_p,                                     \
             const unsigned char *: dbg_const_uchar_p,                         \
             default: dbg_pointer)                                             \
        (__FILE__, __LINE__, __func__, #value, value)
#        define dbg_array(value, length)                                       \
            _Generic((value),                                                  \
             short *: dbg_array_short,                                         \
             const short *: dbg_array_const_short,                             \
             unsigned short *: dbg_array_ushort,                               \
             const unsigned short *: dbg_array_const_ushort,                   \
             int *: dbg_array_int,                                             \
             const int *: dbg_array_const_int,                                 \
             unsigned int *: dbg_array_uint,                                   \
             const unsigned int *: dbg_array_const_uint,                       \
             long *: dbg_array_long,                                           \
             const long *: dbg_array_const_long,                               \
             unsigned long *: dbg_array_ulong,                                 \
             const unsigned long *: dbg_array_const_ulong,                     \
             long long *: dbg_array_llong,                                     \
             const long long *: dbg_array_const_llong,                         \
             unsigned long long *: dbg_array_ullong,                           \
             const unsigned long long *: dbg_array_const_ullong,               \
             float *: dbg_array_float,                                         \
             const float *: dbg_array_const_float,                             \
             double *: dbg_array_double,                                       \
             const double *: dbg_array_const_double,                           \
             char *: dbg_array_char,                                           \
             const char *: dbg_array_const_char,                               \
             signed char *: dbg_array_schar,                                   \
             const signed char *: dbg_array_const_schar,                       \
             unsigned char *: dbg_array_uchar,                                 \
             const unsigned char *: dbg_array_const_uchar,                     \
             bool *: dbg_array_bool,                                           \
             const bool *: dbg_array_const_bool)                               \
        (__FILE__, __LINE__, __func__, #value, value, length)
#    else // __STDC_VERSION__ >= 201112L
// macros undefined
#        define dbg(value) (value)
#        define dbg_array(value, length) (value)
#    endif // __STDC_VERSION__ >= 201112L
#    define CALL_DBG(dbg_func_name, value)                                     \
        dbg_func_name(__FILE__, __LINE__, __func__, #value, value)
#    define CALL_DBG_ARRAY(dbg_func_name, value, length)                       \
        dbg_func_name(__FILE__, __LINE__, __func__, #value, value, length)
#else // NODBG
#    define dbg(value) (value)
#    define dbg_array(value, length) (value)
#    define CALL_DBG(dbg_func_name, value) (value)
#    define CALL_DBG_ARRAY(dbg_func_name, value, length) (value)
#endif // NODBG

#define GRAY "\033[0;2m"
#define WHITE "\033[0m"
#define TURQUOISE "\033[0;36m"

#define DBG_FUNC_DECL(type, name, fmt)                                         \
    static inline type dbg_##name(const char *file, unsigned int line,         \
                                  const char *func_name, const char *expr,     \
                                  type value) {                                \
        FPRINTF(DBG_OUTSTREAM,                                                 \
                GRAY "%s:%u in %s()" WHITE ": " TURQUOISE "%s" WHITE " = " fmt \
                     "\n",                                                     \
                file, line, func_name, expr, value);                           \
        return value;                                                          \
    }

#define DBG_ARRAY_FUNC_DECL(type, name, fmt)                                   \
    static inline type dbg_array_##name(                                       \
        const char *file, unsigned int line, const char *func_name,            \
        const char *expr, type array, size_t length) {                         \
        FPRINTF(DBG_OUTSTREAM,                                                 \
                GRAY "%s:%u in %s()" WHITE ": " TURQUOISE "%s" WHITE " = [ ",  \
                file, line, func_name, expr);                                  \
        for (size_t i = 0; i < length; i++) {                                  \
            if (i)                                                             \
                FPRINTF(DBG_OUTSTREAM, ", ");                                  \
            FPRINTF(DBG_OUTSTREAM, fmt, array[i]);                             \
        }                                                                      \
        FPRINTF(DBG_OUTSTREAM, " ] with length = %zu\n", length);              \
        return array;                                                          \
    }

// Basic types
DBG_FUNC_DECL(char, char, "'%c'")
DBG_FUNC_DECL(signed char, schar, "%hhd")
DBG_FUNC_DECL(unsigned char, uchar, "%hhu")
DBG_FUNC_DECL(short, short, "%hd")
DBG_FUNC_DECL(unsigned short, ushort, "%hu")
DBG_FUNC_DECL(int, int, "%d")
DBG_FUNC_DECL(unsigned int, uint, "%u")
DBG_FUNC_DECL(long, long, "%ld")
DBG_FUNC_DECL(unsigned long, ulong, "%lu")
DBG_FUNC_DECL(long long, llong, "%lld")
DBG_FUNC_DECL(unsigned long long, ullong, "%llu")
DBG_FUNC_DECL(float, float, "%f")
DBG_FUNC_DECL(double, double, "%lf")
// DBG_FUNC_DECL(bool, bool, "%d")

static inline bool dbg_bool(const char *file, unsigned int line,
                            const char *func_name, const char *expr,
                            bool value) {
    FPRINTF(DBG_OUTSTREAM,
            GRAY "%s:%u in %s()" WHITE ": " TURQUOISE "%s" WHITE " = %s\n",
            file, line, func_name, expr, value ? "true" : "false");
    return value;
}

// Pointers
DBG_FUNC_DECL(void *, pointer, "%p")
DBG_FUNC_DECL(char *, char_p, "\"%s\"")
DBG_FUNC_DECL(const char *, const_char_p, "\"%s\"")

static inline const signed char *
dbg_const_schar_p(const char *file, unsigned int line, const char *func_name,
                  const char *expr, const signed char *value) {
    FPRINTF(DBG_OUTSTREAM,
            GRAY "%s:%u in %s()" WHITE ": " TURQUOISE "%s" WHITE " = ", file,
            line, func_name, expr);
    if (!value)
        FPRINTF(DBG_OUTSTREAM, "(null)\n");
    else {
        for (size_t i = 0; value[i]; i++) {
            if (i)
                FPRINTF(DBG_OUTSTREAM, ", ");
            FPRINTF(DBG_OUTSTREAM, "%hhd", value[i]);
        }
        FPRINTF(DBG_OUTSTREAM, "\n");
    }
    return value;
}

static inline signed char *dbg_schar_p(const char *file, unsigned int line,
                                       const char *func_name, const char *expr,
                                       signed char *value) {
    dbg_const_schar_p(file, line, func_name, expr, value);
    return value;
}

static inline const unsigned char *
dbg_const_uchar_p(const char *file, unsigned int line, const char *func_name,
                  const char *expr, const unsigned char *value) {
    FPRINTF(DBG_OUTSTREAM,
            GRAY "%s:%u in %s()" WHITE ": " TURQUOISE "%s" WHITE " = ", file,
            line, func_name, expr);
    if (!value)
        FPRINTF(DBG_OUTSTREAM, "(null)\n");
    else {
        for (size_t i = 0; value[i]; i++) {
            if (i)
                FPRINTF(DBG_OUTSTREAM, ", ");
            FPRINTF(DBG_OUTSTREAM, "%hhu", value[i]);
        }
        FPRINTF(DBG_OUTSTREAM, "\n");
    }
    return value;
}

static inline unsigned char *dbg_uchar_p(const char *file, unsigned int line,
                                         const char *func_name,
                                         const char *expr,
                                         unsigned char *value) {
    dbg_const_uchar_p(file, line, func_name, expr, value);
    return value;
}
// DBG_FUNC_DECL(signed char*, schar_p, "%p")
// DBG_FUNC_DECL(const signed char*, const_schar_p, "%p")
// DBG_FUNC_DECL(unsigned char*, uchar_p, "%p")
// DBG_FUNC_DECL(const unsigned char*, const_uchar_p, "%p")

// Arrays
DBG_ARRAY_FUNC_DECL(short *, short, "%hd")
DBG_ARRAY_FUNC_DECL(const short *, const_short, "%hd")
DBG_ARRAY_FUNC_DECL(unsigned short *, ushort, "%hu")
DBG_ARRAY_FUNC_DECL(const unsigned short *, const_ushort, "%hu")
DBG_ARRAY_FUNC_DECL(int *, int, "%d")
DBG_ARRAY_FUNC_DECL(const int *, const_int, "%d")
DBG_ARRAY_FUNC_DECL(unsigned int *, uint, "%u")
DBG_ARRAY_FUNC_DECL(const unsigned int *, const_uint, "%u")
DBG_ARRAY_FUNC_DECL(long *, long, "%ld")
DBG_ARRAY_FUNC_DECL(const long *, const_long, "%ld")
DBG_ARRAY_FUNC_DECL(unsigned long *, ulong, "%lu")
DBG_ARRAY_FUNC_DECL(const unsigned long *, const_ulong, "%lu")
DBG_ARRAY_FUNC_DECL(long long *, llong, "%lld")
DBG_ARRAY_FUNC_DECL(const long long *, const_llong, "%lld")
DBG_ARRAY_FUNC_DECL(unsigned long long *, ullong, "%llu")
DBG_ARRAY_FUNC_DECL(const unsigned long long *, const_ullong, "%llu")
DBG_ARRAY_FUNC_DECL(float *, float, "%f")
DBG_ARRAY_FUNC_DECL(const float *, const_float, "%f")
DBG_ARRAY_FUNC_DECL(double *, double, "%lf")
DBG_ARRAY_FUNC_DECL(const double *, const_double, "%lf")
DBG_ARRAY_FUNC_DECL(char *, char, "%c")
DBG_ARRAY_FUNC_DECL(const char *, const_char, "%c")
DBG_ARRAY_FUNC_DECL(signed char *, schar, "%hhd")
DBG_ARRAY_FUNC_DECL(const signed char *, const_schar, "%hhd")
DBG_ARRAY_FUNC_DECL(unsigned char *, uchar, "%hhu")
DBG_ARRAY_FUNC_DECL(const unsigned char *, const_uchar, "%hhu")
// DBG_ARRAY_FUNC_DECL(bool *, bool, "%d")
// DBG_ARRAY_FUNC_DECL(const bool *, const_bool, "%d")

static inline const bool *
dbg_array_const_bool(const char *file, unsigned int line, const char *func_name,
                     const char *expr, const bool *array, size_t length) {
    FPRINTF(DBG_OUTSTREAM,
            GRAY "%s:%u in %s()" WHITE ": " TURQUOISE "%s" WHITE " = [ ", file,
            line, func_name, expr);
    for (size_t i = 0; i < length; i++) {
        if (i)
            FPRINTF(DBG_OUTSTREAM, ", ");
        FPRINTF(DBG_OUTSTREAM, "%s", array[i] ? "true" : "false");
    }
    FPRINTF(DBG_OUTSTREAM, " ] with length = %zu\n", length);
    return array;
}

static inline bool *dbg_array_bool(const char *file, unsigned int line,
                                   const char *func_name, const char *expr,
                                   bool *array, size_t length) {
    dbg_array_const_bool(file, line, func_name, expr, array, length);
    return array;
}

void set_breakpoint(void);

#endif // __AYAZTUB__CORE_UTILS__DEBUG_H__
