section .asm


global load_cr3


load_cr3:
    push rbp
    mov rbp, rsp

    mov rax, cr3

    pop rbp
    ret
