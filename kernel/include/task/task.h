#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <def/int.h>

typedef struct
{
	u32 eax;
	u32 ebx;
	u32 ecx;
	u32 edx;
	u32 esi;
	u32 edi;
	u32 ebp;
	
	u32 intr;
	u32 err;
	
	u32 eip;
	u32 cs;
	u32 eflags;
	u32 esp;
	u32 ss;
}  __attribute__((packed)) context_t;

typedef struct task
{
	context_t   context;
	struct task *next;
	struct task *prev;
}                          task_t;

void task_init(void);

context_t *task_switch();

#endif
