section .text

global task_switch
task_switch:
    mov rsp, rdi
    %include "restore_registers.asm"
    iretq

global set_user_registers
set_user_registers:
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ret