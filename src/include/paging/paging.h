#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include "heap/bitmap_heap.h"

#define ALIGN_SIZE  4096
#define PAGING_TOTAL_ENTRIES_PER_TABLE_SIZE 512
#define PAGE_SIZE_2M               0x200000    //2MB
#define PAGE_SIZE_4K               4096
#define PAGING_ACCESS_FROM_ALL      0b00000100
#define PAGING_IS_WRITEABLE         0b00000010
#define PAGING_PRESENT              0b00000001

// https://wiki.osdev.org/Paging
typedef union pml4_struct {
    uint64_t entry;
    struct {
        uint64_t present : 1;   // Page present
        uint64_t writable : 1;   // Readavle/Writeable
        uint64_t user : 1;   // User accessable
        uint64_t write_through : 1;
        uint64_t disable_cache : 1;   // Disable cache
        uint64_t accessed : 1;
        uint64_t ignore_1 : 1;
        uint64_t zero_1 : 1;   // must be zero
        uint64_t zero_2 : 1;   // must be zero
        uint64_t avail_1 : 3;
        uint64_t address : 40;  // Address
        uint64_t avail_2 : 11;
        uint64_t execute_disable : 1;
    } fields;
} __attribute__((packed)) pml4_entry;


typedef union pdp_struct {
    uint64_t entry;
    struct {
        uint64_t present : 1;   // Page present
        uint64_t writable : 1;   // Readavle/Writeable
        uint64_t user : 1;   // User accessable
        uint64_t write_through : 1;
        uint64_t disable_cache : 1;   // Disable cache
        uint64_t accessed : 1;
        uint64_t ignore_1 : 1;
        uint64_t hug : 1;   // Huge page/null
        uint64_t ignore_2 : 1;
        uint64_t avail_1 : 3;
        uint64_t address : 40;  // Address
        uint64_t avail_2 : 11;
        uint64_t execute_disable : 1;
    } fields;
} __attribute__((packed)) pdp_entry;

struct pml4_table
{
    pml4_entry* entries;
} __attribute__((packed, aligned(ALIGN_SIZE)));

struct pd_table
{
    pdp_entry* entries;
} __attribute__((packed, aligned(ALIGN_SIZE)));

extern void* load_paging_directory();
extern void* setup_paging_directory(uint64_t pml4_table_phy_addr);
void paging_initialize_pml4_table(struct pml4_table** pml4_table, uint64_t vir_base_addr, uint64_t vir_max_addr, uint64_t phy_addr, uint32_t page_size, uint8_t flags);
struct pml4_table* paging_initialize(uint64_t vir_base_addr, uint64_t vir_max_addr, uint64_t phy_addr, uint32_t page_size, uint8_t flags);
struct pml4_table* kernel_paging_initialize();
void free_paging(struct pml4_table* pml4_table);

#endif