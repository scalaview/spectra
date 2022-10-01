section .text

global tss_load

tss_load:
    push rbp
    mov rbp, rsp

    mov rax, rdi
    and rax, 0xff
    ltr ax
    pop rbp
    ret