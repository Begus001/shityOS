#include <string.h>

void *memcpy(void *dest, const void *src, size_t n)
{
	uint8_t *d = (uint8_t *) dest;
	const uint8_t *s = (uint8_t *) src;

	while (n--)
		*d++ = *s++;

	return dest;
}

void *memset(void *dest, uint8_t c, size_t n)
{
	uint8_t *d = (uint8_t *) dest;

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
