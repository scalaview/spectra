#ifndef CONFIG_H
#define CONFIG_H

#include "paging/paging.h"

#define KERNEL_PHY_BASE         0x0
#define KERNEL_VMA              0xffff800000000000
#define KERNEL_VM_MAX           0xffff800040000000
#define SECTOR_SIZE             512
#define TOTAL_INTERRUPTS        512
#define KERNEL_CODE_SELECTOR    0x08

#define IO_SECTOR_SIZE          512
#define DISK_BLOCK_SIZE         2048
#define MBR_END_POSITION        (DISK_BLOCK_SIZE * 512) //1MB

#define OS_MAX_FILESYSTEMS      16
#define OS_MAX_FILE_DESCRIPTORS 512

#define RANG_3_VMA              0x400000
#define RANG_3_VM_MAX           (RANG_3_VMA + (2 * PAGE_SIZE_2M))
#define RANG_3_STACK_SIZE       PAGE_SIZE_4K * 4
#define RANG_3_STACK_PTR        RANG_3_VMA
#define RANG_3_STACK_IP         (RANG_3_STACK_PTR - RANG_3_STACK_SIZE)
#define USER_DATA_SEGMENT       0x1b
#define USER_CODE_SEGMENT       0x13
#endif
