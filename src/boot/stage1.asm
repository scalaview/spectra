%include "config.asm"
extern long_cseg

STACK_P     equ 0x70000
WRITABLE_PRESENT     equ 0b0011
p4_table    equ 0x70000
p3_table    equ 0x71000
p2_table    equ 0x72000
MEMORY_BLOCK_SIZE   equ 0x9000
MEMORY_INFO         equ 0x9008

section .text
bits 32
global start
start:
    mov [MEMORY_INFO], ebx
    mov [MEMORY_BLOCK_SIZE], eax
    mov ebp, STACK_P
    mov esp, ebp

    cld
    call enable_a20_line

    call set_up_page_tables

    call enable_paging
    lgdt [(gdt64_descriptor - KERNEL_VMA)]
    jmp gdt64_start.kernel_code:(long_cseg - KERNEL_VMA)

    hlt

enable_a20_line: ; Enable A20 Line
    in al, 0x92
    or al, 2
    out 0x92, al
    ret
; map 4-level paging, 2Mb page size
set_up_page_tables:
    mov edi, p4_table
    xor eax, eax
    mov ecx, 0x10000/4
    rep stosd

    mov eax, p3_table
    or eax, WRITABLE_PRESENT
    mov [p4_table], eax ; make p4 entry point to p3

    mov ebx, (KERNEL_VMA >> 39)
    and ebx, 0x1ff  ; get p4 index
    mov [p4_table + (ebx * 8)], eax

    mov eax, p2_table
    or eax, WRITABLE_PRESENT
    mov ebx, dword(KERNEL_VMA >> 30)
    and ebx, 0x1ff  ; get p3 index
    mov [p3_table + (ebx * 8)], eax

    ; map 512 entries in p2 table
    mov ecx, 0
.map_p2_table:
    mov eax, PAGE_SIZE
    mul ecx
    or eax, 0b10000011 ; huge(07) + writable(02) + present(01)
    mov [p2_table + (ecx * 8)], eax

    inc ecx
    cmp ecx, 512 ; the p2 was done when maped 512 entries
    jne .map_p2_table

    ret

enable_paging:
    mov eax, cr4
    or eax, (1<<5)
    mov cr4, eax

    mov eax, (p4_table)
    mov cr3, eax

    mov ecx, 0xc0000080
    rdmsr
    or eax, (1<<8)
    wrmsr

    ; enable paging in the cr0 register
    mov eax, cr0
    or eax, (1<<31)
    mov cr0, eax

    ret

section .data
align 4096

; global tss64
; global tss64.rsp0
; tss64:
;     dd 0
; tss64.rsp0:
;     times 3 dq 0 ; RSPn
;     dq 0 ; Reserved
; interrupt_stack_table:
;     dq ist_stack_1 ; IST1, NMI
;     dq ist_stack_2 ; IST2, Double fault
;     dq 0 ; IST3
;     dq 0 ; IST4
;     dq 0 ; IST5
;     dq 0 ; IST6
;     dq 0 ; IST7
;     dq 0 ; Reserved
;     dw 0 ; Reserved
;     dw 0 ; I/O Map Base Address
; tss_size equ $ - tss64 - 1

; section .data
; ist_stack_1:
;     resb 0x1000
; ist_stack_2:

; tss64:
;     dd 0
; tss64.rsp0:
;     times 3 dq 0 ; RSPn
;     dq 0 ; Reserved
; interrupt_stack_table:
;     dq ist_stack_1 ; IST1, NMI
;     dq ist_stack_2 ; IST2, Double fault
;     dq 0 ; IST3
;     dq 0 ; IST4
;     dq 0 ; IST5
;     dq 0 ; IST6
;     dq 0 ; IST7
;     dq 0 ; Reserved
;     dw 0 ; Reserved
;     dw 0 ; I/O Map Base Address
; tss_size equ $ - tss64 - 1
global gdt64_start
; global gdt64_start.tss
global gdt64_descriptor
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
    db 00100000b                ; Granularity, 64 bits flag, limit19:16.
    db 0                         ; Base (high).

.user_data:                      ; The data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 11110010b                 ; Present=1 + DPL=00 + S=1 + Type=0010(Read/Write)
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).
; .tss  equ $ - gdt64_start               ; The TSS descriptor
;     dw tss_size & 0xFFFF         ; Limit
;     dw 0                         ; Base (bytes 0-2)
;     db 0                         ; Base (byte 3)
;     db 10001001b                 ; Type, present
;     db 00000000b                 ; Misc
;     db 0                         ; Base (byte 4)
;     dd 0                         ; Base (bytes 5-8)
;     dd 0                         ; Zero / reserved
gdt64_end:

gdt64_descriptor:
    dw gdt64_end - gdt64_start - 1    ; 16-bit Size (Limit) of GDT.
    dq gdt64_start                     ; Base Address of GDT. (CPU will zero extend to 64-bit)
