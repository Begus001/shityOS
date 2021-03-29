#ifndef KERNEL_TTY_H
#define KERNEL_TTY_H

#include <stddef.h>
#include <stdarg.h>

#define PUTN_BUF_MAX 256
#define PUTN_MAX 0xFFFFFFFF
#define KPF_MAX 1024

void tty_init(void);
void tty_clear(void);
void tty_scroll_down(void);

size_t kvsprintf(char *buf, const char *fmt, va_list va);
size_t ksprintf(char *buf, const char *fmt, ...);
size_t kprintf(const char *fmt, ...);

#endif
