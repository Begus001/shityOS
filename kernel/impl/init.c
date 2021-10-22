#include <def/int.h>
#include <mm/mem.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <tty/tty.h>
#include <tty/serial.h>
#include <gdt.h>
#include <intr/intr.h>
#include <boot/multiboot.h>
#include <io/keyboard.h>

_Noreturn void init(multiboot_info_t *mb_info)
{
	tty_init();
	
	gdt_init();
	
	intr_init();
	
	pmm_init(mb_info);
	pmm_memmap();
	
	vmm_init();
	
	kb_init();
	
	kprintf("Welcome to shityOS, the shittiest OS in the world!\n");
	dbgprintf("Welcome to shityOS, the shittiest OS in the world!\n");
	
	intr_enable();
	
	while (1);
}
