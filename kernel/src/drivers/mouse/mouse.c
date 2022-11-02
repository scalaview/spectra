/**
 *  mouse driver
 *  reference https://forum.osdev.org/viewtopic.php?f=1&t=10247
 */

#include "drivers/mouse/mouse.h"
#include "io.h"
#include "assert.h"
#include "idt.h"
#include "debug.h"
#include "drivers/vga/vesa.h"

uint8_t mouse_cycle = 0;
uint8_t mouse_byte[3];
int16_t mouse_x = 0;
int16_t mouse_y = 0;

int8_t abs(int8_t n) {
    const int8_t ret[2] = { n, -n };
    return ret[n < 0];
}

void mouse_wait(uint8_t a_type) {
    uint32_t _time_out = 100000;
    if (a_type == 0) {
        while (_time_out--) //Data
        {
            if ((insb(PS2_PORT_0X64) & 1) == 1) {
                return;
            }
        }
        return;
    }
    else {
        while (_time_out--) //Signal
        {
            if ((insb(PS2_PORT_0X64) & 2) == 0) {
                return;
            }
        }
        return;
    }
}

void mouse_write(uint8_t write) {
    mouse_wait(1);
    outb(PS2_PORT_0X64, SEND_0XD4_BYTE);
    mouse_wait(1);
    outb(PS2_PORT_0X60, write);
}

uint8_t mouse_read() {
    mouse_wait(0);
    return insb(PS2_PORT_0X60);
}

void ps2_mouse_interrupt_handler()
{
    switch (mouse_cycle)
    {
    case 0:
        mouse_byte[0] = insb(PS2_PORT_0X60);
        if (!(mouse_byte[0] & MOUSE_AVALIABLE))
            break;
        mouse_cycle++;
        break;
    case 1:
        mouse_byte[1] = insb(PS2_PORT_0X60);
        mouse_cycle++;
        break;
    case 2:
        mouse_byte[2] = insb(PS2_PORT_0X60);
        int8_t diff_x = mouse_byte[1];
        int8_t diff_y = mouse_byte[2];
        if (abs(diff_x) > 1024) diff_x = 0;
        if (abs(diff_y) > 768) diff_y = 0;
        mouse_x += diff_x;
        mouse_y -= diff_y;

        if (mouse_x >= 1020) mouse_x = 1020;
        if (mouse_x < 0) mouse_x = 0;
        if (mouse_y >= 768) mouse_y = 767;
        if (mouse_y < 0) mouse_y = 0;
        mouse_cycle = 0;
        break;
    }
    if (mouse_byte[0] & 0x01) {
        debug_printf("MOUSE_LEFT_CLICK ");
    }
    if (mouse_byte[0] & 0x02) {
        debug_printf("MOUSE_RIGHT_CLICK ");
    }
    if (mouse_byte[0] & 0x04) {
        debug_printf("MOUSE_MIDDLE_CLICK ");
    }
    debug_printf("mouse_x: %d, mouse_y: %d\n", mouse_x, mouse_y);
    test_draw1();
}
void mouse_initialize()
{
    uint8_t status;
    mouse_wait(1);
    outb(PS2_PORT_0X64, ENABLE_AUXILIARY_DEVICE);

    //Enable the interrupts
    mouse_wait(1);
    outb(PS2_PORT_0X64, 0x20);

    mouse_wait(0);
    status = (insb(PS2_PORT_0X60) | 2);

    mouse_wait(1);
    outb(PS2_PORT_0X64, PS2_PORT_0X60);

    mouse_wait(1);
    outb(PS2_PORT_0X60, status);

    // Tell the mouse to use default settings
    mouse_write(MOUSE_SET_DEFUAL);
    mouse_read();  // ACK

    // Enable the mouse
    mouse_write(MOUSE_ENABLE);
    mouse_read();  // ACK

    idt_register_interrupt_callback(ISR_MOUSE_INTERRUPT, ps2_mouse_interrupt_handler);
}