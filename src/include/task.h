#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include "paging/paging.h"
#include "idt.h"

struct registers
{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

struct task
{
    struct pml4_table* page_chunk;
    struct registers* registers;
    struct process* process;
    struct task* next;
    struct task* prev;
    void* rptr;
    void* tptr;
};

struct tasks_manager
{
    struct task* head;
    struct task* current;
    struct task* tail;
};

struct task* create_task(struct process* process);
void task_switch(struct registers* registers);
void task_launch(struct task* task);
void task_save_current_state(struct interrupt_frame* frame);

extern void set_user_registers();

#endif