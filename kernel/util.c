
#include <util.h>

void outb(uint16_t addr, uint8_t data)
{
	asm volatile("outb %0, %1" : : "a"(data), "Nd"(addr));
}

uint8_t inb(uint16_t addr)
{
	uint8_t value;
	asm volatile("inb %1, %0" : "=a"(value) : "Nd"(addr));
	return value;
}