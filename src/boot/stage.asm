[BITS 32]
; [ORG 0x7e00]
%include "config.asm"
global start
extern bootmain

CODE64_SEG equ gdt64_start.kernel_code - gdt64_start
OFFSET      equ 0x7c00
STACK_BP    equ 0x1000
WRITABLE_PRESENT     equ 0b0011
p4_table    equ 0x70000
p3_table    equ 0x71000
p2_table    equ 0x72000

start:
    mov bx, 0x10
    mov ds, bx ; set data segment
    mov es, bx ; set extra segment
    mov ss, bx ; set stack segment
    mov ebp, STACK_BP
    mov esp, OFFSET
    ; The stack region is from 0--start(0x7c00)

    cld
    call set_up_page_tables

    call enable_paging

    lgdt [gdt64_descriptor]

    jmp CODE64_SEG:long_cseg

; map 4-level paging, 2Mb page size
set_up_page_tables:
    mov edi, p4_table
    xor eax, eax
    mov ecx, 0x10000/4
    rep stosd

    mov eax, p3_table
    or eax, WRITABLE_PRESENT
    mov [p4_table], eax ; make p4 entry point to p3

    mov ebx, (KERNEL_VM_BASE >> 39)
    and ebx, 0x1ff  ; get p4 index
    mov [p4_table + (ebx * 8)], eax

    mov eax, p2_table
    or eax, WRITABLE_PRESENT
    mov ebx, dword(KERNEL_VM_BASE >> 30)
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

gdt64_start:

.gdt64_null:
    dq 0x0000000000000000          ; Null Descriptor - should be present.

.kernel_code:
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

gdt64_end:

gdt64_descriptor:
    dw gdt64_end - gdt64_start - 1    ; 16-bit Size (Limit) of GDT.
    dq gdt64_start                     ; Base Address of GDT. (CPU will zero extend to 64-bit)

[BITS 64]
long_cseg:
    mov rax, long_main
    jmp rax

long_main:
    mov rax, gdt64_descriptor
    lgdt [rax]

    mov rax, DATA_SEG
    mov ds, rax
    mov es, rax
    mov fs, rax
    mov gs, rax
    mov ss, rax
    ; setup new stack
    mov rbp, KERNEL_VMA
    mov rsp, rbp

    call bootmain
    jmp $
