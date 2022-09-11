#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_PHY_BASE         0x0
#define KERNEL_VMA              0xffff800000000000
#define KERNEL_VM_MAX           0xffff800040000000
#define SECTOR_SIZE             512
#define TOTAL_INTERRUPTS        512
#define KERNEL_CODE_SELECTOR    0x08
#define MBIMEMORY_INFO_PHYA     0x9000
#define MEMORY_INFO_ADDR        (KERNEL_VMA + MBIMEMORY_INFO_PHYA)

#endif
