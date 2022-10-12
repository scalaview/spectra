#ifndef COMMAND_ARGUMENT_H
#define COMMAND_ARGUMENT_H

struct command_argument
{
    char argument[512];
    struct command_argument* next;
};

struct command_argument* parser_argument_array(const void* argv[]);
void command_argument_free(struct command_argument* root);
#endif
