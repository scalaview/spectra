#include "drivers/keyboard/keyboard.h"
#include "drivers/keyboard/classic.h"
#include "status.h"
#include "task.h"

static struct keyboard* keyboard_list_head = 0;
static struct keyboard* keyboard_list_tail = 0;

static struct keyboard_buffer keyboard_buffer = {
    .buffer = {0},
    .head = 0,
    .tail = 0
};

int keyboard_insert(struct keyboard* keyboard)
{
    int res = 0;
    if (keyboard->init == 0)
    {
        res = -EINVARG;
        goto out;
    }

    if (keyboard_list_tail)
    {
        keyboard_list_head->next = keyboard;
        keyboard_list_tail = keyboard;
    }
    else
    {
        keyboard_list_head = keyboard;
        keyboard_list_tail = keyboard;
    }

    res = keyboard->init();
out:
    return res;
}

void keyboard_initialize()
{
    keyboard_insert(classic_initialize());
}

void keyboard_push(char c)
{
    if (c == 0)
    {
        return;
    }

    int head = keyboard_buffer.head;
    int tail = keyboard_buffer.tail;

    if ((tail + 1) % OS_KEYBOARD_BUFFER_SIZE == head)
    {
        keyboard_buffer.head = ++head % OS_KEYBOARD_BUFFER_SIZE;
    }
    keyboard_buffer.buffer[tail++] = c;
    keyboard_buffer.tail = tail % OS_KEYBOARD_BUFFER_SIZE;
}

char keyboard_pop()
{
    int head = keyboard_buffer.head;
    if (head == keyboard_buffer.tail)
    {
        task_sleep(PROCESS_WAIT_FOR_KEYBOARD);
    }

    char c = keyboard_buffer.buffer[head];
    while (!c)
    {
        task_sleep(PROCESS_WAIT_FOR_KEYBOARD);
        c = keyboard_buffer.buffer[head];
    }

    keyboard_buffer.buffer[head] = 0;
    keyboard_buffer.head = (head + 1) % OS_KEYBOARD_BUFFER_SIZE;
    return c;
}