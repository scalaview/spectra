#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <stdint.h>
#include <stdbool.h>

#include "config.h"
#include "message.h"
#include "kmemory.h"

struct message_queue
{
    struct message buffer[OS_MAX_MESSAGE_LENGTH];
    int tail;
    int head;
};

void message_push(struct message_queue* queue, struct message* msg);
void message_pop(struct message_queue* queue, struct message* msg_out);

#endif