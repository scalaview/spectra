#include "command_argument.h"
#include "systemctl/process.h"
#include "string.h"
#include "kmemory.h"
#include "heap/kheap.h"
#include "status.h"

void command_argument_free(struct command_argument* root)
{
    struct command_argument* current = root;
    struct command_argument* prev = current;
    while (current)
    {
        prev = current;
        current = prev->next;
        kfree(prev);
    }
}

struct command_argument* parser_argument_array(const void* argv[])
{
    int res = 0;
    struct command_argument* root_command_argument = 0;
    if (argv)
    {
        struct command_argument* prev_command_argument = 0;
        for (int i = 0; i < ARGV_SIZE;i++)
        {
            if (!argv[i]) break;
            struct command_argument* command_argument = kzalloc(sizeof(struct command_argument));
            if (!command_argument) {
                res = -ENOMEM;
                goto out;
            }
            memcpy(command_argument->argument, (void*)argv[i], strlen(argv[i]));
            if (i == 0) root_command_argument = command_argument;
            if (prev_command_argument)   prev_command_argument->next = command_argument;
            prev_command_argument = command_argument;
        }
    }
out:
    if (res < 0)
    {
        command_argument_free(root_command_argument);
        return 0;
    }
    return root_command_argument;
}


