#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#include "config.h"

struct gdt
{
    uint16_t limt_low;               // Limit (low).
    uint16_t base_low1;              // Base (low).
    uint8_t  base_low2;              // Base (middle)
    uint8_t  access_byte;            // Present=1 + DPL=00 + S=1 (system segment) + Type=1000(Execute only)
    uint8_t  flag_limit;             // Granularity, 64 bits flag, limit19:16.
    uint8_t  base_high1;             // Base (high).
    uint32_t base_high2;             // Base (high).
} __attribute__((packed));

struct gdt_descriptor
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

void gdt_load(struct gdt_descriptor* gdt_ptr);

#endif