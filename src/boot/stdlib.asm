[BITS 32]
%include "config.asm"

%define VIDMEM		        0xB8000
section .text

global enable_a20_line
enable_a20_line: ; Enable A20 Line
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

global clear_screen
clear_screen:
	pusha
	mov ecx, 0
.clear:
	mov word [VIDMEM + ecx * 2], 0x0F20
	inc ecx
.end:
	cmp ecx, 2000 ;80 * 25 characters
	jne .clear
	popa
    ret

global detect_long_mode_support
detect_long_mode_support:
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb .not_support

    mov eax, 0x80000001
    cpuid
    test edx, (1<<29)
    jz .not_support
    ret
.not_support:
    hlt

global set_up_page_tables
; map 4-level paging, 2Mb page size
set_up_page_tables:
    mov eax, P3_TABLE_PHY
    or eax, WRITABLE_PRESENT
    mov dword[P4_TABLE_PHY], eax ; make p4 entry point to p3

    mov ebx, (KERNEL_VMA >> 39)
    and ebx, 0x1ff  ; get p4 index
    mov dword[P4_TABLE_PHY + (ebx * 8)], eax

    mov eax, P2_TABLE_PHY
    or eax, WRITABLE_PRESENT
    mov ebx, dword(KERNEL_VMA >> 30)
    and ebx, 0x1ff  ; get p3 index
    mov dword[P3_TABLE_PHY + (ebx * 8)], eax

    ; map 512 entries in p2 table
    mov ecx, 0
.map_p2_table:
    mov eax, PAGE_SIZE
    mul ecx
    or eax, 0b10000011 ; huge(07) + writable(02) + present(01)
    mov dword[P2_TABLE_PHY + (ecx * 8)], eax

    inc ecx
    cmp ecx, 512 ; the p2 was done when maped 512 entries
    jne .map_p2_table

    ret

global enable_paging
enable_paging:
    mov eax, cr4
    or eax, (1<<5)
    mov cr4, eax

    mov eax, (P4_TABLE_PHY)
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

global halt
halt:
    hlt

section .data
align 4096

global p4_table
p4_table:
    times 512 dq 0
global p3_table
p3_table:
    times 512 dq 0
global p2_table
p2_table:
    times 512 dq 0