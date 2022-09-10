[BITS 32]
%define VIDMEM		0xB8000
section .text

enable_a20_line: ; Enable A20 Line
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

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
.not_support
    htl