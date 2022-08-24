[BITS 64]
section .text
extern kernel_main
global start

DATA_SEG        equ 0x10
PIC1_COMMAND    equ 0x20
PIC1_DATA       equ 0x21
PIC2_COMMAND    equ 0xA0
PIC2_DATA       equ 0xA1

start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov rbp, 0x00200000
    mov rsp, rbp

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

    jmp $

times 512-($ - $$) db 0
