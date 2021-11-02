#include <def/int.h>
#include <mm/mem.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <mm/heap.h>
#include <tty/tty.h>
#include <tty/serial.h>
#include <gdt.h>
#include <intr/intr.h>
#include <boot/multiboot.h>
#include <io/keyboard.h>

extern const void kernel_end;
extern const void kernel_start;

_Noreturn void init(multiboot_info_t *mb_info)
{
	tty_init();
	
	gdt_init();
	
	intr_init();
	
	pmm_init(mb_info);
	
	vmm_init();
	
	intr_enable();
	
	u32 heap_addr = (u32) &kernel_end;
	u32 index_addr = heap_addr + 0x1000;
	size_t index_size = 0x1000000;
	u32 mem_addr = (index_addr + index_size + 0x1000) & ~0xFF;
	size_t mem_size = 0x1000000;

	heap_t *heap = heap_create((void *) heap_addr, (void *) mem_addr, mem_size, (void *) index_addr,
	                           index_size, false);
	
//	vmm_print_kernel_dir();
//	vmm_print_table_kernel_dir(768);
	
	kprintf("Welcome to shityOS, the shittiest OS in the world!\n");
	dbgprintf("Welcome to shityOS, the shittiest OS in the world!\n");
	
	while (1);
}
