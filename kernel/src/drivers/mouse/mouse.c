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
#include "message.h"
#include "window_manager.h"

uint8_t mouse_cycle = 0;
uint8_t mouse_byte[3];
int16_t mouse_x = 0;
int16_t __previous_mouse_x = 0;
int16_t mouse_y = 0;
int16_t __previous_mouse_y = 0;
extern struct video_info_struct vesa_video_info;
uint8_t previous_key = 0;

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
    uint8_t status = insb(MOUSE_STATUS);
    while (status & MOUSE_BBIT) {
        if (status & MOUSE_F_BIT) {
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
                if (abs(diff_x) > vesa_video_info.width) diff_x = 0;
                if (abs(diff_y) > vesa_video_info.height) diff_y = 0;
                int32_t moved_x = mouse_x + diff_x;
                int32_t moved_y = mouse_y - diff_y;
                if (moved_x > 0 && moved_x >= vesa_video_info.width)
                {
                    mouse_x = vesa_video_info.width - 1;
                }
                else if (moved_x < 0)
                {
                    mouse_x = 0;
                }
                else
                {
                    mouse_x += diff_x;
                }
                if (moved_y > 0 && moved_y >= vesa_video_info.height)
                {
                    mouse_y = vesa_video_info.height - 1;
                }
                else if (moved_y < 0)
                {
                    mouse_y = 0;
                }
                else
                {
                    mouse_y -= diff_y;
                }
                mouse_cycle = 0;
                struct message message;
                message.x = mouse_x;
                message.y = mouse_y;

                message.diff_x = mouse_x - __previous_mouse_x;
                message.diff_y = mouse_y - __previous_mouse_y;
                __previous_mouse_x = mouse_x;
                __previous_mouse_y = mouse_y;

                if (mouse_byte[0] & MOUSE_LEFT_CLICK) {
                    debug_printf("MOUSE_LEFT_CLICK ");
                    message.event = MESSAGE_MOUSE_PRESS;
                    message.key |= MOUSE_LEFT_CLICK;
                }
                if (mouse_byte[0] & MOUSE_RIGHT_CLICK) {
                    debug_printf("MOUSE_RIGHT_CLICK ");
                    message.event = MESSAGE_MOUSE_PRESS;
                    message.key |= MOUSE_RIGHT_CLICK;

                }
                if (mouse_byte[0] & MOUSE_MIDDLE_CLICK) {
                    debug_printf("MOUSE_MIDDLE_CLICK ");
                    message.event = MESSAGE_MOUSE_PRESS;
                    message.key |= MOUSE_MIDDLE_CLICK;
                }
                if (previous_key & MOUSE_LEFT_CLICK && message.key & MOUSE_LEFT_CLICK)
                {
                    debug_printf("MOUSE_DRAG\n");
                    message.event = MESSAGE_MOUSE_DRAG;
                    message.key |= MOUSE_LEFT_DRAG;
                }
                previous_key = mouse_byte[0];

                debug_printf("diff_x: %d, mouse_x: %d, diff_y: %d, mouse_y: %d\n", diff_x, mouse_x, diff_y, mouse_y);
                window_handle_message(&message);
                // if (message.event) window_add_message_to_focused(&message);
                break;
            }
        }
        status = insb(MOUSE_STATUS);
    }
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