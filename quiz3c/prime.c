#include <stdio.h>
#include <stdlib.h>

#include "coro.h"

static void primeproc(void *_ __attribute__((unused)))
{
    /* Receive a prime number from the left neighbor */
    int p = coro_recv(NULL);
    printf("%d ", p);

    /* Create a new user-level thread for the right neighbor to continue the
     * chain.
     */
    int id = coro_create(primeproc, NULL);
    if (id == -1)
        exit(0);

    /* Continuously filter out numbers divisible by the current prime,
     * sending only those that are not divisible to the right neighbor.
     */
    while (1) {
        int i = coro_recv(NULL);
        if (i % p)
            coro_send(id, i);
    }
}

void coro_main(void *_ __attribute__((unused)))
{
    /* Create the first prime processing user-level thread in the chain */
    int id = coro_create(primeproc, NULL);

    /* Continuously send all integers starting from 2 into the chain */
    for (int i = 2;; i++)
        coro_send(id, i);
}
