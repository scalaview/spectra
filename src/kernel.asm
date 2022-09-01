[BITS 64]
%include "config.asm"
PIC1_COMMAND    equ 0x20
PIC1_DATA       equ 0x21
PIC2_COMMAND    equ 0xA0
PIC2_DATA       equ 0xA1

section .text

extern kernel_main
global start

global divide_zero

start:
    ; mov rax, QWORD DATA_SEG
    ; mov ds, QWORD rax
    ; mov es, QWORD rax
    ; mov fs, QWORD rax
    ; mov gs, QWORD rax
    ; mov ss, QWORD rax
    mov rbp, QWORD KERNEL_VM_BASE
    mov QWORD rsp, rbp

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

    call kernel_main

end:
    hlt
    jmp end

times 512-($ - $$) db 0
