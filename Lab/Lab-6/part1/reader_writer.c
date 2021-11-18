#include <stdio.h>
#include <pthread.h>

#define THREADS_COUNT 3

void *thread_handler(void *id);
void writer_handler(int id);
void reader_handler(int id);

int value = 0;

int main() {

    // Create threads array
    pthread_t threads[THREADS_COUNT];

    // Create and initialze threads
    for (int i = 0; i < THREADS_COUNT; i++)
        pthread_create(&threads[i], NULL, thread_handler, (void *)i);

    // Join the threads into a pool
    for (int i = 0; i < THREADS_COUNT; i++)
        pthread_join(threads[i], NULL);

    return 0;
}

// Handles a thread
void *thread_handler(void *_id) {
    int id = (int)(long)_id;

    if (id == 0)
        writer_handler(id);
    else
        reader_handler(id);
}

// Handles a writer thread
void writer_handler(int id) {

    value += 1; // Increase value by 1
    printf("Writer changed value to %d\n", value);
    // sleep(1);
}

// Handles a reader thread
void reader_handler(int id) {
    
    printf("Reader [%d] read value of %d\n", id, value);
    // sleep(1);
}