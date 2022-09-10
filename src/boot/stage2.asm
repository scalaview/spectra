[BITS 64]
%include "config.asm"

section .text
extern kernel_main
extern gdt64_descriptor
extern init_pic

global long_cseg
long_cseg:
    cli
    mov rax, _start
    jmp rax
_start:
    mov rax, gdt64_descriptor
    lgdt [rax]

    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov rbp, STACK_V
    mov rsp, rbp

    call init_pic
    mov rdi, [MB_MAGICA]
    mov esi, [MBI_PHYA]

    call kernel_main
    hlt