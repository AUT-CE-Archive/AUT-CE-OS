#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    printf(1, "Running test for 'getReadCount' function...\n");
    getReadCount();
    printf(1, "Test completed successfully.\n");
    exit();
}