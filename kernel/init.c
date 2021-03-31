#include <sys/int.h>
#include <sys/string.h>
#include <tty/tty.h>
#include <tty/serial.h>
#include <descriptor_tables/gdt.h>

void init(void)
{
	tty_init();
	gdt_init();
	
	kprintf("Welcome to shityOS, the shittiest OS in the world!\n");
	dbgprintf("Welcome to shityOS, the shittiest OS in the world!\n");
}
