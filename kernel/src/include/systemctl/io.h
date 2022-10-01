#ifndef SYSTEM_CTL_IO_H
#define SYSTEM_CTL_IO_H
#include "idt.h"

void* isr80h_command0_print(struct interrupt_frame* frame);

#endif