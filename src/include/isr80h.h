#ifndef ISR_80H_H
#define ISR_80H_H


enum SystemCommands
{
    SYSTEM_COMMAND1_PRINT,
};

void isr80h_register_commands();

#endif