#include <ayaztub/atomics.h>
#include <ayaztub/utils/util_attributes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4
#define NUM_INCREMENTS 1000000

DEFINE_ATOMIC(int);

atomic_int counter = 0;

void *thread_func(UNUSED void *arg) {
    for (int i = 0; i < NUM_INCREMENTS; ++i) {
        ATOMIC_INC(&counter);
    }
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, thread_func, NULL) != 0) {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    int final = ATOMIC_LOAD(&counter);
    printf("Final counter value: %d (Expected: %d)\n", final,
           NUM_THREADS * NUM_INCREMENTS);

    return 0;
}
