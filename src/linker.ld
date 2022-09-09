ENTRY(start)
OUTPUT_FORMAT(elf64-x86-64)
KERNEL_LMA = 0x100000;
KERNEL_VMA = 0xFFFF800000200000;

SECTIONS
{
    . = KERNEL_LMA;
    .boot : {
      *(.multiboot_header)
    }
    . += KERNEL_VMA;

    .text : AT(ADDR(.text) - KERNEL_VMA)
    {
        *(EXCLUDE_FILE(multiboot_header) .text)
        *(.text)
    }

    .asm ALIGN (0x1000) : AT(ADDR(.asm) - KERNEL_VMA)
    {
        *(.asm)
    }

    .rodata ALIGN (0x1000) : AT(ADDR(.rodata) - KERNEL_VMA)
    {
        *(.rodata)
    }

    .data  ALIGN (0x1000) : AT(ADDR(.data) - KERNEL_VMA)
    {
        *(.data)
    }

    .bss ALIGN (0x1000) : AT(ADDR(.bss) - KERNEL_VMA)
    {
        *(COMMON)
        *(.bss)
    }

    PROVIDE(kernel_end = .);

}