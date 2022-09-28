section .text

global task_switch
task_switch:
    mov rsp, rdi
    %include "restore_registers.asm"
    iretq