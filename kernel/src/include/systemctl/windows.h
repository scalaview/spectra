#ifndef SYSTEM_CTL_WINDOWS_H
#define SYSTEM_CTL_WINDOWS_H
#include "idt.h"

void* isr80h_command9_create_window(struct interrupt_frame* frame);

#endif