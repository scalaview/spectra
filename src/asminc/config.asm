
%define DATA_SEG            0x10
; 2MB
%define PAGE_SIZE           0x20000
%define KERNEL_VMA          0xFFFF800000000000
%define STACK_P             0x70000
%define STACK_V             (KERNEL_VMA + 0x70000)
%define WRITABLE_PRESENT    0b0011
%define P4_TABLE_PHY        (p4_table - KERNEL_VMA)
%define P3_TABLE_PHY        (p3_table - KERNEL_VMA)
%define P2_TABLE_PHY        (p2_table - KERNEL_VMA)
%define MB_MAGICA           0x9000
%define MBI_PHYA            0x9008