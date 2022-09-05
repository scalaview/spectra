#ifndef BITMAP_PMM_H
#define BITMAP_PMM_H
#include "config.h"

#define KERNEL_BITMAP_TABLE_ADDRESS (KERNEL_VM_BASE + 0x11000)

extern const struct pmm_manager bitmap_pmm_manager;
#endif