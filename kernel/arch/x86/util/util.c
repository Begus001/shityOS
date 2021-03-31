
#include <util.h>

void out8(u16 addr, u8 data)
{
	asm volatile("out %1, %0" : : "a"(data), "Nd"(addr));
}

u8 in8(u16 addr)
{
	u8 value;
	asm volatile("in %0, %1" : "=a"(value) : "Nd"(addr));
	return value;
}