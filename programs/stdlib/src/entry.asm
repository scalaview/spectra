[BITS 64]
section .text
global startup
extern main_entry
extern sys_wait

startup:
    call main_entry
    call sys_wait
    jmp $