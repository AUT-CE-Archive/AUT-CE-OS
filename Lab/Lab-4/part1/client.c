#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ 27

int main()
{
    int shmid;
    key_t key;
    char *shm, *s;

    key = 5678;

    // Locate segment
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach segment to data space
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // Read the data
    for (s = shm; *s != NULL; s++)
        putchar(*s);
    putchar('\n');

    // Send * to identify we have finished reading
    *shm = '*';

    printf("\n- Transmission completed.\n");
    exit(0);
}
