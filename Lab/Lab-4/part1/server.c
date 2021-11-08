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
    char *message = "Hello World!";    

    key = 5678;

    // Create segment
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    
    // Attach segment to data space
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // Write some data
    s = shm;

    size_t i = 0;
    for (; i < 12; i++)
        *s++ = message[i];
    *s = NULL;

    // Wait until data has been read (EOF is declared by *)
    while (*shm != '*')
        sleep(1);

    printf("\n- Transmission completed.\n");
    exit(0);
}