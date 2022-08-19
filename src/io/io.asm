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
    push rbp
    mov rbp, rsp

    xor rax, rax
    mov rdx, rdi
    in al, dx

    pop rbp
    ret

insw:
    push rbp
    mov rbp, rsp

    xor rax, rax
    mov rdx, rdi
    in ax, dx

    pop rbp
    ret

outb:
    push rbp
    mov rbp, rsp
    mov rdx, rdi ; unsigned short port
    mov rax, rsi ; unsigned char val
    out dx, al

    pop rbp
    ret

outw:
    push rbp
    mov rbp, rsp

    mov rdx, rdi ; unsigned short port
    mov rax, rsi ; unsigned short val
    out dx, ax

    pop rbp
    ret