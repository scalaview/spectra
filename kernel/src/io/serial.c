#include "serial.h"
#include "io.h"

int __is_transmit_empty(uint16_t port) {
    return insb(port + 5) & 0x20;
}

void outportb(uint16_t port, uint8_t data)
{
    // while (__is_transmit_empty(port) == 0);
    outb(SERIAL_COM1, data);
}