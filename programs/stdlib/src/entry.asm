[BITS 64]
section .text
global startup
extern main
extern exit

startup:
    call main
    call exit
    jmp $