/**
 * @file debug.h
 * @brief Debug utilities for various data types and arrays.
 *
 * This header provides macros and functions to facilitate debugging by printing
 * values of various data types and arrays, along with their expressions, file
 * names, line numbers, and function names.
 * The dbg() macro aims to be similar to the rust dbg!() one.
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
/**
 * @def DBG_OUTSTREAM
 * @brief Macro for the debug output stream
 *
 * This macro allow you to set the debuf output stream as you want
 * (file/stdio/stderr). By default, if undefined, it is an alias of the stderr
 * standard stream.
 */
#    define DBG_OUTSTREAM stderr
#endif // DBG_OUTSTREAM

#ifndef FPRINTF
#    include <stdio.h>
/**
 * @def FPRINTF
 * @brief Macro for formatted output to a file stream.
 *
 * This macro allow you to use your own fprintf function instead of the default
 * one from stdio.h. By default, if undefined, it is an alias of the fprintf
 * standard function.
 */
#    define FPRINTF fprintf
#endif // FPRINTF

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
#define __FILENAME__ ((__FILE__) + (SOURCE_PATH_SIZE))

#ifndef NODBG
#    if __STDC_VERSION__ >= 201112L
/**
 * @def dbg(value)
 * @brief Macro to print a debug message for a variable.
 *
 * This macro prints a debug message for the specified variable, including its
 * name and value.
 * It aims to be the same as the rust dbg!() macro.
 *
 * @note You can remove the dbg() macro defining NODBG. Removing the macro will
 * not remove the value (aka, return of the macro still useful).
 *
 * @warning This macro is only defined for C11 or newer.
 *
 * @param value The variable to debug.
 * @return the value itself.
 *
 * Example usage:
 * @code
 * int x = 42;
 * float f = ((float) dbg(x)) + 0.5f;
 * dbg(f);
 * @endcode
 */
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
        (__FILENAME__, __LINE__, __func__, #value, value)

/**
 * @def dbg_array(value, length)
 * @brief Macro to print a debug message for an array.
 *
 * This macro prints a debug message for the specified array, including its name
 * and elements. It aims to be the same as the rust dbg!() macro but for arrays.
 *
 * @note You can remove the dbg_array() macro defining NODBG. Removing the macro
 * will not remove the value (aka, return of the macro still useful).
 *
 * @warning This macro is only defined for C11 or newer.
 *
 * @param value The array to debug.
 * @param length The length of the array.
 * @return the value array itself.
 *
 * Example usage:
 * @code
 * int arr[] = {1, 2, 3, 4, 5};
 * if (dbg_array(arr, 5)[0] == 0) {
 *     printf("This should not be true...\n");
 * }
 * @endcode
 */
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
        (__FILENAME__, __LINE__, __func__, #value, value, length)
#    else // __STDC_VERSION__ >= 201112L
// macros undefined
#        define dbg(value) (value)
#        define dbg_array(value, length) (value)
#    endif // __STDC_VERSION__ >= 201112L

/**
 * @def CALL_DBG(dbg_func_name, value)
 * @brief Macro to call a debug function for a variable.
 *
 * This macro calls a debug function for the specified variable.
 *
 * @note You can remove the CALL_DBG() macro defining NODBG. Removing the macro
 * will not remove the value (aka, return of the macro still useful).
 *
 * @note Instead of the dbg() macro, this has no C version requirement.
 *
 * @param dbg_func_name The name of the debug function.
 * @param value The variable to debug.
 * @return the value itself.
 *
 * Example usage:
 * @code
 * int x = 69;
 * if ((CALL_DBG(dbg_int, x) + 1) % 10 == 0) {
 *     printf("This is true!\n");
 * }
 * @endcode
 */
#    define CALL_DBG(dbg_func_name, value)                                     \
        dbg_func_name(__FILENAME__, __LINE__, __func__, #value, value)

/**
 * @def CALL_DBG_ARRAY(dbg_func_name, value, length)
 * @brief Macro to call a debug function for an array.
 *
 * This macro calls a debug function for the specified array.
 *
 * @note You can remove the CALL_DBG_ARRAY() macro defining NODBG. Removing the
 * macro will not remove the value (aka, return of the macro still useful).
 *
 * @note Instead of the dbg_array() macro, this has no C version requirement.
 *
 * @param dbg_func_name The name of the debug function.
 * @param value The array to debug.
 * @param length The length of the array.
 * @return the value array itself.
 *
 * Example usage:
 * @code
 * int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 * if (CALL_DBG_ARRAY(dbg_array_int, arr, 10)[0] == 0) {
 *     printf("This is true!\n");
 * }
 * @endcode
 */
#    define CALL_DBG_ARRAY(dbg_func_name, value, length)                       \
        dbg_func_name(__FILENAME__, __LINE__, __func__, #value, value, length)
#else // NODBG
#    define dbg(value) (value)
#    define dbg_array(value, length) (value)
#    define CALL_DBG(dbg_func_name, value) (value)
#    define CALL_DBG_ARRAY(dbg_func_name, value, length) (value)
#endif // NODBG

#define GRAY "\033[0;2m"
#define WHITE "\033[0m"
#define TURQUOISE "\033[0;36m"

/**
 * @def DBG_FUNC_DECL(type, name, fmt)
 * @brief Macro to declare a debug function for a variable.
 *
 * This macro declares a debug function for the specified variable type.
 *
 * @param type The type of the variable.
 * @param name The name of the debug function.
 * @param fmt The format string for the variable value.
 *
 * Example usage:
 * @code
 * DBG_FUNC_DECL(int, my_int, "%d");
 * // create a function following the prototype:
 * // static inline int dbg_my_int(const char *file, unsigned int line,
 * //                              const char *func_name, const char *expr,
 * //                              int value);
 * // This function can new be call using CALL_DBG() macro
 * @endcode
 */
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

/**
 * @def DBG_ARRAY_FUNC_DECL(type, name, fmt)
 * @brief Macro to declare a debug function for an array.
 *
 * This macro declares a debug function for the specified array type.
 *
 * @param type The type of the array elements.
 * @param name The name of the debug function.
 * @param fmt The format string for the array elements.
 *
 * Example usage:
 * @code
 * DBG_ARRAY_FUNC_DECL(const char **, string, "\"%s\"");
 * // create a function following the prototype:
 * // static inline const char **dbg_array_string(const char *file,
 * //                                             unsigned int line,
 * //                                             const char *func_name,
 * //                                             const char *expr,
 * //                                             const char **array,
 * //                                             size_t length);
 * // This function can new be call using CALL_DBG_ARRAY() macro
 * @endcode
 */
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

/**
 * @brief Function to set a breakpoint for debugging.
 *
 * This function raises a `SIGTRAP` signal, which causes the program to stop and
 * enter a state where a debugger can be attached. It is useful for setting
 * breakpoints in code.
 */
void set_breakpoint(void);

#endif // __AYAZTUB__CORE_UTILS__DEBUG_H__
