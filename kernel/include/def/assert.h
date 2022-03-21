#ifndef KERNEL_ASSERT_H
#define KERNEL_ASSERT_H

#include <tty/tty.h>
#include <io/serial.h>
#include <stdbool.h>

static inline void kassert(bool assertion, char *msg)
{
	if (assertion)
		return;
	
	dbgprintf("Kernel assertion failed:\n%s\nSystem halted\n", msg);
	tty_set_color(COLOR_BLACK, COLOR_RED);
	kprintf("Kernel assertion failed:\n%s\nSystem halted\n", msg);
	
	asm volatile("cli;"
	             "hlt;"
	             "jmp $;"::);
}

#endif
