#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_PHY_BASE         0x0
#define KERNEL_VMA              0xffff800000000000
#define KERNEL_VM_MAX           0xffff800040000000
#define SECTOR_SIZE             512
#define TOTAL_INTERRUPTS        512
#define KERNEL_CODE_SELECTOR    0x08

#define OS_SECTOR_SIZE          512

#define OS_MAX_FILESYSTEMS      16
#define OS_MAX_FILE_DESCRIPTORS 512
#endif
