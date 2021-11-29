#include <def/int.h>
#include <mm/mem.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <mm/heap.h>
#include <tty/tty.h>
#include <io/serial.h>
#include <intr/gdt.h>
#include <intr/intr.h>
#include <boot/multiboot.h>
#include <io/keyboard.h>
#include <task/task.h>
#include <def/err.h>

extern const void kernel_end;
extern const void kernel_start;

_Noreturn void init(multiboot_info_t *mb_info)
{
	tty_init();
	
	gdt_init();
	
	intr_init();
	
	pmm_init(mb_info);
	
	vmm_init();
	
	heap_init_kheap();
	
	task_init();
	
	err_unreachable();
}
