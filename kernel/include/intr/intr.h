#ifndef KERNEL_INTR_H
#define KERNEL_INTR_H

#include <def/int.h>

void intr_init(void);
void intr_enable(void);
void intr_disable(void);

#endif
