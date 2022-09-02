[BITS 16]
[ORG 0x7c00]

CODE32_SEG equ gdt32_code - gdt32_start
SMAP        equ 0x534d4150
STACK_P      equ 0x7c00
LOADER_ADDR equ 0x7e00
MEMORY_BLOCK_SIZE   equ 0x9000
MEMORY_INFO         equ 0x9008

start:
    cli ; Clear Interrupts
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, STACK_P
    mov bp, sp

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
    mov dword[MEMORY_BLOCK_SIZE], 0 ; The location to store memory block size
    mov edi, MEMORY_INFO      ; The location to store memory info
    xor ebx, ebx
.get_memory_info:
    mov eax, 0xe820
    mov edx, SMAP       ; ('SMAP')
    mov ecx, 20         ; 20 bytes
    int 0x15
    jc not_support
.calculate:
    add edi, 20
    inc dword[MEMORY_BLOCK_SIZE]
    test ebx, ebx
    jnz .get_memory_info

.memory_done:
; https://wiki.osdev.org/Protected_Mode
load_protected:
    lgdt [gdt32_descriptor]    ; load GDT register with start address of Global Descriptor Table
    mov eax, cr0
    or  eax, 0x1       ; set PE (Protection Enable) bit in CR0 (Control Register 0)
    mov cr0, eax
    ; Perform far jump to selector 08h (offset into GDT, pointing at a 32bit PM code segment descriptor)
    ; to load CS with proper PM32 descriptor)
    jmp CODE32_SEG:protect_cseg  ;0x08:protect_cseg

not_support_long_mode:
    mov si, long_mode_not_support
    jmp end16

not_support:
    mov si, not_support_message

end16:
    call print
.spin16:
    hlt
    jmp .spin16

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
gdt32_start:

; offset 0x0 (0 bytes)
.gdt32_null:
    dd 0x0
    dd 0x0

; offset 0x8 (8 bytes)
gdt32_code:	; cs should point to this descriptor
	dw 0xffff		; segment limit first 0-15 bits
	dw 0x0			; base first 0-15 bits
	db 0x0			; base 16-23 bits
	db 0x9a			; access byte
	db 0b11001111	; high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x0			; base 24-31 bits

; offset 0x10 (16 bytes)
gdt32_data:	; ds, es, fs, gs, and ss should point to this descriptor
	dw 0xffff		; segment limit first 0-15 bits
	dw 0x0			; base first 0-15 bits
	db 0x0			; base 16-23 bits
	db 0x92			; access byte
	db 0b11001111	; high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x0			; base 24-31 bits

gdt32_end:

gdt32_descriptor:
    dw gdt32_end - gdt32_start - 1
    dd gdt32_start

[BITS 32]
protect_cseg:
    mov bx, 0x10
    mov ds, bx ; set data segment
    mov es, bx ; set extra segment
    mov ss, bx ; set stack segment
    mov esp, STACK_P
    mov ebp, esp

enable_a20_line: ; Enable A20 Line
    in al, 0x92
    or al, 2
    out 0x92, al

    call load_sectors

    jmp CODE32_SEG:LOADER_ADDR

load_sectors:
    mov eax, 1  ; read from NO.1 sector
    mov ecx, 4  ; read 4 sectors
    mov edi, LOADER_ADDR
    call ata_lba_read ;https://wiki.osdev.org/ATA_read/write_sectors
    ret

ata_lba_read:
    mov ebx, eax, ; Backup the LBA
    ; Send the highest 8 bits of the lba to hard disk controller
    shr eax, 24
    or eax, 0xE0 ; Select the  master drive
    mov dx, 0x1F6
    out dx, al
    ; Finished sending the highest 8 bits of the lba

    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al
    ; Finished sending the total sectors to read

    ; Send more bits of the LBA
    mov eax, ebx ; Restore the backup LBA
    mov dx, 0x1F3
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send more bits of the LBA
    mov dx, 0x1F4
    mov eax, ebx ; Restore the backup LBA
    shr eax, 8
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send upper 16 bits of the LBA
    mov dx, 0x1F5
    mov eax, ebx ; Restore the backup LBA
    shr eax, 16
    out dx, al
    ; Finished sending upper 16 bits of the LBA

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

    ; Read all sectors into memory
.next_sector:
    push ecx

; Checking if we need to read
.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

; We need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1F0
    rep insw
    pop ecx
    loop .next_sector
    ; End of reading sectors into memory
    ret

times 510-($ - $$) db 0
dw 0xAA55
