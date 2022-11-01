// mouse driver
// https://forum.osdev.org/viewtopic.php?f=1&t=10247
#include "drivers/mouse/mouse.h"
#include "io.h"
#include "assert.h"
#include "idt.h"
#include "debug.h"

uint8_t mouse_cycle = 0;
int8_t mouse_byte[3];
int8_t mouse_x = 0;
int8_t mouse_y = 0;

void mouse_wait(uint8_t a_type) {
    uint32_t _time_out = 100000;
    if (a_type == 0) {
        while (_time_out--) //Data
        {
            if ((insb(0x64) & 1) == 1) {
                return;
            }
        }
        return;
    }
    else {
        while (_time_out--) //Signal
        {
            if ((insb(0x64) & 2) == 0) {
                return;
            }
        }
        return;
    }
}

void mouse_write(uint8_t write) {
    mouse_wait(1);
    outb(0x64, 0xD4);
    mouse_wait(1);
    outb(0x60, write);
}

uint8_t mouse_read() {
    mouse_wait(0);
    return insb(0x60);
}

void ps2_mouse_interrupt_handler()
{
    switch (mouse_cycle)
    {
    case 0:
        mouse_byte[0] = insb(0x60);
        mouse_cycle++;
        break;
    case 1:
        mouse_byte[1] = insb(0x60);
        mouse_cycle++;
        break;
    case 2:
        mouse_byte[2] = insb(0x60);
        mouse_x = mouse_byte[1];
        mouse_y = mouse_byte[2];
        mouse_cycle = 0;
        break;
    }
    debug_printf("mouse_x: %d, mouse_y: %d\n", mouse_x, mouse_y);
    acknowledge_pic(ISR_MOUSE_INTERRUPT);
}
void mouse_initialize()
{
    uint8_t status;
    mouse_wait(1);
    outb(0x64, 0xA8);

    //Enable the interrupts
    mouse_wait(1);
    outb(0x64, 0x20);

    mouse_wait(0);
    status = (insb(0x60) | 2);

    mouse_wait(1);
    outb(0x64, 0x60);

    mouse_wait(1);
    outb(0x60, status);

    // Tell the mouse to use default settings
    mouse_write(0xF6);
    mouse_read();  // ACK

    // Enable the mouse
    mouse_write(0xF4);
    mouse_read();  // ACK

    idt_register_interrupt_callback(ISR_MOUSE_INTERRUPT, ps2_mouse_interrupt_handler);
}