#ifndef BITMAP_HEAP_H
#define BITMAP_HEAP_H

#include <stdint.h>
#include <stddef.h>

#define OS_BITMAP_BLOCK_SIZE    4096
#define BITMAP_HEAP_TABLE_ENTRY_TAKEN 0x01
#define BITMAP_HEAP_TABLE_ENTRY_FREE  0x00

#define BITMAP_HEAP_HAS_NEXT 0x80
#define BITMAP_HEAP_IS_FIRST 0x40

/**
 * @brief
 * avaliable in lower bit
 */
#define bitmap_table_entry_type(e) (e & 0x0f)
#define entry_free(e) (bitmap_table_entry_type(e) == BITMAP_HEAP_TABLE_ENTRY_TAKEN)
#define entry_taken(e) (e & BITMAP_HEAP_TABLE_ENTRY_TAKEN)
#define entry_has_next(e) (e & BITMAP_HEAP_HAS_NEXT)

typedef unsigned char BITMAP_HEAP_TABLE_ENTRY;


struct heap_bitmap_table
{
    BITMAP_HEAP_TABLE_ENTRY* entries;
    size_t total;
};

struct bitmap_heap
{
    struct heap_bitmap_table* table;
    void* start_addr;
};

#define align_down(p) ((uint64_t)(p) - ((uint64_t)(p) % OS_BITMAP_BLOCK_SIZE))
#define align_up(p) (((uint64_t)(p) % OS_BITMAP_BLOCK_SIZE != 0) ? (align_down(p) + OS_BITMAP_BLOCK_SIZE) : (uint64_t)(p))

int init_bitmap_heap(struct bitmap_heap* heap, void* start, void* end, struct heap_bitmap_table* heap_table);
void* bitmap_heap_malloc(struct bitmap_heap* heap, size_t size);
void bitmap_heap_free(struct bitmap_heap* heap, void* ptr);

#endif