#include "paging/paging.h"
#include "heap/kheap.h"
#include "kmemory.h"
#include "config.h"

struct pml4_table* kernel_chunk;

struct pml4_table* paging_initialize(uint8_t flags)
{
    uint64_t base_address = (KERNEL_VM_BASE << 12) >> 12;
    uint64_t max_address = (KERNEL_VM_MAX << 12) >> 12;
    uint64_t offset = (0x0 << 12) >> 12;
    pml4_entry* pml4_entries = kmalloc(sizeof(pml4_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE);

    memset(pml4_entries, 0, sizeof(pml4_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE);

    uint32_t lev_4_start_index = (uint32_t)(base_address >> 39) & 0x1ff;
    uint32_t lev_4_end_index = (uint32_t)(max_address >> 39) & 0x1ff;
    for (int i = lev_4_start_index; i <= lev_4_end_index; i++)
    {
        uint64_t current_lev_4_address = i * (((uint64_t)2) << 38);
        if (current_lev_4_address < base_address)
            continue;
        if (current_lev_4_address >= max_address)
            break;
        pdp_entry* pdp_entries = kmalloc(sizeof(pdp_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE);
        memset(pdp_entries, 0, sizeof(pdp_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE);
        for (int j = 0;j < PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE; j++)
        {
            uint64_t current_lev_3_address = current_lev_4_address + j * (((uint64_t)2) << 29);
            if (current_lev_3_address < base_address)
                continue;
            if (current_lev_3_address >= max_address)
                break;
            pdp_entry* pd_entries = kmalloc(sizeof(pdp_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE);
            memset(pd_entries, 0, sizeof(pdp_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE);
            for (int z = 0; z < PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE; z++)
            {
                pd_entries[z].entry = (offset + (i - lev_4_start_index) * (PAGE_SIZE * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE) + j * (PAGE_SIZE * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE) + z * PAGE_SIZE) | (0b10000000 | flags);
            }
            pdp_entries[j].entry = vir2phy(&pd_entries) | flags;
        }
        pml4_entries[i].entry = vir2phy(&pdp_entries) | flags;
    }
    struct pml4_table* pml4_table = kmalloc(sizeof(struct pml4_table));
    pml4_table->entries = pml4_entries;
    return pml4_table;
}


