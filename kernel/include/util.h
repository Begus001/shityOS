#ifndef KERNEL_UTIL_H
#define KERNEL_UTIL_H

#include <def/int.h>

void outb(u16 addr, u8 data);
u8 inb(u16 addr);

#endif
