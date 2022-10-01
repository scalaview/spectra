[BITS 64]
section .text
global start
extern main_entry

start:
    call main_entry
    jmp $