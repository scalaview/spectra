OUTPUT_FORMAT("elf64-x86-64")
ENTRY(startup)

SECTIONS
{
    . = 0x400000;

    .text : ALIGN(0x1000)
    {
        *(.text)
    }

    .rodata : ALIGN(0x1000)
    {
        *(.rodata)
    }

    .data : ALIGN(0x1000)
    {
        *(.data)
    }

    .bss : ALIGN(0x1000)
    {
        *(.bss)
    }
}
