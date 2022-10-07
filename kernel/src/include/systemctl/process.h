#ifndef SYSTEM_CTL_PROCESS_H
#define SYSTEM_CTL_PROCESS_H
#include "idt.h"

void* isr80h_command1_sleep(struct interrupt_frame* frame);

#endif