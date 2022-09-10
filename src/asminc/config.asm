
%define DATA_SEG            0x10
; 2MB
%define PAGE_SIZE           0x20000
%define KERNEL_VMA          0xFFFF800000000000
%define STACK_P             0x70000
%define STACK_V             (KERNEL_VMA + 0x70000)
%define WRITABLE_PRESENT    0b0011
%define p4_table            0x70000
%define p3_table            0x71000
%define p2_table            0x72000
%define MB_MAGICA           0x9000
%define MBI_PHYA            0x9008