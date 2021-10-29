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
	
	u32 heap_start = (u32) (&kernel_end + 0x1000) & ~0xFF;
	u32 heap_index_start = heap_start + 0x1000;
	
	dbgprintf("MAIN: kernel_start, _end: %x, %x\n", (u32) &kernel_start, (u32) &kernel_end);
	
	vmm_print_kernel_dir();
	vmm_print_table_kernel_dir(768);
	
	kprintf("Welcome to shityOS, the shittiest OS in the world!\n");
	dbgprintf("Welcome to shityOS, the shittiest OS in the world!\n");
	
	while (1);
}
