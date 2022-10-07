OUTPUT_FORMAT("elf64-x86-64")
ENTRY(startup)

SECTIONS
{
    . = 0xffff800040000000;

    .text : ALIGN(16)
    {
        *(.text)
    }

    .rodata : ALIGN(16)
    {
        *(.rodata)
    }

    .data : ALIGN(16)
    {
        *(.data)
    }

    .bss : ALIGN(16)
    {
        *(.bss)
    }
}
