#include <tty/serial.h>
#include <tty/tty.h>

#define COM 0x3F8

static void dbgputc(char c)
{
	if(c == '\n')
	{
		out8(COM, '\r');
		out8(COM, '\n');
		return;
	}
	
	out8(COM, c);
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
