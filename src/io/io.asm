section .asm

global insb
; unsigned char insb(unsigned short port);
insb:
    push rbp

    mov rbp, rsp

    xor rax, rax
    mov rdx, rdi
    in al, dx

    pop rbp
    ret

global insw
; unsigned short insw(unsigned short port);
insw:
    push rbp
    mov rbp, rsp

    xor rax, rax
    mov rdx, rdi
    in ax, dx

    pop rbp
    ret

global outb
; void outb(unsigned short port, unsigned char val);
outb:
    push rbp
    mov rbp, rsp
    mov rdx, rdi ; unsigned short port
    mov rax, rsi ; unsigned char val
    out dx, al

    pop rbp
    ret

global outw
; void outw(unsigned short port, unsigned short val);
outw:
    push rbp
    mov rbp, rsp

    mov rdx, rdi ; unsigned short port
    mov rax, rsi ; unsigned short val
    out dx, ax

    pop rbp
    ret