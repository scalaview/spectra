#include "pmm.h"
#include "heap/bitmap_heap.h"
#include "kmemory.h"
#include "config.h"
#include "bitmap_pmm.h"
#include "assert.h"

extern void* kernel_end;
const struct pmm_manager bitmap_pmm_manager;
struct heap_bitmap_table kernel_bitmap_heap_table;
struct bitmap_heap kernel_bitmap_heap;

static void init_bitmap_pmm()
{
    init_memory_map();
}

static void init_bitmap_kheap()
{
    struct mem_map* mem_map = get_memory_map();
    uint32_t block_size = mem_map->block_size;
    uint64_t start_addr = align_up(&kernel_end), end_addr = align_up(&kernel_end);
    struct e820map* memory_map = mem_map->map;
    for (; memory_map < mem_map->map + block_size; memory_map++)
    {
        uint64_t begin = memory_map->base_address;
        uint64_t end = begin + memory_map->length;
        if (memory_map->type == 1 && phy2vir(end) > start_addr)
        {
            end_addr = phy2vir(align_down(end));
            break;
        }
    }
    assert((end_addr - start_addr) > OS_BITMAP_BLOCK_SIZE);

    kernel_bitmap_heap_table.entries = (BITMAP_HEAP_TABLE_ENTRY*)KERNEL_BITMAP_TABLE_ADDRESS;
    kernel_bitmap_heap_table.total = (end_addr - start_addr) / OS_BITMAP_BLOCK_SIZE;

    int res = init_bitmap_heap(&kernel_bitmap_heap, (void*)start_addr, (void*)end_addr, &kernel_bitmap_heap_table);
    assert(res >= 0);
}

static void* alloc(size_t size)
{
    return bitmap_heap_malloc(&kernel_bitmap_heap, size);
}

static void free(void* ptr)
{
    bitmap_heap_free(&kernel_bitmap_heap, ptr);
}

const struct pmm_manager bitmap_pmm_manager = {
    .name = "bitmap_pmm_manager",
    .init = init_bitmap_pmm,
    .init_kheap = init_bitmap_kheap,
    .alloc = alloc,
    .free = free
};