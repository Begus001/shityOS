#ifndef KERNEL_PIC_H
#define KERNEL_PIC_H

#include <io/util.h>

#define PIC_CMD_ADDR    0x20
#define PIC_DAT_ADDR    0x21
#define PIC_SL_CMD_ADDR 0xA0
#define PIC_SL_DAT_ADDR 0xA1

#define PIC_EOI 0x20

void pic_init(void);

static inline void pic_eoi(void)
{
	out8(PIC_CMD_ADDR, PIC_EOI);
}

static inline void pic_slave_eoi(void)
{
	out8(PIC_SL_CMD_ADDR, PIC_EOI);
}

#endif
