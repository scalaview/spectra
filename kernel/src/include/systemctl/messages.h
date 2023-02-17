#ifndef SYSTEM_CTL_MESSAGES_H
#define SYSTEM_CTL_MESSAGES_H
#include "idt.h"

void* isr80h_command10_get_message(struct interrupt_frame* frame);
void* isr80h_command17_push_message(struct interrupt_frame* frame);

#endif