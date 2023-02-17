#ifndef IO_SERIAL_H
#define IO_SERIAL_H

#include <stdint.h>

/// Serial port for QEMU.
#define SERIAL_COM1             0x03F8

void outportb(uint16_t port, uint8_t data);

#endif