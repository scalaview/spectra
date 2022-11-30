section .text

global load_paging_directory
load_paging_directory:
    push rbp
    mov rbp, rsp

    mov rax, cr3

    pop rbp
    ret

global setup_paging_directory
setup_paging_directory:
    push rbp
    mov rbp, rsp

    mov rax, rdi
    mov cr3, rax

    pop rbp
    ret

global read_cr2
read_cr2:
    mov rax, cr2
    ret

