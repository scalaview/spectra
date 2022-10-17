[BITS 64]
%include "config.asm"

section .text
extern kernel_main
extern gdt64_descriptor
extern init_pic
extern multiboot_magic
extern multiboot_info
extern set_kernel_registers

global long_cseg
long_cseg:
    cli
    mov rax, _start
    jmp rax
_start:
    mov rax, gdt64_descriptor
    lgdt [rax]

    call set_kernel_registers
    xor ax, ax
    mov ss, ax
    mov rbp, STACK_V
    mov rsp, rbp

    call init_pic

    call kernel_main

idle:
    sti
    hlt
    jmp idle