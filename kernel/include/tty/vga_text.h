#ifndef KERNEL_VGA_TEXT_H
#define KERNEL_VGA_TEXT_H

#include <def/int.h>

#include "tty/vga_color.h"

static inline u8 vga_get_color(enum color fg, enum color bg)
{
	return fg | bg << 4;
}

static inline u16 vga_entry(unsigned char c, u8 color)
{
	return (u16) c | (u16) (color << 8);
}

static inline u16 vga_clear_char(u8 color)
{
	return vga_entry((char) 0, color);
}

#endif
