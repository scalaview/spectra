#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "config.h"

typedef int KEYBOARD_CAPS_LOCK_STATE;
typedef int (*KEYBOARD_INIT_FUNCTION)();
typedef void (*KEYBOARD_HANDLER_FUNCTION)();

struct keyboard
{
    KEYBOARD_INIT_FUNCTION init;
    KEYBOARD_HANDLER_FUNCTION handler;
    char name[20];

    struct keyboard* next;
};

struct keyboard_buffer
{
    char buffer[OS_KEYBOARD_BUFFER_SIZE];
    int tail;
    int head;
};

void keyboard_initialize();
void keyboard_push(char c);
char keyboard_pop();

#endif