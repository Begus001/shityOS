#include <sys/int.h>
#include <sys/string.h>

#include <tty/serial.h>
#include <tty/tty.h>

#include "vga_text.h"

static u16 *vidmem = (u16 *) 0xB8000;

static size_t MAX_COLS = 80, MAX_ROWS = 25;

static size_t col, row;
static u8 color;

void tty_init(void)
{
	color = vga_get_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	tty_clear();
	
	dbgprintf("Terminal initialized:\nColor: %x\n", color);
}

void tty_clear(void)
{
	for (size_t i = 0; i < MAX_COLS * MAX_ROWS; i++)
		vidmem[i] = vga_clear_char(color);
	
	col = row = 0;
	
	dbgprintf("Terminal cleared\n");
}

void tty_scroll_down(void)
{
	for (size_t i = 0; i < (MAX_COLS * MAX_ROWS) - MAX_COLS; i++) {
		if (vidmem[i] == vidmem[i + MAX_COLS])
			continue;
		vidmem[i] = vidmem[i + MAX_COLS];
	}
	
	for (size_t i = MAX_COLS * MAX_ROWS; i >= (MAX_COLS * MAX_ROWS) - MAX_COLS; i--)
		vidmem[i] = vga_clear_char(color);
	
	col = 0;
	
	dbgprintf("Terminal scrolled down\n");
}

static void putc(char c)
{
	switch (c) {
		case '\n':
			col = 0;
			goto inc_row;
	}
	
	const size_t index = row * MAX_COLS + col;
	vidmem[index] = vga_entry(c, color);
	
	if (++col >= MAX_COLS) {
inc_row:
		col = 0;
		if (++row >= MAX_ROWS) {
			row--;
			tty_scroll_down();
		}
	}
}

static size_t puts(const char *s)
{
	size_t char_count = 0;
	while (*s != '\0') {
		putc(*s++);
		char_count++;
	}
	
	return char_count;
}

static size_t sputn(char *buf, size_t n, unsigned char base)
{
	if (base > 16 || n > PUTN_MAX)
		return 0;
	
	const char *digit_buf = "0123456789ABCDEF";
	char strbuf[PUTN_BUF_MAX] = "";
	char *p;
	
	p = buf + PUTN_BUF_MAX - 1;
	
	*p = '\0';
	
	while (n) {
		if (--p < strbuf)
			return 0;
		
		*p = digit_buf[n % base];
		n /= base;
	}
	
	strcpy(buf, p);
	
	return strlen(p);
}

size_t kvsprintf(char *buf, const char *fmt, va_list va)
{
	if (strlen(fmt) > KPF_MAX)
		return 0;
	
	const char *s;
	char putn_buf[PUTN_BUF_MAX] = "";
	size_t n;
	
	size_t char_count = 0;
	
	while (*fmt) {
		if (char_count >= KPF_MAX)
			return char_count;
		
		char *head_pointer = buf + strlen(buf);
		if (*fmt == '%') {
			fmt++;
			switch (*fmt) {
				case 'd':
				case 'i':
				case 'u':
					n = va_arg(va, unsigned long int);
					char_count += sputn(putn_buf, n, 10);
					strcat(buf, putn_buf);
					break;
				
				case 'x':
					n = va_arg(va, unsigned long int);
					strcat(buf, "0x");
					char_count += 2;
					char_count += sputn(putn_buf, n, 16);
					strcat(buf, putn_buf);
					break;
				
				case 'b':
					n = va_arg(va, unsigned long int);
					strcat(buf, "0b");
					char_count += 2;
					char_count += sputn(putn_buf, n, 2);
					strcat(buf, putn_buf);
					break;
				
				case 'o':
					n = va_arg(va, unsigned long int);
					strcat(buf, "0");
					char_count += 2;
					char_count += sputn(putn_buf, n, 8);
					strcat(buf, putn_buf);
					break;
				
				case 's':
					s = va_arg(va, char *);
					char_count += strlen(s);
					strcat(buf, s);
					break;
				
				case 'c':
					n = va_arg(va, int);
					*head_pointer = (char) n;
					char_count++;
					break;
				
				case '%':
					*head_pointer = '%';
					char_count++;
					break;
				
				default:
					*head_pointer = '%';
					head_pointer++;
					*head_pointer = *fmt;
					char_count += 2;
					break;
			}
		} else {
			*head_pointer = *fmt;
			char_count++;
		}
		
		fmt++;
	}
	
	return char_count;
}

size_t ksprintf(char *buf, const char *fmt, ...)
{
	size_t char_count;
	va_list va;
	va_start(va, fmt);
	char_count = kvsprintf(buf, fmt, va);
	va_end(va);
	return char_count;
}

static size_t kvprintf(const char *fmt, va_list va)
{
	size_t char_count;
	char buf[KPF_MAX] = "";
	char_count = kvsprintf(buf, fmt, va);
	puts(buf);
	return char_count;
}

size_t kprintf(const char *fmt, ...)
{
	size_t char_count;
	va_list va;
	va_start(va, fmt);
	char_count = kvprintf(fmt, va);
	va_end(va);
	return char_count;
}
