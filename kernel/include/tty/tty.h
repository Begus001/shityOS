#ifndef TTY_H
#define TTY_H

#define PUTNBUF 32

#include <stddef.h>
#include <stdarg.h>

void tty_init(void);
void tty_clear(void);
void tty_scroll_one(void);
void tty_putc(char c);
void tty_puts(const char *s);
int tty_putn(size_t n, unsigned char base);
void kprintf(const char *fmt, ...);

#endif
