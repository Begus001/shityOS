#include <stdint-gcc.h>

#include <tty/tty.h>
#include <tty/vga.h>

static uint16_t *vidmem = (uint16_t *)0xB8000;

static size_t MAX_COLS = 80, MAX_ROWS = 25;

static size_t col, row;
static uint8_t color;

void tty_init(void)
{
	color = vga_get_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	tty_clear();
}

void tty_clear(void)
{
	for (size_t i = 0; i < MAX_COLS * MAX_ROWS; i++)
	{
		vidmem[i] = vga_clear_char(color);
	}
	col = row = 0;
}

void tty_scroll_one(void)
{
	for (size_t i = 0; i < (MAX_COLS * MAX_ROWS) - MAX_COLS; i++)
	{
		if (vidmem[i] == vidmem[i + MAX_COLS])
			continue;
		vidmem[i] = vidmem[i + MAX_COLS];
	}

	for (size_t i = MAX_COLS * MAX_ROWS; i >= (MAX_COLS * MAX_ROWS) - MAX_COLS; i--)
	{
		vidmem[i] = vga_clear_char(color);
	}

	col = 0;
}

void tty_putc(char c)
{
	switch (c)
	{
	case '\n':
	{
		col = 0;
		goto inc_row;
	}
	break;
	}

	const size_t index = row * MAX_COLS + col;
	vidmem[index] = vga_entry(c, color);

	if (++col >= MAX_COLS)
	{
	inc_row:
		col = 0;
		if (++row >= MAX_ROWS)
		{
			row--;
			tty_scroll_one();
		}
	}
}

void tty_puts(const char *s)
{
	while (*s != '\0')
		tty_putc(*s++);
}

int tty_putn(size_t n, unsigned char base)
{
	if (base > 16)
		return 1;

	const char *digit_buf = "0123456789ABCDEF";
	char buf[PUTNBUF];
	char *p;

	p = buf + PUTNBUF - 1;

	*p = '\0';

	while(n)
	{
		if (--p < buf)
			return 1;

		*p = digit_buf[n % base];
		n /= base;
	}

	tty_puts(p);
	
	return 0;
}

void kprintf(const char *fmt, ...)
{
	va_list va;

	const char *s;
	unsigned long n;

	va_start(va, fmt);

	while(*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			switch(*fmt)
			{
				case 'd':
				case 'i':
				case 'u':
				{
					n = va_arg(va, unsigned long int);
					tty_putn(n, 10);
				}
				break;

				case 'x':
				{
					n = va_arg(va, unsigned long int);
					tty_puts("0x");
					tty_putn(n, 16);
				}
				break;

				case 'b':
				{
					n = va_arg(va, unsigned long int);
					tty_puts("0b");
					tty_putn(n, 2);
				}
				break;

				case 'o':
				{
					n = va_arg(va, unsigned long int);
					tty_puts("0o");
					tty_putn(n, 8);
				}
				break;

				case 's':
				{
					s = va_arg(va, char *);
					tty_puts(s);
				}
				break;

				case 'c':
				{
					n = va_arg(va, int);
					tty_putc(n);
				}
				break;

				case '%':
				{
					tty_putc('%');
				}
				break;

				default:
				{
					tty_putc('%');
					tty_putc(*fmt);
				}
				break;
			}
		}
		else
		{
			tty_putc(*fmt);
		}

		fmt++;
	}
	va_end(va);
}
