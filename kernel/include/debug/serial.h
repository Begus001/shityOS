#ifndef KERNEL_SERIAL_H
#define KERNEL_SERIAL_H

#include <stddef.h>
#include <stdarg.h>

#define COM 0x3F8

size_t dbgprintf(const char *fmt, ...);

#endif