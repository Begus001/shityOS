#ifndef KERNEL_UTIL_H
#define KERNEL_UTIL_H

#include <sys/int.h>

void out8(u16 addr, u8 data);
u8 in8(u16 addr);

#endif
