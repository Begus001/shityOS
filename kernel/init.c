#include <stdint-gcc.h>

#include <tty/tty.h>
#include <string.h>
#include <debug/serial.h>

void init(void)
{
	tty_init();
	kprintf("Welcome to shityOS, the shittiest OS in the World!\n");
	dbgprintf("Welcome to shityOS, the shittiest OS in the World!\n");
}
