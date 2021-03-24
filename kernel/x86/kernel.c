#include <stdint-gcc.h>

#include <tty/tty.h>
#include <string.h>

void kernel_init(void)
{
	tty_init();
	kprintf("Welcome to shityOS, the shittiest OS in the world!\n");
}
