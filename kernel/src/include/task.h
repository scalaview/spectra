#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdbool.h>
#include "paging/paging.h"
#include "idt.h"

typedef enum
{
    UNUSED,
    INIT,
    RUNNING,
    READY,
    WAIT,
    TERMINATE
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
};

struct task
{
    struct pml4_table* page_chunk;
    struct registers registers;
    struct process* process;
    TASKSTATE state;
    struct task* next;
    struct task* prev;
    struct task* thead; //threads head
    void* tstack;
    void* kstack;
    uint64_t wait;
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
    struct task_wrapper terminal_list;
};

extern struct tasks_manager tasks_manager;

struct task* create_task(struct process* process);
void task_switch(struct registers* registers);
void task_launch(struct task* task);
void task_save_current_state(struct interrupt_frame* frame);
void task_list_set_current(struct task* task);
bool is_list_empty(struct task_wrapper* list);
void task_list_add_one(struct task_wrapper* list, struct task* task);
void task_list_remove_one(struct task_wrapper* list, struct task* task);
void task_run_schedule();
void task_ready_list_append_one(struct task* task);
void task_run_next();
struct task* task_list_current();
struct task* task_list_next();
struct task* task_sleep_current(int wait);

extern void set_user_registers();

#endif