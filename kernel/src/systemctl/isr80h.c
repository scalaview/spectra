#include "isr80h.h"
#include "idt.h"
#include "systemctl/io.h"
#include "systemctl/process.h"

void isr80h_register_commands()
{
    isr80h_register_command(SYSTEM_COMMAND0_PRINT, isr80h_command0_print);
    isr80h_register_command(SYSTEM_COMMAND1_SLEEP, isr80h_command1_sleep);
}