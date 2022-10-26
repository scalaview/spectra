section .text

global memcpy

; void memcpy(void* dest, void* src, size_t size)
memcpy:
    mov rax, rdi ; dest
    mov rcx, rdx ; size
    shr rcx, 3
    and edx, 7
    rep movsq
    mov ecx, edx
    rep movsb
    ret
