#include "message_queue.h"
#include "task.h"

void message_push(struct message_queue* queue, struct message* msg)
{
    if (!msg) return;

    int tail = queue->tail;

    if ((tail + 1) % OS_MAX_MESSAGE_LENGTH == queue->head)
    {
        int head = queue->head;
        queue->head = (++head) % OS_MAX_MESSAGE_LENGTH;
    }
    queue->buffer[tail++] = *msg;
    queue->tail = tail % OS_MAX_MESSAGE_LENGTH;
}

void message_pop(struct message_queue* queue, struct message* msg_out)
{
    int head = queue->head;
    // if (head == queue->tail)
    // {
    //     return;
    // }

    struct message msg = queue->buffer[head];
    while (!msg.event)
    {
        task_sleep(1);
        msg = queue->buffer[head];
    }
    *msg_out = msg;
    memset(&queue->buffer[head], 0, sizeof(struct message));
    queue->head = (head + 1) % OS_KEYBOARD_BUFFER_SIZE;
}