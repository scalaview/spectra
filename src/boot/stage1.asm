[BITS 32]
%include "config.asm"

section .text
extern long_cseg
extern enable_a20_line
extern clear_screen
extern detect_long_mode_support
extern set_up_page_tables
extern enable_paging
extern multiboot_magic
extern multiboot_info

global start
start:
    mov dword[MBI_PHYA], ebx
    mov dword[MB_MAGICA], eax
    mov ebp, STACK_P
    mov esp, ebp

    cld
    call clear_screen
    call detect_long_mode_support
    call enable_a20_line

    call set_up_page_tables
    call enable_paging

    lgdt [(gdt64_descriptor - KERNEL_VMA)]
    jmp gdt64_start.kernel_code:(long_cseg - KERNEL_VMA)

    hlt

section .data

global gdt64_start
global gdt64_descriptor
global gdt_tss64

gdt64_start:

.null:
    dq 0x0000000000000000          ; Null Descriptor - should be present.

.kernel_code: equ $ - gdt64_start
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10011000b                 ; Present=1 + DPL=00 + S=1 (system segment) + Type=1000(Execute only)
    db 00100000b                 ; Granularity, 64 bits flag, limit19:16.
    db 0                         ; Base (high).

.kernel_data:
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10010010b                 ; Present=1 + DPL=00 + S=1 + Type=0010(Read/Write)
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).

.user_code:                      ; The code descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 11111000b                 ; Present=1 + DPL=00 + S=1 (system segment) + Type=1000(Execute only)
    db 00100000b                 ; Granularity, 64 bits flag, limit19:16.
    db 0                         ; Base (high).

.user_data:                      ; The data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 11110010b                 ; Present=1 + DPL=00 + S=1 + Type=0010(Read/Write)
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).

.gdt64_end:

gdt_tss64:                           ; The TSS descriptor
    dw 0                         ; Limit
    dw 0                         ; Base (bytes 0-2)
    db 0                         ; Base (byte 3)
    db 10001001b                 ; Type, present
    db 00000000b                 ; Misc
    db 0                         ; Base (byte 4)
    dd 0                         ; Base (bytes 5-8)
    dd 0                         ; Zero / reserved

gdt64_descriptor:
    dw $ - gdt64_start - 1    ; 16-bit Size (Limit) of GDT.
    dq gdt64_start                     ; Base Address of GDT. (CPU will zero extend to 64-bit)
