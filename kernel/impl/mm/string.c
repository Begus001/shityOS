#include <mm/mem.h>
#include <mm/vmm.h>

void *memcpy(void *dest, const void *src, size_t n)
{
	char *d = (char *) dest;
	const char *s = (char *) src;
	
	while (n--)
		*d++ = *s++;
	
	return dest;
}

void *memmove(void *dest, const void *src, size_t n)
{
	char *d = (char *) dest;
	
	char buf[4096];
	memcpy(buf, src, n);
	
	char *tmp = buf;
	
	while(n--) {
		*d++ = *tmp++;
	}
	
	return dest;
	// FIXME: Malloc buf when heap is available
}

void *memset(void *dest, char c, size_t n)
{
	char *d = (char *) dest;
	
	while (n--)
		*d++ = c;
	
	return (void *) dest;
}

int memcmp(const void *str1, const void *str2, size_t n)
{
	const char *s1 = (char *) str1;
	const char *s2 = (char *) str2;
	
	while (n-- && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	
	return *s1 - *s2;
}

size_t strlen(const char *str)
{
	size_t n = 0;
	
	while (*str != '\0') {
		n++;
		str++;
	}
	
	return n;
}

char *strcpy(char *dest, const char *src)
{
	return (char *) memcpy(dest, src, strlen(src) + 1);
}

char *strcat(char *dest, const char *src)
{
	size_t n = strlen(src);
	char *p = dest + strlen(dest);
	
	while (n--) {
		*p++ = *src++;
	}
	
	*p = '\0';
	return dest;
}

int strcmp(const char *str1, const char *str2)
{
	const char *s1 = (char *) str1;
	const char *s2 = (char *) str2;
	
	while (*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	
	return *s1 - *s2;
}
