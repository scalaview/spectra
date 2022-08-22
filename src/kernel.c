#include "printk.h"


void kernel_main(void)
{
    char* string = "Hello and Welcome";
    int64_t value = 0x123456789ABCD;

    printk("This value is equal to %d", value);
    while (string) {};
}