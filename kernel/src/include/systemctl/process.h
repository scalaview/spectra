#ifndef SYSTEM_CTL_PROCESS_H
#define SYSTEM_CTL_PROCESS_H
#include "idt.h"

void* isr80h_command1_sleep(struct interrupt_frame* frame);
void* isr80h_command2_exit(struct interrupt_frame* frame);
void* isr80h_command3_wait(struct interrupt_frame* frame);
void* isr80h_command4_fork(struct interrupt_frame* frame);
void* isr80h_command5_execve(struct interrupt_frame* frame);
void* isr80h_command6_malloc(struct interrupt_frame* frame);
void* isr80h_command7_free(struct interrupt_frame* frame);

#endif