#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdbool.h>
#include "paging/paging.h"
#include "idt.h"
#include "process.h"

typedef enum
{
    TASK_UNUSED,
    TASK_INIT,
    TASK_RUNNING,
    TASK_READY,
    TASK_WAIT,
    TASK_TERMINATE
} TASKSTATE;

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
} __attribute__((packed));

struct stack_allocation
{
    void* stack_bottom;
    void* stack_top;
    void* heap_address;
};

struct task
{
    struct registers* registers;
    struct process* process;
    TASKSTATE state;
    struct task* next;
    struct task* prev;
    //TODO support thread task
    struct task* th_next; //threads next
    struct stack_allocation t_stack;
    void* k_stack;
    uint64_t k_context;
    int64_t wait;
};

struct task_wrapper
{
    struct task* next;
    struct task* tail;
};

struct tasks_manager
{
    struct task* current;
    struct task_wrapper ready_list;
    struct task_wrapper wait_list;
    struct task_wrapper terminated_list;
};

extern struct tasks_manager tasks_manager;
extern void restore_registers();
extern void task_context_switch(uint64_t* current, uint64_t next);

struct task* create_task(struct process* process);
void task_switch(struct task* task);
void task_start(void* stack_ptr);
void task_launch(struct task* task);
void task_list_set_current(struct task* task);
bool is_list_empty(struct task_wrapper* list);
void task_list_add_one(struct task_wrapper* list, struct task* task);
void task_list_remove_one(struct task_wrapper* list, struct task* task);
void task_schedule();
void tasks_run();
struct task* task_list_pop_head(struct task_wrapper* list);
void task_ready_list_append_one(struct task* task);
void yield();
struct task* task_list_current();
struct task* task_list_next();
void task_sleep_until(int wait);
void task_wake_up(int wait);
void task_active(struct task* task);
void task_free(struct task* task);
void task_sleep(int wait);
int task_clone(struct task* src, struct task* dest);
struct allocation* task_malloc(struct task* task, size_t size);
uint8_t page_flags_by_ring(RING_LEV ring);

extern void set_user_registers();

#endif