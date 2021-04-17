#ifndef KERNEL_MEM_H
#define KERNEL_MEM_H

#include <def/int.h>
#include <stddef.h>
#include <stdbool.h>

void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *dest, char c, size_t n);
int memcmp(const void *str1, const void *str2, size_t n);

size_t strlen(const char *str);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int strcmp(const char *str1, const char *str2);

void *kmalloc_internal(u32 size, bool page_align, u32 *physical_addr);

static inline void *kmalloc(u32 size)
{
	return kmalloc_internal(size, false, 0x0);
}

static inline void *kamalloc(u32 size)
{
	return kmalloc_internal(size, true, 0x0);
}

static inline void *kpmalloc(u32 size, u32 *physical_addr)
{
	return kmalloc_internal(size, false, physical_addr);
}

static inline void *kapmalloc(u32 size, u32 *physical_addr)
{
	return kmalloc_internal(size, true, physical_addr);
}

#endif
