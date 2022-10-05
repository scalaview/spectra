[BITS 64]
section .start_entry
global start
extern main_entry

start:
    call main_entry
    jmp $