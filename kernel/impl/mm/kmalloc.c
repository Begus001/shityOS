#include <mm/mem.h>

extern const u32 kernel_end;

static u32 addr = (u32) &kernel_end;

void *kmalloc_internal(u32 size, bool page_align, u32 *physical_addr)
{
	if (page_align) {
		addr &= 0xFFFFF000;  // Align addr to page 4k
		addr += 0x1000;
	} else {
		for (; addr % size; addr++);  // Align addr to size
	}
	
	if (physical_addr)
		*physical_addr = addr;
	
	u32 tmp = addr;
	addr += size;
	return (void *) tmp;
}
