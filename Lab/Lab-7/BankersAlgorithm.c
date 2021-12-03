#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define CUSTOMERS_COUNT 3 // Number of customers
#define RESOURCES_COUNT 6 // Number of resources

// Boundaries for availability, maximum
#define MAX_AVAILABLITY 8
#define MIN_AVAILABLITY 4
#define MAX_MAXIMUM 9
#define MIN_MAXIMUM 6
#define MAX_ALLOCATION 3
#define MIN_ALLOCATION 0

int available[RESOURCES_COUNT];                         // The available amount of each resource
int maximum[CUSTOMERS_COUNT][RESOURCES_COUNT];          // The maximum demand of each customer
int allocation[CUSTOMERS_COUNT][RESOURCES_COUNT];       // The amount currently allocated to each customer
int need[CUSTOMERS_COUNT][RESOURCES_COUNT];             // The remaining need of each customer
pthread_t customers[CUSTOMERS_COUNT];                   // Array of customers

int request_resources(int customer_num, int request[]); // Customer requests resources
int release_resources(int customer_num, int request[]); // Customer returns resources
int is_safe();                                          // Checks saftly of the currenst status of the bank (0: not safe/1: safe)
int get_random(int min, int max);                       // Returns a random number within the given range inclusive
void initialize();                                      // Initializes resource matrices
void print_all(int customer_num, int request[]);        // Prints all matrices and arrays
void update_need();                                     // Updates the need matrix (max - allocated = need)
void *customer(void *_customer_num);                    // Thread handler

pthread_mutex_t mutex;      // Locks the critical reigons

int main()
{
    int i, j, k;

    // Initialize random seed based on time
    srand(time(NULL));

    // Initialize resource matrices
    initialize();

    // Initialize mutex
	pthread_mutex_init(&mutex, NULL);

	// Initialize threads
	for (i = 0; i < CUSTOMERS_COUNT; i++)
		pthread_create(&customers[i], NULL, customer, (void *)i);

	// Join threads in a pool
	for (i = 0; i < CUSTOMERS_COUNT; i++)
		pthread_join(customers[i], NULL);

    return 0;
}

void *customer(void *_customer_num) {
    int customer_num = (int)(long)_customer_num;
    int i, j, k;

    while (1) {

        // Lock
        pthread_mutex_lock(&mutex);

        // Creata a random request
        int request[RESOURCES_COUNT];
        for (i = 0; i < RESOURCES_COUNT; i++)
            request[i] = get_random(0, 5);

        if (get_random(0, 1) == 0) {
            // Requset
            print_all(customer_num, request);
            request_resources(customer_num, request);
            print_all(-1, request);
        } else {
            // Release
            print_all(customer_num, request);
            release_resources(customer_num, request);
            print_all(-1, request);
        }
        printf("\n----------------------------------------------------------------\n");

        // Unlock
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }    
}

// Customer requests resources
int request_resources(int customer_num, int request[])
{
    int i, j, k;

    // Check 1 - Validate whether user has deemanded more that needed
    for (i = 0; i < RESOURCES_COUNT; i++)
        if (request[i] > need[customer_num][i])
        {
            printf("\n- [Request, %d] Exceeded Need!\n", customer_num);
            return -1;
        }

    // Check 2 - Validate if bank has the resources to allocate
    for (i = 0; i < RESOURCES_COUNT; i++)
        if (request[i] > available[i])
        {
            printf("\n- [Request, %d] Not enough resources available. Please wait...\n", customer_num);
            return -1;
        }

    // Check 3 - Simulate allocation and see if its OK. Commit if it was and roll-back if wasn't
    // Pretend allocation
    for (i = 0; i < RESOURCES_COUNT; ++i)
    {
        need[customer_num][i] -= request[i];       // Decrease users need
        available[i] -= request[i];                // Deallocate from the bank
        allocation[customer_num][i] += request[i]; // Allocate to the user
    }

    // Check if the allocation is OK
    if (is_safe())
        printf("\n- [Request, %d] Allocation approved and completed.\n", customer_num);
    else
    { // Allocation will violate safety regulations => Roll-Back
        printf("\n- [Request, %d] Sanity check failed!\n", customer_num);
        for (i = 0; i < RESOURCES_COUNT; ++i)
        {
            need[customer_num][i] += request[i];
            available[i] += request[i];
            allocation[customer_num][i] -= request[i];
        }
        return -1;
    }

    return 0;
}

