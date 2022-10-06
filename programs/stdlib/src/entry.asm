[BITS 64]
section .text
global startup
extern main_entry

startup:
    call main_entry
    jmp $