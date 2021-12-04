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

task_t *task_new(void *entry)
{

}

_Noreturn static void idle_task(void)
{
	while(1);
}

void task_init(void)
{
	root_task = kmalloc(sizeof(task_t));
	memset(root_task, 0, sizeof(task_t));
	root_task->pid = pid_counter++;
	root_task->dir = kdir;
	vmm_alloc_at((void *) 0x100000, true);
	void *esp = vmm_alloc_size_at((void *) 0x101000, true, 0x4000) + 0x4000;
	memcpy((void *) 0x100000, idle_task, 0x1000);
	context_t context = {.cs = GDT_RING3_CODE | 3, .ss = GDT_RING3_DATA | 3, .eflags = 0x202, .eip =
	0x100000, .esp = (u32) esp};
	root_task->context = &context;
	root_task->dir = kdir_phys;
	root_task->next = NULL;
	
	task_current = root_task;
	
	intr_enable();
	
	kassert(false, "Unreachable code reached at end of task_init");
}

context_t *task_switch(context_t *cntxt)
{
	if (task_current != root_task)
		task_current->context = cntxt;
	
	task_current = task_current->next;
	
	if (!task_current)
		task_current = root_task;
	
	return task_current->context;
}
