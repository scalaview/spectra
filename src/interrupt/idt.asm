section .asm
PIC1_COMMAND    equ 0x20
PIC1_DATA       equ 0x21
PIC2_COMMAND    equ 0xA0
PIC2_DATA       equ 0xA1

global enable_interrupts
global disable_interrupts
global load_idt
global interrupt_pointers
global init_pic

extern interrupt_handler

enable_interrupts:
    sti
    ret

disable_interrupts:
    cli
    ret

; https://wiki.osdev.org/PIC
init_pic:
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

    mov al, 11111111b
    out PIC2_COMMAND, al ; disable PIC2
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

interrupt_pointers:
%assign i 0
%rep 512
    interrupt_entry i
%assign i i+1
%endrep