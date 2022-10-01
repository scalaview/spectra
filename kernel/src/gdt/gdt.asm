
%include "config.asm"

section .text
global gdt_load

; void gdt_load(struct gdt* gdt);
gdt_load:
    push rbp
    mov rbp, rsp

    mov rax, rdi
    lgdt [rax]
    pop rbp
    ret
