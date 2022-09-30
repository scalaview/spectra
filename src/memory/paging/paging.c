#include "paging/paging.h"
#include "heap/kheap.h"
#include "kmemory.h"
#include "assert.h"
#include "status.h"
#include "config.h"


struct pml4_table* kernel_chunk;

int paging_initialize_pml4_table(struct pml4_table** pml4_table, uint64_t vir_base_addr, uint64_t vir_max_addr, uint64_t phy_addr, uint32_t page_size, uint8_t flags)
{
    if (page_size != PAGE_SIZE_2M && page_size != PAGE_SIZE_4K)
    {
        return -EIO;
    }
    if (phy_addr % page_size != 0 || vir_base_addr % page_size != 0 || vir_max_addr % page_size != 0)
    {
        return -EINVARG;
    }
    if (!*pml4_table)
    {
        *pml4_table = kzalloc(sizeof(struct pml4_table));
    }

    // 64-bit processors support 48-bit virtual addressing and 256-TiB virtual address spaces.
    uint64_t base_address = (vir_base_addr << 16) >> 16;
    uint64_t max_address = (vir_max_addr << 16) >> 16;
    uint64_t offset = (phy_addr << 16) >> 16;
    uint32_t offset_count = 0;

    pml4_entry* pml4_entries;
    if ((*pml4_table)->entries)
    {
        pml4_entries = (*pml4_table)->entries;
    }
    else
    {
        pml4_entries = kzalloc(sizeof(pml4_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE);
        (*pml4_table)->entries = pml4_entries;
    }
    uint32_t lev_4_start_index = (uint32_t)(base_address >> 39);
    uint32_t lev_4_end_index = (uint32_t)(max_address >> 39);
    for (int i = lev_4_start_index; i <= lev_4_end_index; i++)
    {
        uint64_t current_lev_4_address = i * (((uint64_t)2) << 38);
        pdp_entry* pdp3_entries;
        if (!pml4_entries[i].fields.address)
        {
            pdp3_entries = kzalloc(sizeof(pdp_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE);
            pml4_entries[i].entry = vir2phy(pdp3_entries) | flags;
        }
        else
        {
            pdp3_entries = (pdp_entry*)phy2vir(pml4_entries[i].fields.address << 12);
        }
        for (int j = 0;j < PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE; j++)
        {
            uint64_t current_lev_3_start_address = current_lev_4_address + j * (((uint64_t)2) << 29);
            uint64_t current_lev_3_end_address = current_lev_3_start_address + PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE * PAGE_SIZE_2M;
            if (current_lev_3_end_address < base_address)
                continue;
            if (current_lev_3_start_address >= max_address)
                break;
            pdp_entry* pd2_entries;
            if (!pdp3_entries[j].fields.address)
            {
                pd2_entries = kzalloc(sizeof(pdp_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE);
                pdp3_entries[j].entry = vir2phy(pd2_entries) | flags;
            }
            else
            {
                pd2_entries = (pdp_entry*)phy2vir(pdp3_entries[j].fields.address << 12);
            }
            for (int z = 0; z < PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE; z++)
            {
                if (page_size == PAGE_SIZE_2M)
                {
                    uint64_t current_lev_2_address = current_lev_3_start_address + z * page_size;
                    if (current_lev_2_address >= base_address && current_lev_2_address < max_address)
                    {
                        pd2_entries[z].entry = (offset + offset_count * page_size) | (0b10000000 | flags); //2MB paging, Set Huge 1
                        offset_count++;
                    }
                }
                else if (page_size == PAGE_SIZE_4K)
                {
                    uint64_t current_lev_2_start_address = current_lev_3_start_address + z * (((uint64_t)2) << 20);
                    uint64_t current_lev_2_end_address = current_lev_2_start_address + PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE * PAGE_SIZE_4K;
                    if (current_lev_2_end_address < base_address)
                        continue;
                    if (current_lev_2_start_address >= max_address)
                        break;
                    pdp_entry* pd1_entries;
                    if (!pd2_entries[z].fields.address)
                    {
                        pd1_entries = kzalloc(sizeof(pdp_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE);
                        pd2_entries[z].entry = vir2phy(pd1_entries) | flags;
                    }
                    else
                    {
                        pd1_entries = (pdp_entry*)phy2vir(pd2_entries[z].fields.address << 12);
                    }
                    for (int t = 0; t < PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE; t++)
                    {
                        uint64_t current_lev_1_address = current_lev_2_start_address + t * page_size;
                        if (current_lev_1_address >= base_address && current_lev_1_address < max_address)
                        {
                            pd1_entries[t].entry = (offset + offset_count * page_size) | flags;
                            offset_count++;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

// 2MB/4K paging struct initialize
struct pml4_table* paging_initialize(uint64_t vir_base_addr, uint64_t vir_max_addr, uint64_t phy_addr, uint32_t page_size, uint8_t flags)
{
    int res = 0;
    struct pml4_table* pml4_table = kzalloc(sizeof(struct pml4_table));
    if (!pml4_table)
    {
        res = -ENOMEM;
        goto out;
    }
    res = paging_initialize_pml4_table(&pml4_table, vir_base_addr, vir_max_addr, phy_addr, page_size, flags);
out:
    if (res)
    {
        kfree(pml4_table);
        return 0;
    }
    return pml4_table;
}

struct pml4_table* kernel_paging_initialize()
{
    return paging_initialize(KERNEL_VMA, KERNEL_VM_MAX, KERNEL_PHY_BASE, PAGE_SIZE_2M, PAGING_IS_WRITEABLE | PAGING_PRESENT);
}

void free_paging_directory(pdp_entry* dir, uint16_t level)
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
            if (level == 2)
            {
                kfree(e);
                continue;
            }
            free_paging_directory(e, level - 1);
        }
    }
    kfree(dir);
}

void free_paging(struct pml4_table* pml4_table)
{
    free_paging_directory((pdp_entry*)pml4_table->entries, 4);
    kfree(pml4_table);
}