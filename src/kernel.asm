[BITS 64]
section .text
extern KMain
global start

start:
    mov rsp,0x200000
    call KMain

End:
    hlt
    jmp End