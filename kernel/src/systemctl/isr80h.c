#include "isr80h.h"
#include "idt.h"
#include "systemctl/io.h"

void isr80h_register_commands()
{
    isr80h_register_command(SYSTEM_COMMAND1_PRINT, isr80h_command0_print);
}