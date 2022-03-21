#include <stddef.h>
#include <io/serial.h>
#include <tty/tty.h>
#include <intr/intr.h>
#include <def/assert.h>
#include <io/keyboard.h>
#include <mm/mem.h>
#include <mm/gdt.h>

#include "task/task.h"

static u32 pid_counter = 0;
static task_t *root_task = NULL;

static void append_task(task_t *task)
{
	task_t *p = root_task;
	
	while (p->next) {
		p = p->next;
	}
	
	p->next = task;
}

task_t *task_new(void *entry_phys)
{
	task_t *new_task = kmalloc(sizeof(task_t));
	memset(new_task, 0, sizeof(task_t));
	
	void *page_dir_phys = vmm_create_directory();
	if (!vmm_alloc_at_dir(page_dir_phys, (void *) 0x100000, true))
		return NULL;
	
	new_task->dir = page_dir_phys;
	
	context_t *context = kmalloc(sizeof(context_t));
	*context = (context_t) {
			.cs = GDT_RING3_CODE | 3,
			.ss = GDT_RING3_DATA | 3,
			.eflags = 0x202,
			.eip = 0x100000,
			.esp = 0x100000 + 0x1000,
	};
	new_task->context = context;
	
	new_task->kernel_stack = kmalloc(KERNEL_STACK_SIZE);
	new_task->pid = pid_counter++;
	dbgprintf("TASK: task %d stack: %x\n", new_task->pid, (u32) (new_task->kernel_stack +
	                                                             KERNEL_STACK_SIZE));
	
	append_task(new_task);
	
	vmm_change_directory(page_dir_phys);
	memset((void *) 0x100000, (char) 0xEB, 1);
	memset((void *) 0x100001, (char) 0xFE, 1);
	vmm_change_directory(kdir_phys);
	
	return new_task;
}

void task_init(void)
{
	root_task = kmalloc(sizeof(task_t));
	memset(root_task, 0, sizeof(task_t));
	root_task->pid = pid_counter++;
	
	vmm_alloc_at((void *) IDLE_TASK_ADDR, true);
	memset((void *) IDLE_TASK_ADDR, (char) 0xEB, 1);
	memset((void *) (IDLE_TASK_ADDR + 1), (char) 0xFE, 1);
	
	context_t *context = kmalloc(sizeof(context_t));
	*context = (context_t) {
			.cs = GDT_RING3_CODE | 3,
			.ss = GDT_RING3_DATA | 3,
			.eflags = 0x202,
			.eip = IDLE_TASK_ADDR,
			.esp = IDLE_TASK_ADDR + 0x1000,
	};
	
	root_task->context = context;
	root_task->dir = kdir_phys;
	root_task->kernel_stack = kmalloc(KERNEL_STACK_SIZE);
	dbgprintf("TASK: root task stack: %x\n", (u32) (root_task->kernel_stack + KERNEL_STACK_SIZE));
	gdt_get_tss()[1] = (u32) (root_task->kernel_stack + KERNEL_STACK_SIZE);
	
	task_current = root_task;
}

context_t *task_switch(context_t *cntxt)
{
	if (task_current != root_task)
		task_current->context = cntxt;
	
	task_current = task_current->next;
	
	if (!task_current)
		task_current = root_task;
	
	vmm_change_directory(task_current->dir);
	
	if (task_current->next) {
		gdt_get_tss()[1] = (u32) (task_current->next->kernel_stack + KERNEL_STACK_SIZE);
	} else {
		gdt_get_tss()[1] = (u32) (root_task->kernel_stack + KERNEL_STACK_SIZE);
	}
	
	return task_current->context;
}
