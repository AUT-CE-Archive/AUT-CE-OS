#include "types.h"
#include "stat.h"
#include "user.h"
#include <stddef.h>

#define PAGESIZE 4096

int
thread_creator(void (*fn) (void*), void *arg)
{
    void *fptr = malloc(2 * (PAGESIZE));    // Assign twice the PAGESIZE to *fptr
    void *stack = NULL;

    // Error allocating memory for stack
    if (fptr == 0)
        return -1;
    
    // Check if *fptr is page aligned
    int mod = (uint)fptr % PAGESIZE;
    if (mod == 0)
        stack = fptr;
    else
        stack = fptr + (PAGESIZE - mod);    

    // Create thread
    int thread_id = clone((void*)stack);    

    // Failed
    if (thread_id < 0)
        printf(1, "Clone Failed.\n");
    // Successful
    else if (thread_id == 0) {
        (fn)(arg);      // Process the function
        free(stack);    // Free the allocated memory after function is finished
        exit();         // Exit the current process
    }
    
    return thread_id;
}