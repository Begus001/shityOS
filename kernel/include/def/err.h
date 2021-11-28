#ifndef KERNEL_ERR_H
#define KERNEL_ERR_H

#include <tty/tty.h>
#include <io/serial.h>

_Noreturn static inline void err_halt(char *msg)
{
	dbgprintf(msg);
	tty_set_color(COLOR_BLACK, COLOR_RED);
	kprintf(msg);
	__asm__ __volatile__("int 0");
}

_Noreturn static inline void err_unreachable(void)
{
	err_halt("Unreachable code reached\n");
}

#endif
