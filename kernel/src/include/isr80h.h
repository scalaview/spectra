#ifndef ISR_80H_H
#define ISR_80H_H


enum SystemCommands
{
    SYSTEM_COMMAND0_PRINT,
    SYSTEM_COMMAND1_SLEEP,
    SYSTEM_COMMAND2_EXIT,
    SYSTEM_COMMAND3_WAIT,
    SYSTEM_COMMAND4_FORK,
    SYSTEM_COMMAND5_EXECVE,
};

void isr80h_register_commands();

#endif