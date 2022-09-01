ENTRY(kernel_main)
OUTPUT_FORMAT(elf64-x86-64)

KERNEL_VM_BASE = 0xFFFF00000C200000;

SECTIONS
{
    . = KERNEL_VM_BASE;

    .text : ALIGN(4096)
    {
        *(.text)
    }


    .asm : ALIGN(4096)
    {
        *(.asm)
    }

    .rodata : ALIGN(4096)
    {
        *(.rodata)
    }

    .data : ALIGN(4096)
    {
        *(.data)
    }

    .bss : ALIGN(4096)
    {
        *(COMMON)
        *(.bss)
    }
    PROVIDE(kernel_end = .);

}