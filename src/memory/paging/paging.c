#include "paging/paging.h"
#include "heap/kheap.h"
#include "kmemory.h"
#include "assert.h"
#include "config.h"

struct pml4_table* kernel_chunk;

// 2MB paging struct initialize
struct pml4_table* paging_initialize(uint64_t vir_base_addr, uint64_t vir_max_addr, uint64_t phy_addr, uint8_t flags)
{
    assert(phy_addr % PAGE_SIZE == 0);
    // 64-bit processors support 48-bit virtual addressing and 256-TiB virtual address spaces.
    uint64_t base_address = (vir_base_addr << 16) >> 16;
    uint64_t max_address = (vir_max_addr << 16) >> 16;
    uint64_t offset = (phy_addr << 16) >> 16;

    pml4_entry* pml4_entries = kmalloc(sizeof(pml4_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE);
    memset(pml4_entries, 0, sizeof(pml4_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE);

    uint32_t lev_4_start_index = (uint32_t)(base_address >> 39);
    uint32_t lev_4_end_index = (uint32_t)(max_address >> 39);
    for (int i = lev_4_start_index; i <= lev_4_end_index; i++)
    {
        uint64_t current_lev_4_address = i * (((uint64_t)2) << 38);
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
                pd_entries[z].entry = (offset + (i - lev_4_start_index) * (PAGE_SIZE * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE) + j * (PAGE_SIZE * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE) + z * PAGE_SIZE) | (0b10000000 | flags); //2MB paging, Set Huge 1
            }
            pdp_entries[j].entry = vir2phy(pd_entries) | flags;
        }
        pml4_entries[i].entry = vir2phy(pdp_entries) | flags;
    }
    struct pml4_table* pml4_table = kmalloc(sizeof(struct pml4_table));
    pml4_table->entries = pml4_entries;
    return pml4_table;
}

struct pml4_table* kernel_paging_initialize()
{
    return paging_initialize(KERNEL_VM_BASE, KERNEL_VM_MAX, KERNEL_PHY_BASE, PAGING_IS_WRITEABLE | PAGING_PRESENT);
}

void free_paging_directory(pdp_entry* dir)
{
    for (int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE; i++)
    {
        pdp_entry entry = dir[i];
        if (entry.fields.address)
        {
            pdp_entry* e = (pdp_entry*)phy2vir(entry.fields.address << 12);
            if (e->fields.hug)
            {
                kfree(e);
                continue;
            }
            free_paging_directory(e);
        }
    }
    kfree(dir);

}

void free_paging(struct pml4_table* pml4_table)
{
    free_paging_directory((pdp_entry*)pml4_table->entries);
    kfree(pml4_table);
}