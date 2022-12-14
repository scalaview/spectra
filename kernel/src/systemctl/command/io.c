#include "systemctl/io.h"
#include "printk.h"
#include "drivers/keyboard/keyboard.h"

void* isr80h_command0_print(struct interrupt_frame* frame)
{
    int64_t argc = frame->rsi;
    int64_t* argv = (int64_t*)frame->rdx;
    if (argc < 2)
    {
        printk("missing params in print");
        return 0;
    }
    print_to_screen((char*)argv[0], (int)argv[1], (char)argv[2]);

    return (void*)argv[1];
}


void* isr80h_command8_readkey(struct interrupt_frame* frame)
{
    char c = keyboard_pop();
    return (void*)((int64_t)c);
}