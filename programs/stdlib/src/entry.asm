[BITS 64]
section .text
global startup
extern main_entry
extern exit

startup:
    call main_entry
    call exit
    jmp $