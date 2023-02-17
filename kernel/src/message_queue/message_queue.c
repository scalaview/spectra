#include "message_queue.h"
#include "debug.h"

void message_push(struct task* task, struct message_queue* queue, struct message* msg)
{
    if (!msg) return;

    int tail = queue->tail;

    queue->buffer[tail++] = *msg;
    queue->tail = tail % OS_MAX_MESSAGE_LENGTH;
    debug_printf("push task: %x, head: %d, tail: %d, msg: %d\n", task, queue->head, queue->tail, msg->event);
    task_wake_up_one(task, WINDOW_WAIT_FOR_MESSAGE);
}

void message_pop(struct task* task, struct message_queue* queue, struct message* msg_out)
{
    if (queue->head == queue->tail)  task_sleep_one_until(task, WINDOW_WAIT_FOR_MESSAGE);
    struct message msg = queue->buffer[queue->head];
    debug_printf("pop task: %x, head: %d, tail: %d, msg: %d\n", task, queue->head, queue->tail, msg.event);
    *msg_out = msg;
    memset(&queue->buffer[queue->head], 0, sizeof(struct message));
    queue->head = (queue->head + 1) % OS_MAX_MESSAGE_LENGTH;
}