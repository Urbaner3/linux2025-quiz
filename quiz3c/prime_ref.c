
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "coro.h"

// Forward declaration
void coro_run(void);
void coro_init(void);

static void primeproc(void *_ __attribute__((unused)))
{
    /* Receive a prime number from the left neighbor */
    int p = coro_recv(NULL);
    
    /* Print the prime number with a space after it */
    printf("%d ", p);
    fflush(stdout);

    /* Create a new user-level thread for the right neighbor to continue the
     * chain.
     */
    int id = coro_create(primeproc, NULL);
    if (id == -1) {
        printf("\n");  // Add the final newline
        fflush(stdout);
        coro_exit();
    }

    /* Continuously filter out numbers divisible by the current prime,
     * sending only those that are not divisible to the right neighbor.
     */
    while (1) {
        int i = coro_recv(NULL);
        if (i % p != 0) {
            coro_send(id, i);
        }
        coro_yield();  // Give other coroutines a chance to run
    }
}

void coro_main(void *_ __attribute__((unused)))
{
    /* Create the first prime processing user-level thread in the chain */
    int id = coro_create(primeproc, NULL);
    if (id == -1) {
        printf("Failed to create coroutine\n");
        coro_exit();
    }

    /* Send integers starting from 2 into the chain */
    for (int i = 2; i <= 100; i++) {
        coro_send(id, i);
        coro_yield();  // Give the prime processors a chance to run
    }
    
    // Keep yielding to let the coroutines process all numbers
    for (int i = 0; i < 1000; i++) {
        coro_yield();
    }
    
    // Make sure we have a newline at the end
    printf("\n");
    fflush(stdout);
    
    coro_exit();
}

int main(void) {
    // Initialize the coroutine system
    coro_init();
    
    // Create the main coroutine
    int main_coro = coro_create(coro_main, NULL);
    if (main_coro == -1) {
        printf("Failed to create main coroutine\n");
        return 1;
    }
    
    // Start the coroutine system
    coro_run();
    
    return 0;
}