// Customer releases resources
int release_resources(int customer_num, int request[])
{
    int i, j, k;

    // Check 1 - User has enough to release
    for (i = 0; i < RESOURCES_COUNT; i++)
        if (request[i] > allocation[customer_num][i]) {
            printf("\n- [Release, %d] Not enough resources allocated!\n", customer_num);
            return -1;
        }


    // Check 1 - Users returning more resources than bank can store
    for (i = 0; i < RESOURCES_COUNT; i++)
        if (request[i] + available[i] > maximum[customer_num][i])
        {
            printf("\n- [Release, %d] Exceeded bank capacity!\n", customer_num);
            return -1;
        }

    /*
        No sanity check for transaction safetly regulations!
    */
    for (i = 0; i < RESOURCES_COUNT; ++i)
    {
        need[customer_num][i] += request[i];
        available[i] += request[i];
        allocation[customer_num][i] -= request[i];
    }
    printf("\n- [Release, %d] Much appriciated!\n", customer_num);
}

// Returns a random number within the given range inclusive
int get_random(int min, int max)
{
    return (rand() % (max + 1 - min)) + min;
}

// Initialize the availibility, maximum and allcation
void initialize()
{
    int i, j, k;

    // Initialize available resources
    for (i = 0; i < RESOURCES_COUNT; i++)
        available[i] = get_random(MIN_AVAILABLITY, MAX_AVAILABLITY);

    // Initialize maximum resurces per customer & the current allocation
    for (i = 0; i < CUSTOMERS_COUNT; i++)
        for (j = 0; j < RESOURCES_COUNT; j++)
        {
            maximum[i][j] = get_random(MIN_MAXIMUM, MAX_MAXIMUM);
            allocation[i][j] = get_random(MIN_ALLOCATION, MAX_ALLOCATION);
        }
}

// Updates the need matrix (max - allocated = need)
void update_need()
{
    int i, j, k;

    for (i = 0; i < CUSTOMERS_COUNT; i++)
        for (j = 0; j < RESOURCES_COUNT; j++)
            need[i][j] = maximum[i][j] - allocation[i][j];
}

// Checks saftly of the currenst status of the bank
int is_safe()
{
    int i, j, k;

    // Updated need table
    update_need();

    // Indicates whether the processes has been allocated to the requested resources
    int finish [CUSTOMERS_COUNT] = {0};
    
    // Stores a copy of availble resources in work
    int work [RESOURCES_COUNT];
    for (i = 0; i < RESOURCES_COUNT; i++)
        work[i] = available[i];

    // Sanity check
    for (i = 0; i < CUSTOMERS_COUNT; i++) {
        if (finish[i] == 0) {
            for (j = 0; j < RESOURCES_COUNT; j++) {
                if (need[i][j] <= work[j]) {
                    if (j == RESOURCES_COUNT - 1) {
                        finish[i] = 1;
                        for (k = 0; k < RESOURCES_COUNT; k++)
                            work[k] += allocation[i][k];
                        i = -1; // Repeat the process for the same user one more time
                        break;
                    }
                    else
                        continue;
                }
                else
                    break;
            }
        }
        else
            continue;
    }

    for (i = 0; i < CUSTOMERS_COUNT; i++)
        if (finish[i] == 0)
            return -1;
    return 0;
}

// Print the matrices and arrays of the simulation
void print_all(int customer_num, int request[])
{
    int i, j, k;

    update_need(); // Update needs
    if (customer_num != -1)
        printf("\n     Max       Allocated        Need       Available     Customer\n");
    else
        printf("\n     Max       Allocated        Need       Available\n");

    for (i = 0; i < CUSTOMERS_COUNT; i++)
    {

        // Maximum
        for (j = 0; j < RESOURCES_COUNT; j++)
            printf("%d ", maximum[i][j]);
        printf("| ");

        // Available
        for (j = 0; j < RESOURCES_COUNT; j++)
            printf("%d ", allocation[i][j]);
        printf("| ");

        // need
        for (j = 0; j < RESOURCES_COUNT; j++)
            printf("%d ", need[i][j]);
        printf("| ");

        // Available
        if (i == 0)
        {
            for (j = 0; j < RESOURCES_COUNT; j++)
                printf("%d ", available[j]);
            printf("| ");
        }
        else
            printf("            | ");

        // customer ID
        if (i == customer_num)
        {
            for (j = 0; j < RESOURCES_COUNT; j++)
                printf("%d ", request[j]);
        }

        printf("\n");
    }
}