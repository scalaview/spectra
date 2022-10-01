section .data

global multiboot_magic
multiboot_magic:
    dd  0

global multiboot_info
multiboot_info:
    dd 0

mbimemory_info_mem:
    times 64    dq  0
global mbimemory_info
mbimemory_info:
    dq mbimemory_info_mem