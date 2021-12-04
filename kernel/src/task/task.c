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

void task_init(void)
{
	root_task = kmalloc(sizeof(task_t));
	memset(root_task, 0, sizeof(task_t));
	root_task->pid = pid_counter++;
	root_task->dir = kdir;
	vmm_alloc_at((void *) IDLE_TASK_ADDR, true);
	memset((void *) IDLE_TASK_ADDR, (char) 0xEB, 1);
	memset((void *) IDLE_TASK_ADDR + 1, (char) 0xFE, 1);
	context_t context = {.cs = GDT_RING3_CODE | 3, .ss = GDT_RING3_DATA | 3, .eflags = 0x202, .eip =
	IDLE_TASK_ADDR, .esp = (u32) IDLE_TASK_ADDR + 0x1000};
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
