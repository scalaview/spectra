[BITS 64]
section .text
global startup
extern setup_env
extern main
extern exit

startup:
    call setup_env
    call main
    call exit
    jmp $