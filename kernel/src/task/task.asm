section .text

global task_start
global restore_registers
task_start:
    mov rsp, rdi
restore_registers:
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

; void task_context_switch(uint64_t* current, uint64_t next);
; save current task kernel context and switch to next task
global task_context_switch
task_context_switch:
    push rbx
    push rbp
    push r12
    push r13
    push r14
    push r15

    mov [rdi], rsp
    mov rsp, rsi

    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    pop rbx

    ret
