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
 * #include <ayaztub/utils/debug.h>
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
 *
 * @example example_debug.c
 */

#ifndef __AYAZTUB__UTILS__DEBUG_H__
#define __AYAZTUB__UTILS__DEBUG_H__

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

#define _DBG_PRINT_HDR_FMT                                                     \
    GRAY "%s:%u in %s()" RESET ": " TURQUOISE "%s" RESET " = "
#define _DBG_PRINT_HDR_ARGS file, line, func_name, expr

#define _DBG_ARRAY_PRINT_HDR_FMT                                               \
    GRAY "%s:%u in %s()" RESET ": " TURQUOISE "%s" RESET " = [ "
#define _DBG_ARRAY_PRINT_HDR_ARGS file, line, func_name, expr

#define _DBG_ARRAY_PRINT_FOOTER                                                \
    fprintf(DBG_OUTSTREAM, " ] with length = %zu\n", length)

#define _DBG_FUNC_PREFIX static inline
#define _DBG_FUNC_SIG(name, type)                                              \
    type name(const char *file, unsigned int line, const char *func_name,      \
              const char *expr, type value)
#define _DBG_ARRAY_FUNC_SIG(name, type)                                        \
    type name(const char *file, unsigned int line, const char *func_name,      \
              const char *expr, type array, size_t length)

#define DBG_TYPE_MAP(X)                                                        \
    X(char, dbg_char)                                                          \
    X(signed char, dbg_schar)                                                  \
    X(unsigned char, dbg_uchar)                                                \
    X(short, dbg_short)                                                        \
    X(unsigned short, dbg_ushort)                                              \
    X(int, dbg_int)                                                            \
    X(unsigned int, dbg_uint)                                                  \
    X(long, dbg_long)                                                          \
    X(unsigned long, dbg_ulong)                                                \
    X(long long, dbg_llong)                                                    \
    X(unsigned long long, dbg_ullong)                                          \
    X(float, dbg_float)                                                        \
    X(double, dbg_double)                                                      \
    X(bool, dbg_bool)                                                          \
    X(char *, dbg_char_p)                                                      \
    X(const char *, dbg_const_char_p)                                          \
    X(signed char *, dbg_schar_p)                                              \
    X(const signed char *, dbg_const_schar_p)                                  \
    X(unsigned char *, dbg_uchar_p)                                            \
    X(const unsigned char *, dbg_const_uchar_p)                                \
    X(void *, dbg_pointer)

#define DBG_ARRAY_TYPE_MAP(X)                                                  \
    X(short *, dbg_array_short)                                                \
    X(const short *, dbg_array_const_short)                                    \
    X(unsigned short *, dbg_array_ushort)                                      \
    X(const unsigned short *, dbg_array_const_ushort)                          \
    X(int *, dbg_array_int)                                                    \
    X(const int *, dbg_array_const_int)                                        \
    X(unsigned int *, dbg_array_uint)                                          \
    X(const unsigned int *, dbg_array_const_uint)                              \
    X(long *, dbg_array_long)                                                  \
    X(const long *, dbg_array_const_long)                                      \
    X(unsigned long *, dbg_array_ulong)                                        \
    X(const unsigned long *, dbg_array_const_ulong)                            \
    X(long long *, dbg_array_llong)                                            \
    X(const long long *, dbg_array_const_llong)                                \
    X(unsigned long long *, dbg_array_ullong)                                  \
    X(const unsigned long long *, dbg_array_const_ullong)                      \
    X(float *, dbg_array_float)                                                \
    X(const float *, dbg_array_const_float)                                    \
    X(double *, dbg_array_double)                                              \
    X(const double *, dbg_array_const_double)                                  \
    X(char *, dbg_array_char)                                                  \
    X(const char *, dbg_array_const_char)                                      \
    X(signed char *, dbg_array_schar)                                          \
    X(const signed char *, dbg_array_const_schar)                              \
    X(unsigned char *, dbg_array_uchar)                                        \
    X(const unsigned char *, dbg_array_const_uchar)                            \
    X(bool *, dbg_array_bool)                                                  \
    X(const bool *, dbg_array_const_bool)                                      \
    X(char **, dbg_array_cstr)                                                 \
    X(const char **, dbg_array_const_cstr)

/* Helpers */
#define DBG_GEN(type, fn)                                                      \
type:                                                                          \
    fn,

/* Allow user to extend */
#ifdef DBG_USER_TYPE_MAP
#    define DBG_ALL_TYPES(X)                                                   \
        DBG_TYPE_MAP(X)                                                        \
        DBG_USER_TYPE_MAP(X)
#else
#    define DBG_ALL_TYPES(X) DBG_TYPE_MAP(X)
#endif

/* Allow user to extend */
#ifdef DBG_ARRAY_USER_TYPE_MAP
#    define DBG_ARRAY_ALL_TYPES(X)                                             \
        DBG_ARRAY_TYPE_MAP(X)                                                  \
        DBG_ARRAY_USER_TYPE_MAP(X)
