#ifndef KERNEL_INTR_H
#define KERNEL_INTR_H

#include <def/int.h>

void intr_init(void);
void intr_enable(void);
void intr_disable(void);

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
}  __attribute__((packed)) err_context_t;

#endif
