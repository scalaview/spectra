[BITS 64]

section .text

global sys_print
sys_print:
    xor rax, rax
    push rdx
    push rsi
    push rdi

    mov rdi, 0  ; command
    mov rsi, 3  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 24
    ret