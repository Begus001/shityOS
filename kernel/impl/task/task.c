#include <stddef.h>
#include <io/serial.h>
#include <tty/tty.h>
#include <intr/intr.h>
#include <def/assert.h>
#include <io/keyboard.h>
#include <mm/mem.h>

#include "task/task.h"

static u32 pid_counter = 0;
static task_t *root_task = NULL;
task_t *current_task = NULL;

extern void asm_load_kernel_task(context_t *);

static void exec(char *cmd)
{
	kprintf("Unkown command: %s\n", cmd);
}

_Noreturn static void test(void)
{
	while (1) {
		kprintf("1");
		for (int i = 0; i < 10000000; ++i) {
			asm volatile("nop");
		}
	}
}

_Noreturn static void test2(void)
{
	while (1) {
		kprintf("2");
		for (int i = 0; i < 10000000; ++i) {
			asm volatile("nop");
		}
	}
}

_Noreturn static void kernel_task_entry(void)
{
	dbgprintf("TASK: Kernel task started\n");
	
	kprintf("Welcome to shityOS, the shittiest OS in the world!\n");
	dbgprintf("Welcome to shityOS, the shittiest OS in the world!\n");
	
	intr_enable();
	
	while (1) {
		kprintf("0");
		for (int i = 0; i < 10000000; ++i) {
			asm volatile("nop");
		}
	}
}

task_t *task_new(void *entry)
{
	task_t *new_task = kmalloc(sizeof(task_t));
	memset(new_task, 0, sizeof(task_t));
	
	new_task->pid = pid_counter++;
	new_task->active = true;
	new_task->dir = kdir;
	new_task->stack = kmalloc(0x4000) + 0x4000;
	new_task->heap = kheap;
	new_task->context = (context_t) {.eip = (u32) entry, .eflags = 0x202, .cs =
	0x10, .esp = (u32) new_task->stack, .ebp = (u32) new_task->stack};
	
	task_t *p = root_task;
	while (p->next)
		p = p->next;
	p->next = new_task;
	
	return new_task;
}

void task_init(void)
{
	root_task = kmalloc(sizeof(task_t));
	memset(root_task, 0, sizeof(task_t));
	
	root_task->pid = pid_counter++;
	root_task->active = true;
	root_task->dir = kdir;
	root_task->stack = (u8 *) &kernel_stack_top;
	root_task->heap = kheap;
	root_task->context = (context_t) {.eax = 0xCAFEBABE, .eip = (u32) kernel_task_entry, .eflags =
	0x202, .cs = 0x10, .esp = (u32) &kernel_stack_top};
	
	current_task = root_task;
	
	task_new(test);
	task_new(test2);
	
	asm_load_kernel_task(&root_task->context);
	
	kassert(false, "Unreachable end of task_init reached");
}

task_t *task_switch(void)
{
	current_task = current_task->next;
	
	if (!current_task)
		current_task = root_task;
	
	return current_task;
}
