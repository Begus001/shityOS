#ifndef KERNEL_MEM_H
#define KERNEL_MEM_H

#include <def/int.h>
#include <stddef.h>
#include <stdbool.h>
#include <mm/heap.h>

void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *dest, char c, size_t n);
int memcmp(const void *str1, const void *str2, size_t n);

size_t strlen(const char *str);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int strcmp(const char *str1, const char *str2);

void *kmalloc(size_t size);
void kfree(void *addr);

#endif
