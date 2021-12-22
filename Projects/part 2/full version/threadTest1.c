#include "types.h"
#include "stat.h"
#include "user.h"

int stack[4096] __attribute__ ((aligned (4096)));
int x = 0;

int main(int argc, char *argv[])
{
    // Create a fork
    int tid = clone(stack);

    // Fork failed
    if (tid < 0) {
        printf(2, "Error!\n");
    // Child process
    } else if (tid == 0) {        
        while(1) {  // Infinity
            x++;
            sleep(100);
        }
    } else {
        while(1) {
            printf(1, "x = %d\n", x);
            sleep(100);
        }
    }

    exit();
}