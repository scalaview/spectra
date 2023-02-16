[BITS 64]

section .text

global sys_print
sys_print:
    xor rax, rax
    push rdx
    push rsi
    push rdi

    mov rdi, 0  ; command
    mov rsi, 3  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 24
    ret

global sys_sleep
sys_sleep:
    xor rax, rax
    push rdi

    mov rdi, 1  ; command
    mov rsi, 1  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 8
    ret

global exit
exit:
    xor rax, rax
    push rdi

    mov rdi, 2  ; command
    mov rsi, 1  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 8
    ret

global sys_wait
sys_wait:
    xor rax, rax
    push rdi

    mov rdi, 3  ; command
    mov rsi, 1  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 8
    ret

global fork
fork:
    xor rax, rax

    mov rdi, 4  ; command
    mov rsi, 0  ; argc
    mov rdx, 0  ; argv
    int 0x80

    ret

; int sys_execve(const char* pathname, const char* argv, const char* envp, int ring_lev);
global sys_execve
sys_execve:
    xor rax, rax
    push rcx
    push rdx
    push rsi
    push rdi

    mov rdi, 5  ; command
    mov rsi, 4  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 32
    ret

global malloc
malloc:
    xor rax, rax
    push rdi

    mov rdi, 6  ; command
    mov rsi, 1  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 8
    ret

global free
free:
    xor rax, rax
    push rdi

    mov rdi, 7  ; command
    mov rsi, 1  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 8
    ret

global keyboard_getkey
keyboard_getkey:
    xor rax, rax

    mov rdi, 8  ; command
    mov rsi, 0  ; argc
    mov rdx, 0  ; argv
    int 0x80

    ret

; extern struct gui_window* create_window_content(int32_t x, int32_t y, int32_t z, uint32_t width, uint32_t height, uint32_t gcolor, uint16_t attributes);

global create_window_content
create_window_content:
    xor rax, rax
    mov rax, QWORD[rsp + 8]
    push rax ; attributes
    push r9  ; gcolor
    push r8  ; height
    push rcx ; width
    push rdx ; z
    push rsi ; y
    push rdi ; x

    mov rdi, 9  ; command
    mov rsi, 7  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 56
    ret

global sys_window_get_message
sys_window_get_message:
    xor rax, rax
    push rsi
    push rdi

    mov rdi, 10  ; command
    mov rsi, 2  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 16
    ret

global sys_window_free
sys_window_free:
    xor rax, rax
    push rdi

    mov rdi, 11  ; command
    mov rsi, 1  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 8
    ret

global fopen
fopen:
    xor rax, rax
    push rdx
    push rsi
    push rdi

    mov rdi, 12  ; command
    mov rsi, 3  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 24
    ret

global fread
fread:
    xor rax, rax
    push rcx
    push rdx
    push rsi
    push rdi

    mov rdi, 14  ; command
    mov rsi, 4  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 32
    ret

global fstat
fstat:
    xor rax, rax
    push rdx
    push rsi
    push rdi

    mov rdi, 13  ; command
    mov rsi, 3  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 24
    ret

global fseek
fseek:
    xor rax, rax
    push rdx
    push rsi
    push rdi

    mov rdi, 16  ; command
    mov rsi, 3  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 24
    ret

global fclose
fclose:
    xor rax, rax
    push rdi

    mov rdi, 15  ; command
    mov rsi, 1  ; argc
    mov rdx, rsp ; argv
    int 0x80

    add rsp, 8
    ret
