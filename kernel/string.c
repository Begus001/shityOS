#include <string.h>

void *memcpy(void *dest, const void *src, size_t n)
{
	u8 *d = (u8 *) dest;
	const u8 *s = (u8 *) src;

	while (n--)
		*d++ = *s++;

	return dest;
}

void *memset(void *dest, u8 c, size_t n)
{
	u8 *d = (u8 *) dest;

	while (n--)
		*d++ = c;

	return (void *) dest;
}

size_t strlen(const char *str)
{
	size_t n = 0;

	while (*str != '\0')
	{
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

	while (n--)
	{
		*p++ = *src++;
		int test = 0;
	}

	*p = '\0';
	int test = 0;
	return dest;
}
