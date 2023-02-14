#ifndef SYSTEM_CTL_WINDOWS_H
#define SYSTEM_CTL_WINDOWS_H
#include "idt.h"

void* isr80h_command9_create_window_content(struct interrupt_frame* frame);
void* isr80h_command11_free_window_content(struct interrupt_frame* frame);

#endif