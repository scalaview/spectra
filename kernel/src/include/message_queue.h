#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <stdint.h>

#include "config.h"
#include "message.h"

struct message_queue
{
    struct message buffer[OS_MAX_MESSAGE_LENGTH];
    int tail;
    int head;
};


#endif