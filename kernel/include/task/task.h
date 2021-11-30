#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <def/int.h>
#include <mm/vmm.h>
#include <mm/mem.h>

typedef struct
{
	u32 eax;
	u32 ebx;
	u32 ecx;
	u32 edx;
	u32 esi;
	u32 edi;
	u32 ebp;
	
	u32 eip;
	u32 cs;
	u32 eflags;
	u32 esp;
	u32 ss;
}  __attribute__((packed)) context_t;

typedef struct task
{
	u32 pid;
	bool active;
	context_t context;
	page_directory_t *dir;
	u8 *stack;
	heap_t *heap;
	struct task *next;
} task_t;

void task_init(void);

context_t *task_switch();

#endif
