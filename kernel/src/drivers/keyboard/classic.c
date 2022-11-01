#include "drivers/keyboard/classic.h"
#include "drivers/keyboard/keyboard.h"
#include "task.h"
#include "idt.h"
#include "io.h"

static char key_map[256] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', '\b', 0, 'q', 'w', 'e', 'r', 't', 'y', 'u',
    'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f',
    'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z',
    'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

static char shift_key_map[256] = {
    0, 1, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U',
    'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C',
    'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '
};

static unsigned char shift_code[256] = {
    [0x2A] = SHIFT,[0x36] = SHIFT,[0xAA] = SHIFT,[0xB6] = SHIFT
};

static unsigned char caps_lock_code[256] = {
    [0x3A] = CAPS_LOCK
};

static unsigned int state;

int classic_keyboard_initialize();
void classic_keyboard_interrupt_handler();

struct keyboard classic_keyboard = {
    .name = {"Classic"},
    .init = classic_keyboard_initialize
};

int classic_keyboard_initialize()
{
    idt_register_interrupt_callback(ISR_KEYBOARD_INTERRUPT, classic_keyboard_interrupt_handler);

    // https://wiki.osdev.org/%228042%22_PS/2_Controller
    // 0xAE:	Enable first PS/2 port
    outb(PS2_PORT, PS2_COMMAND_ENABLE_FIRST_PORT);
    return 0;
}

static char classic_keyboard_read()
{
    unsigned char scan_code;
    char ch;

    scan_code = insb(KEYBOARD_INPUT_PORT);

    if (scan_code == KEYBOARD_E0_KEY) {
        state |= E0_SIGN;
        return 0;
    }

    if (state & E0_SIGN) {
        state &= ~E0_SIGN;
        return 0;
    }

    if (scan_code & KEYBOARD_KEY_UP) {
        state &= ~(shift_code[scan_code]);
        return 0;
    }

    state |= shift_code[scan_code];
    state ^= caps_lock_code[scan_code];

    if (state & SHIFT) {
        ch = shift_key_map[scan_code];
    }
    else {
        ch = key_map[scan_code];
    }

    if (state & CAPS_LOCK) {
        if ('a' <= ch && ch <= 'z')
            ch -= 32;
        else if ('A' <= ch && ch <= 'Z')
            ch += 32;
    }

    return ch;
}

void classic_keyboard_interrupt_handler()
{
    char ch = classic_keyboard_read();
    acknowledge_pic(ISR_KEYBOARD_INTERRUPT);

    if (ch > 0)
    {
        keyboard_push(ch);
        task_wake_up(PROCESS_WAIT_FOR_KEYBOARD);
    }
}

struct keyboard* classic_initialize()
{
    return &classic_keyboard;
}