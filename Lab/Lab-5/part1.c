#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void printHistogram(int *hist);
int get_hist_index(int tries, int threshold);

#define HIST_SIZE 5000

int main() {
    clock_t begin = clock();

    int hist[HIST_SIZE] = {0};
    srand(time(NULL)); // Call only once!

    int i;
    for (i = 0; i < HIST_SIZE; i++)
        hist[get_hist_index(HIST_SIZE / 2, 49) + (HIST_SIZE / 2)] += 1;
    
    // printHistogram(hist);
    // for (i = 0; i < HIST_SIZE; i++)
    //     printf("%d, ", hist[i]);
    
    clock_t end = clock();
    printf("Execution Time: %f\n", (double)(end - begin) / CLOCKS_PER_SEC);

    return 0;
}

// Generates a random index for historgram
int get_hist_index(int tries, int threshold) {
    int i, counter = 0;
    for (i = 0; i < tries; i++)
    {
        if (rand() % 100 > threshold)
            counter++;
        else
            counter--;
    }
    return counter;
}