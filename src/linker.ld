ENTRY(start)
OUTPUT_FORMAT(elf64-x86-64)

KERNEL_VMA = 0xFFFF800000200000;

SECTIONS
{
    . = KERNEL_VMA;

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