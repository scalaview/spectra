#ifndef CONFIG_H
#define CONFIG_H

#include "paging/paging.h"

#define KERNEL_PHY_BASE         0x0
#define KERNEL_VMA              0xffff800000000000
#define KERNEL_VM_MAX           0xffff800040000000
#define KERNEL_VGA_BASE         KERNEL_VM_MAX
#define SECTOR_SIZE             512
#define TOTAL_INTERRUPTS        512
#define KERNEL_CODE_SELECTOR    0x08

#define IO_SECTOR_SIZE          512
#define DISK_BLOCK_SIZE         2048
#define MBR_END_POSITION        (DISK_BLOCK_SIZE * 512) //1MB

#define OS_MAX_FILESYSTEMS      16
#define OS_MAX_FILE_DESCRIPTORS 512

#define RING_0_VMA              0xffff800060000000
#define RING_0_VM_MAX           (RING_0_VMA + (2 * PAGE_SIZE_2M))
#define KERNEL_DATA_SEGMENT     0x10
#define KERNEL_CODE_SEGMENT     0x08

#define RING_3_VMA              0x400000
#define RING_3_VM_MAX           (RING_3_VMA + (2 * PAGE_SIZE_2M))
#define RING_3_STACK_SIZE       PAGE_SIZE_4K * 4
#define RING_3_STACK_PTR        RING_3_VMA
#define USER_DATA_SEGMENT       0x20
#define USER_CODE_SEGMENT       0x18


#define OS_MAX_ISR80H_COMMANDS  1024
#define PROCESS_ALLOCATIONS     512
#define OS_KEYBOARD_BUFFER_SIZE 1024


#define PROCESS_WAIT_FOR_KEYBOARD   -2

#define OS_MAX_MESSAGE_LENGTH    512
#define OS_MAX_WINDOW_LENGTH     64
#endif
