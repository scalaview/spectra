#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_desc64 {
    uint16_t offset_1;        // offset bits 0..15
    uint16_t selector;        // a code segment selector in GDT or LDT
    uint8_t  ist0;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
    uint8_t  type_attributes; // gate type, dpl, and p fields
    uint16_t offset_2;        // offset bits 16..31
    uint32_t offset_3;        // offset bits 32..63
    uint32_t zero;            // reserved
} __attribute__((packed));

struct idtr_desc64 {
    uint16_t limit; // Size of descriptor table - 1
    uint64_t base_address; // Base address of the start of the interrupt descriptor table
} __attribute__((packed));

struct interrupt_frame {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t error_code;
    uint64_t rip;
    uint64_t cs;
    uint64_t flags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed));

void idt_initialize();
void interrupt_handler();

extern void enable_interrupts();
extern void disable_interrupts();

// https://wiki.osdev.org/PIC
extern void init_pic();

#endif