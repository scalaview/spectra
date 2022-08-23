#include "printk.h"

void kernel_main(void)
{
    terminal_screen_initialize();
    char* str = "hhhhhhhhhhhhhhhhhhhhhhhhh\n";
    int64_t value = 0x123456789ABCD;
    for (int i = 0;i < 25; i++)
        printk("%d, %s", i, str);

    printk("This value is % \nequal to %d", value);
    while (str) {};
}