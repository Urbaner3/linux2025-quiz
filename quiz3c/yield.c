#include <stdio.h>

#include "coro.h"

static void yield_thread(void *_ __attribute__((unused)))
{
    printf("Current environment %08x.\n", coro_getid());
    for (int i = 0; i < 5; i++) {
        coro_yield();
        printf("Back in environment %08x, iteration %d.\n", coro_getid(), i);
    }
    printf("All done in environment %08x.\n", coro_getid());
}

void coro_main(void *_ __attribute__((unused)))
{
    for (int i = 0; i < 3; i++)
        coro_create(yield_thread, NULL);
    coro_exit();
}
