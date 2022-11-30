#include "paging/paging.h"
#include "heap/kheap.h"
#include "kmemory.h"
#include "assert.h"
#include "status.h"
#include "config.h"
#include "drivers/vga/vesa.h"

struct pml4_table* kernel_chunk;

int paging_initialize_pdp_entry(pdp_entry* pd_entries, uint64_t base_address, uint64_t max_address, uint64_t offset, uint32_t* offset_count_ptr, uint64_t prev_lev_start_address, uint32_t page_size, uint64_t current_level_page_size, uint8_t flags)
{
    int res = 0;
    if (page_size != PAGE_SIZE_2M && page_size != PAGE_SIZE_4K)
    {
        res = -EINVARG;
        goto out;
    }
    if (base_address % page_size != 0 || max_address % page_size != 0 || offset % page_size != 0)
    {
        res = -EINVARG;
        goto out;
    }
    int max_index = ((max_address - prev_lev_start_address) % current_level_page_size) != 0 ? ((max_address - prev_lev_start_address + current_level_page_size) / current_level_page_size) : ((max_address - prev_lev_start_address) / current_level_page_size);
    int start_index = base_address > prev_lev_start_address ? ((base_address - prev_lev_start_address) / current_level_page_size) : 0;
    for (int i = start_index; i < max_index && i < PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE; i++)
    {
        if (page_size == current_level_page_size)
        {
            pd_entries[i].entry = (offset + (*offset_count_ptr) * page_size) | (((page_size == PAGE_SIZE_1G || page_size == PAGE_SIZE_2M) ? 0b10000000 : 0) | flags); //1GB or 2MB paging, Set Huge 1
            (*offset_count_ptr)++;
        }
        else
        {
            pdp_entry* next_entries;
            if (!pd_entries[i].fields.address)
            {
                next_entries = kzalloc(sizeof(pdp_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE);
                pd_entries[i].entry = vir2phy(next_entries) | flags;
            }
            else
            {
                next_entries = (pdp_entry*)phy2vir(pd_entries[i].fields.address << 12);
            }
            uint64_t current_start_address = prev_lev_start_address + (uint64_t)i * current_level_page_size;
            res = paging_initialize_pdp_entry(next_entries, base_address, max_address, offset, offset_count_ptr, current_start_address, page_size, current_level_page_size / PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE, flags);
            if (res < 0)
            {
                if (!pd_entries[i].fields.address)
                {
                    pd_entries[i].entry = 0;
                    kfree(next_entries);
                }
                goto out;
            }
        }
    }
out:
    return res;
}

int paging_initialize_pml4_table(struct pml4_table** pml4_table, uint64_t vir_base_addr, uint64_t vir_max_addr, uint64_t phy_addr, uint32_t page_size, uint8_t flags)
{
    int res = 0;
    if (page_size != PAGE_SIZE_2M && page_size != PAGE_SIZE_4K)
    {
        res = -EINVARG;
        goto out;
    }
    if (phy_addr % page_size != 0 || vir_base_addr % page_size != 0 || vir_max_addr % page_size != 0)
    {
        res = -EINVARG;
        goto out;
    }
    if (!*pml4_table)
    {
        *pml4_table = kzalloc(sizeof(struct pml4_table));
        if (!*pml4_table)
        {
            res = -ENOMEM;
            goto out;
        }
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
    res = paging_initialize_pdp_entry((pdp_entry*)pml4_entries, base_address, max_address, offset, &offset_count, 0, page_size, (uint64_t)PAGE_SIZE_1G * PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE, flags);
    if (res < 0)
    {
        if (!(*pml4_table)->entries)
        {
            (*pml4_table)->entries = 0;
            kfree(pml4_entries);
        }
    }
out:
    return res;
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