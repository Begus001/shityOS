#ifndef KERNEL_GDT_H
#define KERNEL_GDT_H

#define RING0 0x0
#define RING1 0x1
#define RING2 0x2
#define RING3 0x3

#define GDT_RING0_CODE 0x8
#define GDT_RING0_DATA 0x10
#define GDT_RING3_CODE 0x18
#define GDT_RING3_DATA 0x20
#define GDT_TSS 0x24

void gdt_init(void);

#endif
