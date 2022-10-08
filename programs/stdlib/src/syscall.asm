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

global sys_sleep
sys_sleep:
    xor rax, rax
    push rdi

    mov rdi, 1  ; command
    mov rsi, 1  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 8
    ret

global exit
exit:
    xor rax, rax
    push rdi

    mov rdi, 2  ; command
    mov rsi, 1  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 8
    ret

global sys_wait
sys_wait:
    xor rax, rax
    push rdi

    mov rdi, 3  ; command
    mov rsi, 1  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 8
    ret

global fork
fork:
    xor rax, rax

    mov rdi, 4  ; command
    mov rsi, 0  ; argc
    mov rdx, 0  ; argv
    int 0x80

    ret

; int sys_execve(const char* pathname, const char* argv, const char* envp, int ring_lev);
global sys_execve
sys_execve:
    xor rax, rax
    push rcx
    push rdx
    push rsi
    push rdi

    mov rdi, 5  ; command
    mov rsi, 4  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 32
    ret