#else
#    define DBG_ARRAY_ALL_TYPES(X) DBG_ARRAY_TYPE_MAP(X)
#endif

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
            _Generic((value), DBG_ALL_TYPES(DBG_GEN) default: dbg_pointer)(    \
                __FILENAME__, __LINE__, __func__, #value, value)

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
                DBG_ARRAY_ALL_TYPES(DBG_GEN) default: dbg_array_pointer)(      \
                __FILENAME__, __LINE__, __func__, #value, value, length)
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

#ifndef GRAY
#    define GRAY "\033[0;2m"
#endif // GRAY
#ifndef RESET
#    define RESET "\033[0m"
#endif // RESET
#ifndef TURQUOISE
#    define TURQUOISE "\033[0;36m"
#endif // TURQUOISE

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
 * // This function can now be called using CALL_DBG() macro
 * int main(int argc, char **argv) {
 *     CALL_DBG(dbg_my_int, argc);
 *     return 0;
 * }
 * @endcode
 */
#define DBG_FUNC_DECL(type, name, fmt)                                         \
    static inline type dbg_##name(const char *file, unsigned int line,         \
                                  const char *func_name, const char *expr,     \
                                  type value) {                                \
        fprintf(DBG_OUTSTREAM, _DBG_PRINT_HDR_FMT fmt "\n",                    \
                _DBG_PRINT_HDR_ARGS, value);                                   \
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
 * // This function can now be called using CALL_DBG_ARRAY() macro
 * int main(int argc, char **argv) {
 *     CALL_DBG_ARRAY(dbg_array_string, (const char **)argv, argc);
 *     return 0;
 * @endcode
 */
#define DBG_ARRAY_FUNC_DECL(type, name, fmt)                                   \
    static inline type dbg_array_##name(                                       \
        const char *file, unsigned int line, const char *func_name,            \
        const char *expr, type array, size_t length) {                         \
        fprintf(DBG_OUTSTREAM, _DBG_ARRAY_PRINT_HDR_FMT,                       \
                _DBG_ARRAY_PRINT_HDR_ARGS);                                    \
        for (size_t i = 0; i < length; i++) {                                  \
            if (i)                                                             \
                fprintf(DBG_OUTSTREAM, ", ");                                  \
            fprintf(DBG_OUTSTREAM, fmt, array[i]);                             \
        }                                                                      \
        _DBG_ARRAY_PRINT_FOOTER;                                               \
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
    fprintf(DBG_OUTSTREAM, _DBG_PRINT_HDR_FMT "%s\n", _DBG_PRINT_HDR_ARGS,
            value ? "true" : "false");
    return value;
}

// Pointers
DBG_FUNC_DECL(void *, pointer, "%p")
DBG_FUNC_DECL(char *, char_p, "\"%s\"")
DBG_FUNC_DECL(const char *, const_char_p, "\"%s\"")

static inline const signed char *
dbg_const_schar_p(const char *file, unsigned int line, const char *func_name,
                  const char *expr, const signed char *value) {
    fprintf(DBG_OUTSTREAM, _DBG_PRINT_HDR_FMT, _DBG_PRINT_HDR_ARGS);
    if (!value)
        fprintf(DBG_OUTSTREAM, "(null)\n");
    else {
        for (size_t i = 0; value[i]; i++) {
            if (i)
                fprintf(DBG_OUTSTREAM, ", ");
            fprintf(DBG_OUTSTREAM, "%hhd", value[i]);
        }
        fprintf(DBG_OUTSTREAM, "\n");
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
    fprintf(DBG_OUTSTREAM, _DBG_PRINT_HDR_FMT, _DBG_PRINT_HDR_ARGS);
    if (!value)
        fprintf(DBG_OUTSTREAM, "(null)\n");
    else {
        for (size_t i = 0; value[i]; i++) {
            if (i)
                fprintf(DBG_OUTSTREAM, ", ");
            fprintf(DBG_OUTSTREAM, "%hhu", value[i]);
        }
        fprintf(DBG_OUTSTREAM, "\n");
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
    fprintf(DBG_OUTSTREAM, _DBG_ARRAY_PRINT_HDR_FMT, _DBG_ARRAY_PRINT_HDR_ARGS);
    for (size_t i = 0; i < length; i++) {
        if (i)
            fprintf(DBG_OUTSTREAM, ", ");
        fprintf(DBG_OUTSTREAM, "%s", array[i] ? "true" : "false");
    }
    _DBG_ARRAY_PRINT_FOOTER;
    return array;
}

static inline bool *dbg_array_bool(const char *file, unsigned int line,
                                   const char *func_name, const char *expr,
                                   bool *array, size_t length) {
    dbg_array_const_bool(file, line, func_name, expr, array, length);
    return array;
}

DBG_ARRAY_FUNC_DECL(char **, cstr, "%s")
DBG_ARRAY_FUNC_DECL(const char **, const_cstr, "%s")
DBG_ARRAY_FUNC_DECL(void **, pointer, "%p")

/**
 * @brief Function to set a breakpoint for debugging.
 *
 * This function raises a `SIGTRAP` signal, which causes the program to stop and
 * enter a state where a debugger can be attached. It is useful for setting
 * breakpoints in code.
 */
void set_breakpoint(void);

#endif // __AYAZTUB__UTILS__DEBUG_H__
