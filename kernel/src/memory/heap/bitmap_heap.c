#include "heap/bitmap_heap.h"
#include "kmemory.h"
#include "status.h"
#include "assert.h"

#include <stdbool.h>

bool validate_aligment(void* ptr)
{
    return ((uint64_t)(ptr) % OS_BITMAP_BLOCK_SIZE) == 0;
}

static int address_to_start_block(struct bitmap_heap* heap, void* address)
{
    return ((int)(address - heap->start_addr)) / OS_BITMAP_BLOCK_SIZE;
}

bool validate_heap_table(void* start, void* end, struct heap_bitmap_table* table)
{
    size_t size = (size_t)(end - start);
    if (table->total != size / OS_BITMAP_BLOCK_SIZE)
    {
        return false;
    }
    return true;
}

int init_bitmap_heap(struct bitmap_heap* heap, void* start, void* end, struct heap_bitmap_table* heap_table)
{
    int res = 0;

    if (!validate_aligment(start) || !validate_aligment(end))
    {
        res = -EINVARG;
        goto out;
    }

    if (!validate_heap_table(start, end, heap_table))
    {
        res = -EINVARG;
        goto out;
    }

    memset(heap, 0, sizeof(heap));
    heap->start_addr = start;
    heap->table = heap_table;

    size_t table_size = sizeof(OS_BITMAP_BLOCK_SIZE) * heap_table->total;
    memset(heap_table->entries, 0, table_size);

out:
    return res;
}

int get_start_block(struct heap_bitmap_table* table, size_t total)
{
    int start_block = -1;
    int block_count = 0;
    for (size_t i = 0; i < table->total; i++)
    {
        if (entry_taken(table->entries[i]))
        {
            if (block_count > 0)
            {
                start_block = -1;
                block_count = 0;
            }
            continue;
        }
        if (start_block == -1)
            start_block = i;
        block_count++;
        if (block_count == total)
            break;
    }
    if (start_block == -1 || block_count != total)
    {
        return -ENOMEM;
    }
    return start_block;
}

static void* block_to_address(struct bitmap_heap* heap, int start_block)
{
    return heap->start_addr + (start_block * OS_BITMAP_BLOCK_SIZE);
}

static void mark_bitmap_blocks_taken(struct heap_bitmap_table* table, int start_block, int total)
{
    int end_block = start_block + total - 1;
    BITMAP_HEAP_TABLE_ENTRY entry = BITMAP_HEAP_TABLE_ENTRY_TAKEN | BITMAP_HEAP_IS_FIRST;
    if (total > 1)
    {
        entry |= BITMAP_HEAP_HAS_NEXT;
    }

    for (int i = start_block; i <= end_block; i++)
    {
        table->entries[i] = entry;
        entry = (i != end_block - 1) ? (BITMAP_HEAP_TABLE_ENTRY_TAKEN | BITMAP_HEAP_HAS_NEXT) : BITMAP_HEAP_TABLE_ENTRY_TAKEN;
    }
}

static void mark_bitmap_blocks_free(struct heap_bitmap_table* table, int start_block)
{
    for (size_t i = start_block; i < table->total;i++)
    {
        BITMAP_HEAP_TABLE_ENTRY entry = table->entries[i];
        assert(entry_taken(entry));
        table->entries[i] = BITMAP_HEAP_TABLE_ENTRY_FREE;
        if (!entry_has_next(entry))
        {
            break;
        }
    }
}

void* bitmap_heap_malloc_blocks(struct bitmap_heap* heap, size_t total)
{
    void* address = 0;
    int start_block = get_start_block(heap->table, total);
    if (start_block < 0)
    {
        goto out;
    }

    address = block_to_address(heap, start_block);
    mark_bitmap_blocks_taken(heap->table, start_block, total);
out:
    return address;
}

void* bitmap_heap_malloc(struct bitmap_heap* heap, size_t size)
{
    size_t aligned_size = align_up(size);
    size_t total_blocks = aligned_size / OS_BITMAP_BLOCK_SIZE;
    return bitmap_heap_malloc_blocks(heap, total_blocks);
}

void bitmap_heap_free(struct bitmap_heap* heap, void* ptr)
{
    mark_bitmap_blocks_free(heap->table, address_to_start_block(heap, ptr));
}