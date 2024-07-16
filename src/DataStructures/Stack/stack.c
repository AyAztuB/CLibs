#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
methods:
    new, delete
    init (to provide more parameters like initial capacity)
    copy / clone
    from_array (to init from a C buffer)
    clear (to free? and restore the stack as a new one)
    reserve (to extend the capacity)
    push, pop, peek, is_empty, length
    print

to convert: (destroying the current obj to create the targeted obj)
    from_array (see above)
    to_vector
    to_slice
    to_iterator
    // WARNING: conversion will result in the destruction of the current stack
object
    // to continue to use the current structure, prefere to interprate it as
another type
    // i.e. use a as_<NEW_TYPE> method instead of a to_<NEW_TYPE> method

to interprate: (create a dest obj that is a shallow copy of the current one)
        // WARNING: Idk how to handle the realloc error that should also modifie
the another struct ptr (We should never invalidate a valide DataStructure...)
        // create a shared_ptr / weak_ptr / unique_ptr like in C++ ????
    as_vector
    as_slice
    as_iterator
*/

struct stack {
    size_t length;
    size_t capacity;
    size_t sizeof_elm;
    void *data;
};

/*
    NAMING:

    function:
    <OBJ>_<METHOD>(OBJ, [PARAMS]);

    NEW => DELETE
    CREATE => DESTROY
    ALLOC => FREE

    NEW ?= CREATE(EMPTY) ???
    INIT => NEW / CREATE / ALLOC (With extra/less parameters?? empty??)
*/

/**
 * create an empty new stack with a default capacity of 4.
 * this new stack will be abble to store elm of sizeof(elm) == sizeof_elm
 * provided WARNING: Can return NULL if allocation failed => must be catch by
 * the caller A NULL pointer passed to any function of the library will lead to
 * a SEGV
 */
struct stack *stack_new(size_t sizeof_elm) {
    struct stack *s = malloc(sizeof(*s));
    if (s) {
        s->sizeof_elm = sizeof_elm;
        s->capacity = 4;
        s->length = 0;
        s->data = malloc(4 * sizeof_elm);
        if (!s->data) {
            free(s);
            s = NULL;
        }
    }
    return s;
}

/**
 * delete stack by freeing its allocated memory.
 * can clear content allocation if stack not empty using the free_elm provided
 * function WARNING: free_elm take a pointer to the element to remove...
 */
void stack_delete(struct stack *stack, void (*free_elm)(void *)) {
    if (free_elm && stack->length) {
        for (size_t i = 0; i < stack->length; i++) {
            free_elm(((int8_t *)stack->data) + i * stack->sizeof_elm);
        }
    }
    free(stack->data);
    free(stack);
}

/**
 * stack_new with custom initial capacity (ensure stack have enough place to
 * store initial_capacity elements of size sizeof_elm)
 */
struct stack *stack_init(size_t sizeof_elm, size_t initial_capacity) {
    struct stack *s = malloc(sizeof(*s));
    if (s) {
        s->sizeof_elm = sizeof_elm;
        s->capacity = initial_capacity;
        s->length = 0;
        s->data = malloc(initial_capacity * sizeof_elm);
        if (!s->data) {
            free(s);
            s = NULL;
        }
    }
    return s;
}

// cpy_func take the address of the element to copy and return the address of
// the copyed element example: if stack containes *int => int** cpy_func(int**)
// will be a correct usage signature
struct stack *stack_copy(struct stack *stack, void *(*cpy_func)(void *)) {
    struct stack *s = stack_init(stack->sizeof_elm, stack->length);
    if (s && cpy_func) {
        for (size_t i = 0; i < stack->length; i++) {
            memcpy(((int8_t *)s->data) + i * s->sizeof_elm,
                   cpy_func(((int8_t *)stack->data) + i * stack->sizeof_elm),
                   s->sizeof_elm);
        }
        s->length = stack->length;
    } else if (s) {
        s->length = stack->length;
        memcpy(s->data, stack->data, s->sizeof_elm * s->length);
    }
    return s;
}

/**
 * The content of the array is shadow copied (memcpy) so the buffer can be
 * allocated on the stack call (not necessary heap-allocated) but it's not
 * recommanded to continue to use the buffer after that operation as the stack
 * will take the ownership of the elements inside the given buffer.
 */
struct stack *stack_from_array(void *data, size_t sizeof_elm, size_t length) {
    struct stack *s = stack_init(sizeof_elm, length);
    if (s) {
        s->length = length;
        memcpy(s->data, data, sizeof_elm * length);
    }
    return s;
}
