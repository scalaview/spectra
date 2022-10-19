#ifndef CLASSIC_KEYBOARD_H
#define CLASSIC_KEYBOARD_H

#define E0_SIGN         0
#define SHIFT           (1 << 1)
#define CAPS_LOCK       (1 << 2)

#define PS2_PORT                        0x64
#define PS2_COMMAND_ENABLE_FIRST_PORT   0xAE
#define ISR_KEYBOARD_INTERRUPT          0x21

// The PS/2 Controller itself uses 2 IO ports (IO ports 0x60 and 0x64). Like many IO ports, reads and writes may access different internal registers.
#define KEYBOARD_INPUT_PORT             0x60
#define KEYBOARD_KEY_UP                 0x80

// https://wiki.osdev.org/PS/2_Keyboard
#define KEYBOARD_E0_KEY                 0xE0
struct keyboard* classic_initialize();

#endif