#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define COUNT 5

void *handle_philosopher(void *id);
void putdown_chopstick(int phil_id);
void pickup_chopstick(int phil_id);

sem_t take_lock;
sem_t put_down_lock;
sem_t chopstick[COUNT];
int chop_states[COUNT];

int main() {

    // Create threads array for philosophers
    pthread_t philosopher[COUNT];

    // Set all chop sticks to 0
    for (int i = 0; i < COUNT; i++)
        chop_states[i] = 0;

    sem_init(&take_lock, 0 , 1);
    sem_init(&put_down_lock, 0 , 1);

    for (int i = 0; i < COUNT; i++)
        sem_init(&chopstick[i], 0, 1);   //  If pshared has the value 0, then the semaphore is shared between the threads of a process

    // Create and initialze threads
    for (int i = 0; i < COUNT; i++)
        pthread_create(&philosopher[i], NULL, handle_philosopher, (void *)i);

    // Join the threads into a pool
    for (int i = 0; i < COUNT; i++)
        pthread_join(philosopher[i], NULL);
}

// Handles a philosopher actions
void *handle_philosopher (void *id) {
    int phil_id = (int)(long)id;
    int *temp;

    while(1) {
        pickup_chopstick(phil_id);         // take the chop stick
        sleep(1);
        putdown_chopstick(phil_id);     // put down the chop stick
    }
}

void pickup_chopstick(int phil_id){
    sem_wait(&take_lock);  // lock critical region

    // Think philosopher think!!!
    printf("Philosopher %d is thinking!!\n", phil_id);

    sem_wait(&chopstick[phil_id]);
    sem_wait(&chopstick[(phil_id + 1) % COUNT]);

    // Take the chop stick (1 as taken)
    chop_states[phil_id] = 1;
    chop_states[(phil_id + 1) % COUNT] = 1;

    sem_post(&take_lock);   // unlock critical region

    // Eat philosopher eat!!!
    printf("Philosopher %d is eating using chopstick[%d] and chopstick[%d]!!\n", phil_id, phil_id, (phil_id + 1) % COUNT);
}

void putdown_chopstick(int phil_id){

    sem_wait(&put_down_lock);  // lock critical region
    sem_post(&chopstick[phil_id]);
    sem_post(&chopstick[(phil_id + 1) % COUNT]);

    // Free the chop stick (0 as released)
    chop_states[phil_id] = 0;
    chop_states[(phil_id + 1) % COUNT] = 0;

    // Done eating!
    printf("Philosopher %d is finished eating!!\n", phil_id);

    sem_post(&put_down_lock);   // unlock critical region
}