ENTRY(start)
OUTPUT_FORMAT(elf64-x86-64)
SECTIONS
{
    . = 0x7c00;
    .text :
    {
        *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
    }
}