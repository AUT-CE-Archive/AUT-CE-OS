#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


int get_hist_index(int tries, int threshold);

#define HIST_SIZE 50000

int main(int argc, char *argv[]) {
    clock_t begin = clock();

    int fd[2], i, hist[HIST_SIZE] = {0};
    srand(time(NULL)); // Call only once!    

    // Create pipe
    pipe(fd);

    // Child
    if (fork() != 0) {        
        close(fd[0]);   // Close read

        for (i = 0; i < HIST_SIZE / 2; i++)
            hist[get_hist_index(HIST_SIZE / 2, 49) + (HIST_SIZE / 2)] += 1;

        write(fd[1], &hist, sizeof(hist));
                
        close(fd[1]);   // Close write
    }
    // Parent
    else {
        read(fd[0], &hist, sizeof(hist));
        
        for (i = 0; i < HIST_SIZE / 2; i++)
            hist[get_hist_index(HIST_SIZE / 2, 49) + (HIST_SIZE / 2)] += 1;

        write(fd[0], &hist, sizeof(hist));
        
        close(fd[0]);   // Close read
    }

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