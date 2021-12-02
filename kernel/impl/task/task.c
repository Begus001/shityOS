#include <stddef.h>
#include <io/serial.h>
#include <tty/tty.h>
#include <intr/intr.h>
#include <def/assert.h>
#include <io/keyboard.h>
#include <mm/mem.h>
#include <intr/gdt.h>

#include "task/task.h"

size_t task_user_off = offsetof(task_t, user);
size_t task_cntxt_off = offsetof(task_t, context);

size_t cntxt_eax_off = offsetof(context_t, eax);
size_t cntxt_ebx_off = offsetof(context_t, ebx);
size_t cntxt_ecx_off = offsetof(context_t, ecx);
size_t cntxt_edx_off = offsetof(context_t, edx);
size_t cntxt_esi_off = offsetof(context_t, esi);
size_t cntxt_edi_off = offsetof(context_t, edi);
size_t cntxt_ebp_off = offsetof(context_t, ebp);
size_t cntxt_esp_off = offsetof(context_t, esp);
size_t cntxt_eip_off = offsetof(context_t, eip);
size_t cntxt_cs_off = offsetof(context_t, cs);
size_t cntxt_eflags_off = offsetof(context_t, eflags);

size_t cntxt_num = sizeof(context_t) / 4;

static u32 pid_counter = 0;
static task_t *root_task = NULL;
task_t *task_current = NULL;

extern void asm_load_kernel_task(context_t *);

_Noreturn static void test2(void)
{
	while (1) {
		kprintf("2");
		for (int i = 0; i < 10000000; ++i) {
			asm volatile("nop");
		}
	}
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

task_t *task_new(void *entry, bool user)
{
	task_t *new_task = kmalloc(sizeof(task_t));
	memset(new_task, 0, sizeof(task_t));
	
	new_task->pid = pid_counter++;
	new_task->stack = kmalloc(0x4000) + 0x4000;
	if (!user) {
		new_task->user = false;
		new_task->context =
				(context_t) {.eip = (u32) entry, .eflags = 0x202, .cs = GDT_RING0_CODE, .ss =
				GDT_RING0_DATA, .esp = (u32) new_task->stack, .ebp = (u32) new_task->stack};
	} else {
		new_task->user = true;
		new_task->context =
				(context_t) {.eip = (u32) entry, .eflags = 0x202, .cs = GDT_RING3_CODE, .ss =
				GDT_RING3_DATA, .esp = (u32) new_task->stack, .ebp = (u32) new_task->stack};
	}
	
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
	root_task->stack = (u8 *) &kernel_stack_top;
	root_task->context = (context_t) {.eax = 0xCAFEBABE, .eip = (u32) kernel_task_entry, .eflags =
	0x202, .cs = 0x10, .esp = (u32) &kernel_stack_top};
	
	task_current = root_task;
	
	task_new(test, false);
	task_new(test2, false);
	
	asm_load_kernel_task(&root_task->context);
	
	kassert(false, "Unreachable end of task_init reached");
}

task_t *task_switch(void)
{
	task_current = task_current->next;
	
	if (!task_current)
		task_current = root_task;
	
	return task_current;
}
