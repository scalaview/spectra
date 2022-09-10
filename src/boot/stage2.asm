[BITS 64]

%include "config.asm"
%define GDT_TSS         0x28

STACK_P    equ (0x7c00 + KERNEL_VMA)
global long_cseg
extern kernel_start
extern kernel_main
extern gdt64_start
; extern gdt64_start.tss
extern gdt64_descriptor
extern tss64
extern tss64.rsp0
extern kernel_stack_top
section .text

long_cseg:
    cli
    mov rax, _start
    jmp rax
_start:
    mov rax, gdt64_descriptor
    lgdt [rax]

    mov bx, DATA_SEG
    mov ss, bx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    mov rbp, (0x70000+KERNEL_VMA)
    call kernel_start
    jmp $

aaaa:
    mov rax, 1010

    ret