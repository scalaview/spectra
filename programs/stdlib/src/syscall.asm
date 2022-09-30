[BITS 64]

section .text

global sys_print
sys_print:
    xor rax, rax
    push rsi
    push rdi

    mov rdi, 0
    mov rsi, rsp
    int 0x80

    add rsp, 16
    ret