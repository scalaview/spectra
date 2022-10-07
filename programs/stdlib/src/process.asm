[BITS 64]

section .text

global exit
exit:
    xor rax, rax
    push rdx
    push rsi
    push rdi

    mov rdi, 2  ; command
    mov rsi, 1  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 24
    ret

global sys_wait
sys_wait:
    xor rax, rax
    push rdx
    push rsi
    push rdi

    mov rdi, 3  ; command
    mov rsi, 1  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 24
    ret