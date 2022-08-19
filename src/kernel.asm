[BITS 64]
section .text
extern kernel_main
global start

start:
    mov rsp, 0x200000
    call kernel_main

End:
    hlt
    jmp End