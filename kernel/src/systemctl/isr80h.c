#include "isr80h.h"
#include "idt.h"
#include "systemctl/io.h"
#include "systemctl/process.h"
#include "systemctl/windows.h"

void isr80h_register_commands()
{
    isr80h_register_command(SYSTEM_COMMAND0_PRINT, isr80h_command0_print);
    isr80h_register_command(SYSTEM_COMMAND1_SLEEP, isr80h_command1_sleep);
    isr80h_register_command(SYSTEM_COMMAND2_EXIT, isr80h_command2_exit);
    isr80h_register_command(SYSTEM_COMMAND3_WAIT, isr80h_command3_wait);
    isr80h_register_command(SYSTEM_COMMAND4_FORK, isr80h_command4_fork);
    isr80h_register_command(SYSTEM_COMMAND5_EXECVE, isr80h_command5_execve);
    isr80h_register_command(SYSTEM_COMMAND6_MALLOC, isr80h_command6_malloc);
    isr80h_register_command(SYSTEM_COMMAND7_FREE, isr80h_command7_free);
    isr80h_register_command(SYSTEM_COMMAND8_READKEY, isr80h_command8_readkey);
    isr80h_register_command(SYSTEM_COMMAND9_CREATE_WINDOW, isr80h_command9_create_window_content);
}