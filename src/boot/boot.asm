[BITS 16]
[ORG 0x7c00]

CODE_SEG equ gdt_code - gdt_start

start:
    cli ; Clear Interrupts
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    ; mov sp, 0x7c00

; https://www.felixcloutier.com/x86/cpuid
; https://wiki.osdev.org/Setting_Up_Long_Mode
detect_long_mode_support:
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb not_support_long_mode

    mov eax, 0x80000001
    cpuid
    test edx, (1<<29)
    jz not_support_long_mode
    ; detect 1 Page enable
    ; test edx, (1<<26)
    ; jz not_support_long_mode

; http://www.ctyme.com/intr/rb-1741.htm
memory_info_start:
    mov eax, 0xe820
    mov edx, 0x534d4150  ; ('SMAP')
    mov ecx, 20          ; 20 bytes
    mov dword[0x9000], 0 ; The location to store memory block size
    mov edi, 0x9008      ; The location to store memory info
    xor ebx, ebx
    int 0x15
    jc not_support

get_memory_info:
    add edi, 20
    inc dword[0x9000]
    test ebx, ebx
    jz memory_done

    mov eax, 0xe820
    mov edx, 0x534d4150
    mov ecx, 20
    int 0x15
    jnc get_memory_info

memory_done:
; https://wiki.osdev.org/Protected_Mode
load_protected:
    lgdt [gdt_descriptor]    ; load GDT register with start address of Global Descriptor Table
    mov eax, cr0
    or  eax, 0x1       ; set PE (Protection Enable) bit in CR0 (Control Register 0)
    mov cr0, eax
    ; Perform far jump to selector 08h (offset into GDT, pointing at a 32bit PM code segment descriptor)
    ; to load CS with proper PM32 descriptor)
    jmp CODE_SEG:protect_cseg  ;0x08:protect_cseg

not_support_long_mode:
    mov si, long_mode_not_support
    jmp end

not_support:
    mov si, not_support_message

end:
    call print
    jmp $

print:
    mov bx, 0
.loop:
    lodsb
    cmp al, 0
    je .done
    call print_char
    jmp .loop
.done:
    ret

print_char:
    mov ah, 0eh
    int 0x10
    ret

drive_id:       db 0
long_mode_not_support:  db "Long Mode Not Support"
not_support_message:        db "Panic: an error in boot process"

disk_address_packet: times 16 db 0
memory_block_size:  dw 0
memory_size:        dw 0

; GDT
; Shown below is a NASM assembly implementation of a GDT which opens up all 4 GB of available memory:
; base = 0x00000000, segment limit = 0xffffffff
gdt_start:

; offset 0x0 (0 bytes)
.gdt_null:
    dd 0x0
    dd 0x0

; offset 0x8 (8 bytes)
gdt_code:	; cs should point to this descriptor
	dw 0xffff		; segment limit first 0-15 bits
	dw 0x0			; base first 0-15 bits
	db 0x0			; base 16-23 bits
	db 0x9a			; access byte
	db 0b11001111	; high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x0			; base 24-31 bits

; offset 0x10 (16 bytes)
gdt_data:	; ds, es, fs, gs, and ss should point to this descriptor
	dw 0xffff		; segment limit first 0-15 bits
	dw 0x0			; base first 0-15 bits
	db 0x0			; base 16-23 bits
	db 0x92			; access byte
	db 0b11001111	; high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x0			; base 24-31 bits

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
protect_cseg:
    mov ax, 100
    jmp $

times 510-($ - $$) db 0
dw 0xAA55
