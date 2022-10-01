#ifndef TASKSWITCHSEGMENT_H
#define TASKSWITCHSEGMENT_H

#include <stdint.h>

#define TSS_SEGMENT     0x28

struct tss_entry
{
    uint32_t reserved0;    //Reserved
    uint64_t rsp0;          //RSP0
    uint64_t rsp1;          //RSP1
    uint64_t rsp2;          //RSP2
    uint64_t reserved1;     //Reserved
    uint64_t ist1;          //IST1
    uint64_t ist2;          //IST2
    uint64_t ist3;          //IST3
    uint64_t ist4;          //IST4
    uint64_t ist5;          //IST5
    uint64_t ist6;          //IST6
    uint64_t ist7;          //IST17
    uint64_t reserved2;     //Reserved
    uint16_t reserved3;     //Reserved
    uint16_t io_map_base;   //I/O Map Base Address
} __attribute__((packed));

extern struct tss_entry tss;

void tss_load(int tss_segment);
void initialize_tss();
void set_tss_rsp0(uint64_t rsp0);

#endif
