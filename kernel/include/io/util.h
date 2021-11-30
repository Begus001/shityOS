#ifndef KERNEL_UTIL_H
#define KERNEL_UTIL_H

#include <def/int.h>

static inline void out8(u16 addr, u8 data)
{
	asm volatile("out %1, %0" : : "a"(data), "Nd"(addr));
}

static inline u8 in8(u16 addr)
{
	u8 value;
	asm volatile("in %0, %1" : "=a"(value) : "Nd"(addr));
	return value;
}

#endif
