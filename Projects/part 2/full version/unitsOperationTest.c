#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    printf(1, "Result: %d\n", unit0_operation(4));
    printf(1, "Result: %d\n", unit1_operation(1));
    printf(1, "Result: %d\n", unit2_operation(2));
    unit3_operation(3);
    printf(1, "Test completed successfully.\n");
    exit();
}