#ifndef KERNEL_UTIL_H
#define KERNEL_UTIL_H

#include <sys/int.h>

static inline void out8(u16 addr, u8 data)
{
	__asm__ volatile("out %1, %0" : : "a"(data), "Nd"(addr));
}

static inline u8 in8(u16 addr)
{
	u8 value;
	__asm__ volatile("in %0, %1" : "=a"(value) : "Nd"(addr));
	return value;
}

#endif
