section .asm
%define PIC1_COMMAND        0x20
%define PIC1_DATA           0x21
%define PIC2_COMMAND        0xA0
%define PIC2_DATA           0xA1

extern interrupt_handler

global enable_interrupts
enable_interrupts:
    sti
    ret

global disable_interrupts
disable_interrupts:
    cli
    ret

global init_pic
init_pic:   ; https://wiki.osdev.org/PIC
    ; Remap the master PIC
    mov al, 00010001b
    out PIC1_COMMAND, al ; Tell master PIC
    out PIC2_COMMAND, al ; Tell slave PIC

    mov al, 0x20 ; Interrupt 0x20 is where master ISR should start
    out PIC1_DATA, al
    mov al, 0x28 ; start from 40 in Slave
    out PIC2_DATA, al

    mov al, 0x4 ; 00000100b The slave attached to the master with IRQ2
    out PIC1_DATA, al  ; Set IRQ2 using
    mov al, 0x2
    out PIC2_DATA, al ; Set slave id to 2

    mov al, 0x1 ; 00000001b 8086/88 (MCS-80/85) mode
    out PIC1_DATA, al
    out PIC2_DATA, al
    ; End remap of the master PIC

    ; mov al, 11111111b
    ; out PIC2_COMMAND, al ; disable PIC2
    ret

%macro INT_NOERRCODE 1
  global int%1
  int%1:
    cli
    push 0 ; error code
    %include "save_registers.asm"

    mov rdi, %1
    mov rsi, rsp
    call interrupt_handler

    %include "restore_registers.asm"
    add rsp, 8
    std
    iretq
%endmacro

%macro INT_ERRCODE 1
  global int%1
  int%1:
    cli
    %include "save_registers.asm"
    mov rdi, %1
    mov rsi, rsp
    call interrupt_handler

    %include "restore_registers.asm"
    add rsp, 8
    std
    iretq
%endmacro

; https://wiki.osdev.org/Exceptions
%assign i 0
%rep 512
    %if i = 8 || i = 10 || i = 11 || i = 12 || i = 13 || i = 14 || i = 17 || i = 21
    INT_ERRCODE i
    %else
    INT_NOERRCODE i
    %endif
%assign i i+1
%endrep

global load_idt
load_idt:
    push rbp
    mov rbp, rsp

    lidt [rdi]

    pop rbp
    ret

section .data

%macro interrupt_entry 1
    dq int%1
%endmacro

global interrupt_pointers
interrupt_pointers:
%assign i 0
%rep 512
    interrupt_entry i
%assign i i+1
%endrep