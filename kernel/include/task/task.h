#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <stddef.h>
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
}  __attribute__((packed)) exc_context_t;

typedef struct task
{
	u32 pid;
	bool user;
	context_t context;
	u8 *stack;
	struct task *next;
} task_t;

void task_init(void);

task_t *task_switch(void);

#endif
