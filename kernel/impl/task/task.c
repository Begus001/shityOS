#include <stddef.h>
#include <io/serial.h>
#include <tty/tty.h>
#include <intr/intr.h>
#include <def/err.h>

#include "task/task.h"

static u32 pid_counter = 0;
static task_t *root_task = NULL;

extern void kernel_stack_top;
extern void asm_load_kernel_task(context_t *);

_Noreturn static void kernel_task_entry(void)
{
	dbgprintf("TASK: Kernel task started\n");
	
	kprintf("Welcome to shityOS, the shittiest OS in the world!\n");
	dbgprintf("Welcome to shityOS, the shittiest OS in the world!\n");
	
	intr_enable();
	
	while (1);
}

void task_init(void)
{
	root_task = kmalloc(sizeof(task_t));
	memset(root_task, 0, sizeof(task_t));
	
	root_task->pid = pid_counter++;
	root_task->active = true;
	root_task->dir = kdir;
	root_task->stack = &kernel_stack_top;
	root_task->heap = kheap;
	root_task->context = (context_t) {.eip = (u32) kernel_task_entry, .eflags = 0x202, .cs =
	0x10, .ss = 0x10, .esp = (u32) &kernel_stack_top, .ebp = (u32) &kernel_stack_top};
	
	dbgprintf("kernel stack top %x\n", &kernel_stack_top);
	
	asm_load_kernel_task(&root_task->context);
	
	err_unreachable();
}

context_t *task_switch(void)
{

}
