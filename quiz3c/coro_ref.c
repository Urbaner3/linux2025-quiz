
#include "coro.h"
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <string.h>

#define MAX_THREADS 128
#define STACK_SIZE (128 * 1024)  // Increased stack size to 128KB

typedef struct {
    ucontext_t context;
    int id;
    int active;
    int mailbox;
    int sender_id;
    int has_message;
    void *stack;
} thread_t;

static thread_t threads[MAX_THREADS];
static int current_thread = 0;
static int thread_count = 0;
static ucontext_t scheduler_context;

// Debug function to print thread states
void print_thread_states(void) {
    printf("Thread states:\n");
    for (int i = 0; i < thread_count; i++) {
        printf("Thread %d: active=%d, has_message=%d\n", 
               i, threads[i].active, threads[i].has_message);
    }
}

// Initialize the coroutine system
void coro_init(void) {
    memset(threads, 0, sizeof(threads));
    thread_count = 0;
    current_thread = -1;  // No thread running initially
}

int coro_create(coro_entry entry, void *args) {
    if (thread_count >= MAX_THREADS) {
        return -1;  // Too many threads
    }
    
    int id = thread_count++;
    threads[id].id = id;
    threads[id].active = 1;
    threads[id].has_message = 0;
    threads[id].stack = malloc(STACK_SIZE);
    
    if (!threads[id].stack) {
        threads[id].active = 0;
        thread_count--;
        return -1;  // Failed to allocate stack
    }
    
    // Initialize the context
    getcontext(&threads[id].context);
    threads[id].context.uc_stack.ss_sp = threads[id].stack;
    threads[id].context.uc_stack.ss_size = STACK_SIZE;
    threads[id].context.uc_link = &scheduler_context;  // Return to scheduler
    
    // Make the context
    makecontext(&threads[id].context, (void (*)(void))entry, 1, args);
    
    return id;
}

// Internal scheduler function
static void schedule(void) {
    while (1) {
        // Find the next active thread
        int found = 0;
        int start = (current_thread + 1) % thread_count;
        int i = start;
        
        do {
            if (threads[i].active) {
                found = 1;
                current_thread = i;
                break;
            }
            i = (i + 1) % thread_count;
        } while (i != start);
        
        if (!found) {
            printf("\nAll coroutines completed.\n");
            exit(0);
        }
        
        // Switch to the selected thread
        swapcontext(&scheduler_context, &threads[current_thread].context);
    }
}

void coro_yield(void) {
    int old_thread = current_thread;
    swapcontext(&threads[old_thread].context, &scheduler_context);
}

void coro_exit(void) {
    threads[current_thread].active = 0;
    free(threads[current_thread].stack);
    threads[current_thread].stack = NULL;
    
    // Return to scheduler
    setcontext(&scheduler_context);
}

void coro_destroy(int id) {
    if (id < 0 || id >= thread_count || !threads[id].active) {
        return;  // Invalid ID or already destroyed
    }
    
    threads[id].active = 0;
    free(threads[id].stack);
    threads[id].stack = NULL;
    
    // If destroying the current thread, yield to scheduler
    if (id == current_thread) {
        setcontext(&scheduler_context);
    }
}

int coro_getid(void) {
    return current_thread;
}

int coro_recv(int *who) {
    // Wait for a message
    while (!threads[current_thread].has_message) {
        coro_yield();
    }
    
    // Get the message
    int val = threads[current_thread].mailbox;
    if (who) {
        *who = threads[current_thread].sender_id;
    }
    
    threads[current_thread].has_message = 0;
    return val;
}

void coro_send(int dest, int val) {
    if (dest < 0 || dest >= thread_count || !threads[dest].active) {
        return;  // Invalid destination
    }
    
    // Send the message
    threads[dest].mailbox = val;
    threads[dest].sender_id = current_thread;
    threads[dest].has_message = 1;
}

// Start the coroutine system
void coro_run(void) {
    // Set up the scheduler context
    getcontext(&scheduler_context);
    
    // Allocate a stack for the scheduler
    void* scheduler_stack = malloc(STACK_SIZE);
    if (!scheduler_stack) {
        fprintf(stderr, "Failed to allocate scheduler stack\n");
        exit(1);
    }
    
    scheduler_context.uc_stack.ss_sp = scheduler_stack;
    scheduler_context.uc_stack.ss_size = STACK_SIZE;
    scheduler_context.uc_link = NULL;  // No return context
    
    makecontext(&scheduler_context, schedule, 0);
    
    // Start scheduling
    schedule();
}
