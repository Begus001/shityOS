#include <def/int.h>
#include <tty/tty.h>
#include <string/string.h>
#include <debug/serial.h>
#include <gdt.h>

void init(void)
{
	tty_init();
	gdt_init();
	
	kprintf("Welcome to shityOS, the shittiest OS in the World!\n");
	dbgprintf("Welcome to shityOS, the shittiest OS in the World!\n");
}
