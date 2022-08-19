[BITS 32]
; [ORG 0x7e00]
global start
extern bootmain

CODE64_SEG equ gdt64_code - gdt64_start
OFFSET      equ 0x7c00
STACK_BP    equ 0x1000

start:
    mov bx, 0x10
    mov ds, bx ; set data segment
    mov es, bx ; set extra segment
    mov ss, bx ; set stack segment
    mov ebp, STACK_BP
    mov esp, OFFSET
    ; The stack region is from 0--start(0x7c00)

    cld
    mov edi, 0x70000
    xor eax, eax
    mov ecx, 0x10000/4
    rep stosd

    mov dword[0x70000], 0x71007
    mov dword[0x71000], 10000111b

    lgdt [gdt64_descriptor]

    mov eax, cr4
    or eax, (1<<5)
    mov cr4, eax

    mov eax, 0x70000
    mov cr3, eax

    mov ecx, 0xc0000080
    rdmsr
    or eax, (1<<8)
    wrmsr

    ; enable paging in the cr0 register
    mov eax, cr0
    or eax, (1<<31)
    mov cr0, eax

    mov ecx, 0xc0000080
    rdmsr
    or eax, (1<<8)
    wrmsr

    jmp CODE64_SEG:long_cseg

gdt64_start:

.gdt64_null:
    dq 0x0000000000000000          ; Null Descriptor - should be present.

gdt64_code:
    dq 0x00209A0000000000          ; 64-bit code descriptor (exec/read).

gdt64_data:
    dq 0x0000920000000000          ; 64-bit data descriptor (read/write).

gdt64_end:

gdt64_descriptor:
    dw gdt64_end - gdt64_start - 1    ; 16-bit Size (Limit) of GDT.
    dd gdt64_start                     ; 32-bit Base Address of GDT. (CPU will zero extend to 64-bit)

[BITS 64]
long_cseg:
    mov bx, 0x10
    mov ds, bx ; set data segment
    mov es, bx ; set extra segment
    mov ss, bx ; set stack segment
    mov ebp, STACK_BP
    mov esp, OFFSET

    call bootmain
    jmp $
