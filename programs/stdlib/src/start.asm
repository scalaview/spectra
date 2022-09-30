[BITS 64]
section .text
global start
extern user_main

start:
    call user_main
    jmp $