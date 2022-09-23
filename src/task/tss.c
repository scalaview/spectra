#include "tss.h"
#include "gdt.h"
#include "config.h"
#include "kmemory.h"

struct tss_entry tss;

void initialize_tss()
{
    extern void* kernel_stack_ptr;
    extern struct gdt gdt_tss64;
    extern struct gdt_descriptor gdt64_descriptor;
    extern void* ist_1_stack_ptr;
    extern void* ist_2_stack_ptr;
    memset(&tss, 0, sizeof(tss));
    tss.rsp0 = (uint64_t)&kernel_stack_ptr;
    tss.ist1 = (uint64_t)&ist_1_stack_ptr;
    tss.ist2 = (uint64_t)&ist_2_stack_ptr;
    gdt_tss64.limt_low = sizeof(tss);
    gdt_tss64.base_low1 = ((uint64_t)&tss) & 0xffff;
    gdt_tss64.base_low2 = (((uint64_t)&tss) >> 16) & 0xff;
    gdt_tss64.base_high1 = (((uint64_t)&tss) >> 24) & 0xff;
    gdt_tss64.base_high2 = (((uint64_t)&tss) >> 32) & 0xffffffff;
    gdt_load(&gdt64_descriptor);
    tss_load(TSS_SEGMENT);
}

