#include "lib.h"

static int read_cmd(char* buffer)
{
    char ch[2] = { 0 };
    int buffer_size = 0;

    while (1) {
        ch[0] = keyboard_getkey();

        if (ch[0] == '\n' || buffer_size >= 80) {
            printf("%s", ch);
            break;
        }
        else if (ch[0] == '\b') {
            if (buffer_size > 0) {
                buffer_size--;
                printf("%s", ch);
            }
        }
        else {
            buffer[buffer_size++] = ch[0];
            printf("%s", ch);
        }
    }

    return buffer_size;
}

int main(int argc, char** argv)
{
    char buffer[80] = { 0 };
    int buffer_size = 0;

    while (1) {
        printf("shell# ");
        buffer_size = read_cmd(buffer);

        if (buffer_size == 0) {
            continue;
        }
    }
    return 0;
}