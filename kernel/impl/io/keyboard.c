#include <io/util.h>

#include <stdbool.h>
#include <io/keyboard.h>
#include <tty/tty.h>
#include <io/serial.h>

#define IO_BUF_ADDR 0x60
#define STAT_ADDR 0x64

static u8 german_set[1024] = {
		0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 's', '\'', 0,
		0, 'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', 'u', '+', '\n',
		0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'o', 'a', '^', 0, '#',
		'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0,
		'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		'7', '8', '9', '-',
		'4', '5', '6', '+',
		'1', '2', '3',
		'0', ',', 0, 0, 0, 0, 0
};


static inline u8 read_status(void)
{
	return in8(STAT_ADDR);
}

static inline void send_command(u8 data)
{
	do {
		while (read_status() & 0x2);
		out8(IO_BUF_ADDR, data);
	} while (in8(0x60) != 0xFA);
}

static inline u8 read_data(u8 addr)
{
	while (!(read_status() & 0x1));
	
	return in8(addr);
}
bool shift_switch = false;
void kb_handle_irq(void)
{
	u8 key = kb_get_key();
	
	if (key < 0x80)
		kprintf("%c", german_set[key]);
	
	dbgprintf("KB: %x\n", key);
	
	// TODO: Notify key pressed
}

void kb_init(void)
{
	send_command(0xF4);
}

u8 kb_get_key(void)
{
	return read_data(IO_BUF_ADDR);
}
