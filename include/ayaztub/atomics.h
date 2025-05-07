/**
 * @file atomics.h
 * @brief Atomic operation macros and typedefs using GCC/Clang `__atomic_*`
 * builtins.
 *
 * This header provides macros for performing atomic operations using the
 * `__atomic_*` builtins provided by GCC 4.7+ or Clang 3.3+. The macros are
 * designed to work with scalar or pointer types that are 1, 2, 4, or 8 bytes in
 * size.
 *
 * Compilation will fail if the compiler does not support the required
 * built-ins.
 *
 * @note All operations use `__ATOMIC_SEQ_CST` memory ordering for maximum
 * safety.
 * @todo Add other atomic operation (even if less common in my use cases)
 * @todo Add support if `__atomic_*` builtins cannot be used
 *
 * Usage example:
 * @code
 * #include <stdio.h>
 * #include <ayaztub/atomics.h>
 *
 * DEFINE_ATOMIC(int);
 *
 * int main() {
 *     atomic_int counter = 0;
 *
 *     ATOMIC_STORE(&counter, 10);
 *     int val = ATOMIC_LOAD(&counter);
 *     printf("Initial: %d\n", val);
 *
 *     ATOMIC_INC(&counter);
 *     printf("After increment: %d\n", ATOMIC_LOAD(&counter));
 *
 *     int expected = 11;
 *     if (ATOMIC_COMPARE_EXCHANGE(&counter, &expected, 20)) {
 *         printf("CAS succeeded: %d\n", ATOMIC_LOAD(&counter));
 *     } else {
 *         printf("CAS failed: expected was %d\n", expected);
 *     }
 *     return 0;
 * }
 * @endcode
 *
 * @example example_atomics.c
 */

#ifndef __AYAZTUB__ATOMICS_H__
#define __AYAZTUB__ATOMICS_H__

#include <ayaztub/utils/util_macros.h>

/** @defgroup compiler builtins detection
 * @{
 */
#if (defined(__GNUC__)                                                         \
     && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)))              \
    || (defined(__clang__)                                                     \
        && (__clang_major__ > 3                                                \
            || (__clang_major__ == 3 && __clang_minor__ >= 3)))
#    define _HAVE___ATOMIC_BUILTINS 1
#else
#    define _HAVE___ATOMIC_BUILTINS 0
#    error                                                                     \
        "Atomic operations not yet implemented without `__atomic_*` builtins support: Please use GCC 4.7+ or Clang 3.3+"
#endif // GCC 4.7+ or Clang 3.3+
/** @} */

#if _HAVE___ATOMIC_BUILTINS == 1

/**
 * @brief Load the value atomically from the given pointer.
 * @param Ptr Pointer to the atomic variable.
 * @return Value loaded from the pointer.
 */
#    define ATOMIC_LOAD(Ptr) __atomic_load_n(Ptr, __ATOMIC_SEQ_CST)

/**
 * @brief Store a value atomically to the given pointer.
 * @param Ptr Pointer to the atomic variable.
 * @param Value Value to store.
 */
#    define ATOMIC_STORE(Ptr, Value)                                           \
        __atomic_store_n(Ptr, Value, __ATOMIC_SEQ_CST)

/**
 * @brief Atomically exchange the value at the pointer with a new value.
 * @param Ptr Pointer to the atomic variable.
 * @param Value New value to set.
 * @return The old value before the exchange.
 */
#    define ATOMIC_EXCHANGE(Ptr, Value)                                        \
        __atomic_exchange_n(Ptr, Value, __ATOMIC_SEQ_CST)

/**
 * @brief Atomically compare and exchange the value.
 * @param Ptr Pointer to the atomic variable.
 * @param PtrExpected Pointer to the expected value.
 * @param DesiredValue Value to store if the comparison succeeds.
 * @return true if the exchange took place, false otherwise.
 */
#    define ATOMIC_COMPARE_EXCHANGE(Ptr, PtrExpected, DesiredValue)            \
        __atomic_compare_exchange_n(Ptr, PtrExpected, DesiredValue, 0,         \
                                    __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)

/**
 * @brief Atomically add a value and return the old value.
 * @param Ptr Pointer to the atomic variable.
 * @param Value Value to add.
 * @return The value before the addition.
 */
#    define ATOMIC_FETCH_ADD(Ptr, Value)                                       \
        __atomic_fetch_add(Ptr, Value, __ATOMIC_SEQ_CST)

/**
 * @brief Atomically increment the value and return the new value.
 * @param Ptr Pointer to the atomic variable.
 * @return The value after incrementing.
 */
#    define ATOMIC_INC(Ptr) __atomic_add_fetch(Ptr, 1, __ATOMIC_SEQ_CST)

/**
 * @brief Atomically decrement the value and return the new value.
 * @param Ptr Pointer to the atomic variable.
 * @return The value after decrementing.
 */
#    define ATOMIC_DEC(Ptr) __atomic_sub_fetch(Ptr, 1, __ATOMIC_SEQ_CST)
#endif // _HAVE___ATOMIC_BUILTINS == 1

/**
 * @brief Declares a volatile (atomic) type.
 * @param Type The base type.
 */
#define DEFINE_ATOMIC(Type) typedef volatile Type atomic_##Type

/**
 * @brief Declares a volatile (atomic) pointer to a type.
 * @param Type The base type.
 */
#define DEFINE_ATOMIC_PTR(Type)                                                \
    typedef volatile Type *GLUE(atomic_, GLUE(Type, _ptr))

/**
 * @brief Shorthand for declaring a volatile atomic type.
 * @param Type The base type.
 */
#define ATOMIC(Type) volatile Type

#endif // __AYAZTUB__ATOMICS_H__
