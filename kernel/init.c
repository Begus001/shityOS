#include <sys/int.h>
#include <sys/string.h>
#include <tty/tty.h>
#include <tty/serial.h>

#ifdef x86

#include <gdt/gdt.h>
#include <intr/intr.h>

#endif

void init(void)
{
	tty_init();

#ifdef x86
	gdt_init();
#endif
	
	intr_init();
	intr_enable();
	
	kprintf("Welcome to shityOS, the shittiest OS in the world!\n");
	dbgprintf("Welcome to shityOS, the shittiest OS in the world!\n");
	
	while (1);
}
