#ifndef ISR_80H_H
#define ISR_80H_H


enum SystemCommands
{
    SYSTEM_COMMAND0_PRINT,
    SYSTEM_COMMAND1_SLEEP,
};

void isr80h_register_commands();

#endif