section .multiboot_header
%define VGA_WIDTH                           1024
%define VGA_HEIGHT                          768
%define VGA_BITS                            32
%define MULTIBOOT2_HEADER_MAGIC             0xe85250d6
%define MULTIBOOT_HEADER_TAG_FRAMEBUFFER    5
%define MULTIBOOT_HEADER_TAG_OPTIONAL       1
%define MULTIBOOT_HEADER_TAG_END            0
%define GRUB_MULTIBOOT_ARCHITECTURE_I386    0

multiboot_header:
    dd   MULTIBOOT2_HEADER_MAGIC
    dd   GRUB_MULTIBOOT_ARCHITECTURE_I386
    dd   multiboot_header_end - multiboot_header
    dd   -(MULTIBOOT2_HEADER_MAGIC + GRUB_MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header))
framebuffer_tag_start:
    dw MULTIBOOT_HEADER_TAG_FRAMEBUFFER
    dw MULTIBOOT_HEADER_TAG_OPTIONAL
    dd framebuffer_tag_end - framebuffer_tag_start
    dd VGA_WIDTH
    dd VGA_HEIGHT
    dd VGA_BITS
framebuffer_tag_end:
    dw MULTIBOOT_HEADER_TAG_END
    dw 0
    dd 0
multiboot_header_end: