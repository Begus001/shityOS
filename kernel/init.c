#include <stdint-gcc.h>

#include <tty/tty.h>
#include <string.h>
#include <debug/serial.h>

void init(void)
{
	tty_init();
	char buf[10] = {'X', 'X', 'X', 'X', 'X', '\0', 'X', 'X', 'X', 'X'};
	kprintf(strcat(buf, "lole"));
	kprintf("\n\ndone?");
}
