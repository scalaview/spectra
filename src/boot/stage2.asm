[BITS 64]

%include "config.asm"

extern kernel_main
extern gdt64_descriptor
extern init_pic
section .text

global long_cseg
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
    mov rbp, STACK_V
    call init_pic
    mov rdi, [MB_MAGICA]
    mov esi, [MBI_PHYA]

    call kernel_main
    hlt