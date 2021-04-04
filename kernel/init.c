#include <sys/def/int.h>
#include <sys/string.h>
#include <tty/tty.h>
#include <tty/serial.h>
#include <mm/pmm.h>

#ifdef x86

#include <gdt/gdt.h>
#include <intr/intr.h>
#include <boot/multiboot.h>

#endif

#ifdef x86
_Noreturn void init(multiboot_info_t *mb_info)
{
	tty_init();
	
	gdt_init();
	
	intr_init();
	
	pmm_init(mb_info);
	pmm_memmap();
	pmm_blocks_used();
	pmm_blocks_free();
	
	kprintf("Welcome to shityOS, the shittiest OS in the world!\n");
	dbgprintf("Welcome to shityOS, the shittiest OS in the world!\n");

	intr_enable();

	while(1);
}


#endif
