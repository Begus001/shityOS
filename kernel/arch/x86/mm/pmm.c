#include <stdbool.h>
#include <tty/serial.h>
#include <tty/tty.h>
#include <sys/string.h>

#include <mm/pmm.h>

#define BLOCK_SIZE 4096
#define BITS       32
#define BITMAP_MAX 4294967296/BLOCK_SIZE/BITS  // 32768

static u32 bitmap[BITMAP_MAX];
static u32 blocks_used = BITMAP_MAX * BITS;

extern const void kernel_start;
extern const void kernel_end;

static inline bool test_bit(size_t bit)
{
	return bitmap[bit / BITS] & (1 << (bit % BITS));
}

static inline void mark_free(void *addr)
{
	uptr index = (uptr) addr / BLOCK_SIZE;
	bitmap[index / BITS] &= ~(1 << (index % BITS));
	blocks_used--;
}

static inline void mark_used(void *addr)
{
	uptr index = (uptr) addr / BLOCK_SIZE;
	bitmap[index / BITS] |= (1 << (index % BITS));
	blocks_used++;
}

void pmm_init(void *info_struct)
{
	memset(bitmap, 0xFF, sizeof(bitmap));  // Mark all used
	
	multiboot_info_t *mb_info = (multiboot_info_t *) info_struct;
	
	if (!(mb_info->flags & (1 << 6))) {
		dbgprintf("No memory map information was given by the bootloader");
		__asm__ volatile("cli;hlt");
	}
	
	multiboot_mmap_t *mb_mmap = mb_info->mmap_addr;
	multiboot_mmap_t *mb_mmap_end = (void *) ((uptr) mb_mmap + mb_info->mmap_length);
	
	while (mb_mmap < mb_mmap_end) {
		if (mb_mmap->type == 1) {  // Mark memory free
			uptr addr = (uptr) mb_mmap->base_addr;
			uptr end_addr = (uptr) addr + mb_mmap->length;
			
			while (addr < end_addr) {
				mark_free((void *) addr);
				addr += BLOCK_SIZE;
			}
		}
		mb_mmap++;
	}
	
	uptr addr = (uptr) &kernel_start;
	uptr end_addr = (uptr) &kernel_end;
	while (addr < end_addr) {  // Mark kernel used again
		mark_used((void *) addr);
		addr += BLOCK_SIZE;
	}
	
	mark_used((void *) 0);
}

void pmm_memmap(void)
{
	size_t used = 0, free = 0, stretch_start = 0, stretch_end, stretch_count = 0;
	bool stretch_used;
	
	dbgprintf("Memory map:\n");
	
	if (bitmap[0] & 0x1)
		stretch_used = true;
	else
		stretch_used = false;
	
	for (size_t i = 0; i < BITMAP_MAX * BITS; i++) {
		if (test_bit(i) != stretch_used) {
			stretch_end = i - 1;
			dbgprintf("Stretch %d: blocks %d to %d %s (%d)\n", stretch_count++, stretch_start,
			          stretch_end, stretch_used ? "used" : "free", stretch_end - stretch_start + 1);
			stretch_used = !stretch_used;
			stretch_start = stretch_end + 1;
		}
		
		if (test_bit(i))
			used++;
		else
			free++;
	}
	
	stretch_end = (BITMAP_MAX - 1) * BITS + BITS - 1;
	dbgprintf("Stretch %d: blocks %d to %d %s (%d)\n", stretch_count, stretch_start, stretch_end,
	          stretch_used ? "used" : "free", stretch_end - stretch_start + 1);
	
	dbgprintf("Blocks in use: %d\nBlocks free: %d\n", used, free);
}

void pmm_blocks_used(void)
{
	dbgprintf("Blocks in use: %d\n", blocks_used);
}

void pmm_blocks_free(void)
{
	dbgprintf("Blocks free: %d\n", BITMAP_MAX * BITS - blocks_used);
}

void *pmm_alloc(size_t size)
{
	size_t stretch_start, stretch_end;
	bool counting = false;
	
	for (size_t i = 0; i < BITMAP_MAX * BITS; i++) {
		if (!test_bit(i) && !counting) {
			stretch_start = i;
			counting = true;
		} else if (test_bit(i) && counting) {
			stretch_end = i - 1;
			if ((stretch_end - stretch_start + 1) * BLOCK_SIZE >= size) {
				for (size_t k = 0; k < size / (BLOCK_SIZE + 1) + 1; k++)
					mark_used((void *) (stretch_start + k));
				return (void *) (stretch_start * BLOCK_SIZE);
			} else {
				counting = false;
			}
		}
	}
	
	return (void *) 0x0;
}

void *pmm_alloc_block(void)
{
	for (size_t i = 0; i < BITMAP_MAX; i++) {
		if (bitmap[i] == 0xFFFFFFFF)
			continue;
		for (u8 k = 0; k < 32; k++) {
			u32 test = (1 << k);
			if (!(bitmap[i] & test)) {
				bitmap[i] |= test;
				blocks_used++;
				return (void *) ((i * 32 + k) * BLOCK_SIZE);
			}
		}
	}
	
	return (void *) 0x0;
}

void pmm_free(void *addr, size_t size)
{
	for (size_t i = 0; i < size / (BLOCK_SIZE + 1) + 1; i++)
		mark_free((void *) (addr + i));
}

void pmm_free_block(void *addr)
{
	mark_free(addr);
}
