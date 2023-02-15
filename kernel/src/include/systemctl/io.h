#ifndef SYSTEM_CTL_IO_H
#define SYSTEM_CTL_IO_H
#include "idt.h"

void* isr80h_command0_print(struct interrupt_frame* frame);
void* isr80h_command8_readkey(struct interrupt_frame* frame);
void* isr80h_command12_fopen(struct interrupt_frame* frame);
void* isr80h_command13_fstat(struct interrupt_frame* frame);
void* isr80h_command14_fread(struct interrupt_frame* frame);
void* isr80h_command15_fclose(struct interrupt_frame* frame);
void* isr80h_command16_fseek(struct interrupt_frame* frame);
#endif