#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#include "config.h"

struct gdt
{
    uint16_t limt_low;               // Limit (low).
    uint16_t base_low1;              // Base (low).
    uint8_t  base_low2;              // Base (middle)
    union {
        uint8_t  access_byte;            // Present=1 + DPL=00 + S=1 (system segment) + Type=1000(Execute only)
        struct {
            uint8_t access : 1;
            uint8_t read_write : 1;
            uint8_t data_or_code : 1;
            uint8_t executable : 1;
            uint8_t s_type : 1;
            uint8_t privilege_level : 2;
            uint8_t present : 1;
        } access_byte_fields;
    };
    union {
        uint8_t  limit_flags;             // Granularity, 64 bits flag, limit19:16.
        struct {
            uint8_t limit : 4;
            uint8_t reserved : 1;
            uint8_t long_mode : 1;
            uint8_t size : 1;
            uint8_t granularity : 1;

        } limit_flags_fields;
    };
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