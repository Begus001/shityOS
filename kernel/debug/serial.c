#include <util.h>

#include <debug/serial.h>
#include <tty/tty.h>

size_t vdbgprintf(const char *fmt, va_list va)
{
	size_t char_count;
	char buf[KPF_MAX] = "";
	char_count = kvsprintf(buf, fmt, va);
	// TODO: Add dbgputs
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
