#include <util.h>

#include <debug/serial.h>
#include <tty/tty.h>

static void dbgputc(char c)
{
	if(c == '\n')
	{
		outb(COM1, '\r');
		outb(COM1, '\n');
		return;
	}

	outb(COM1, c);
}

static size_t dbgputs(const char *s)
{
	size_t char_count;
	while(*s != '\0')
	{
		dbgputc(*s++);
		char_count++;
	}

	return char_count;
}

static size_t vdbgprintf(const char *fmt, va_list va)
{
	size_t char_count;
	char buf[KPF_MAX] = "";
	char_count = kvsprintf(buf, fmt, va);
	dbgputs(buf);
	return char_count;
}

size_t dbgprintf(const char *fmt, ...)
{
	size_t char_count;
	va_list va;
	va_start(va, fmt);
	char_count = vdbgprintf(fmt, va);
	va_end(va);
	return char_count;
}
