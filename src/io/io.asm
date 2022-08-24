section .asm

global insb
global insw
global outb
global outw

; unsigned char insb(unsigned short port);
; unsigned short insw(unsigned short port);

; void outb(unsigned short port, unsigned char val);
; void outw(unsigned short port, unsigned short val);

insb:
    %include "pushsp.asm"

    mov rbp, rsp

    xor rax, rax
    mov rdx, rdi
    in al, dx

    %include "popsp.asm"
    ret

insw:
    %include "pushsp.asm"
    mov rbp, rsp

    xor rax, rax
    mov rdx, rdi
    in ax, dx

    %include "popsp.asm"
    ret

outb:
    %include "pushsp.asm"
    mov rbp, rsp
    mov rdx, rdi ; unsigned short port
    mov rax, rsi ; unsigned char val
    out dx, al

    %include "popsp.asm"
    ret

outw:
    %include "pushsp.asm"
    mov rbp, rsp

    mov rdx, rdi ; unsigned short port
    mov rax, rsi ; unsigned short val
    out dx, ax

    %include "popsp.asm"
    ret