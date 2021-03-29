#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

#include <def/int.h>
#include <stddef.h>

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *dest, u8 c, size_t n);

size_t strlen(const char *str);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);

#endif
