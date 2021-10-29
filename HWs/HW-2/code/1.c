#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define   MAX_COUNT  200
#define   BUF_SIZE   100

int main(void)
{
    pid_t  pid;
    int    i;
    char   buf[BUF_SIZE];

    int a = 30; /* address of a in memory is 2000 */
    pid = fork();

    if (pid < 0) {

        printf("fork failed");
        return 1;

    } else if (pid == 0) {

        a = a + 10;
        printf("%d %p", a, &a);
        printf("\n");
    } else {

        a = a - 10;
        printf("%d %p", a, &a);
        printf("\n");        
    }
    return 0;
}