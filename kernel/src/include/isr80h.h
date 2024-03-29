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
    SYSTEM_COMMAND6_MALLOC,
    SYSTEM_COMMAND7_FREE,
    SYSTEM_COMMAND8_READKEY,
    SYSTEM_COMMAND9_CREATE_WINDOW,
    SYSTEM_COMMAND10_GET_MESSAGE,
    SYSTEM_COMMAND11_FREE_WINDOW_CONTENT,
    SYSTEM_COMMAND12_FOPEN,
    SYSTEM_COMMAND13_FSTAT,
    SYSTEM_COMMAND14_FREAD,
    SYSTEM_COMMAND15_FLOSE,
    SYSTEM_COMMAND16_FSEEK,
    SYSTEM_COMMAND17_PUSH_MESSAGE
};

void isr80h_register_commands();

#endif