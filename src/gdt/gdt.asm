
%include "config.asm"

section .asm
global gdt_load

; void gdt_load(struct gdt* gdt);
gdt_load:
    push rbp
    mov rbp, rsp

    mov rax, rdi
    lgdt [rax]
    pop rbp
    ret


section .bss
align 4096
global ist_1_stack_ptr
global ist_2_stack_ptr
user_land_stack:
    resb RANG3_STACK_SIZE
ist_1_stack_ptr:
    resb RANG3_STACK_SIZE
ist_2_stack_ptr:
