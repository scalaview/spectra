section .ass
global load_idt

load_idt:
    push rbp
    mov rbp, rsp

    mov rbx, rdi
    lidt [rbx]

    pop rbp
    ret
