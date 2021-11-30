#ifndef KERNEL_KERNEL_H
#define KERNEL_KERNEL_H

#define KERNEL_VIRT_BASE 0xC0000000

extern const void kernel_end;
extern const void kernel_start;
extern const void kernel_stack_top;
extern const void kernel_stack_bottom;

#endif
