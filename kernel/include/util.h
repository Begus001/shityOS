#ifndef KERNEL_UTIL_H
#define KERNEL_UTIL_H

#include <stdint.h>

void outb(uint16_t addr, uint8_t data);
uint8_t inb(uint16_t addr);

#endif
