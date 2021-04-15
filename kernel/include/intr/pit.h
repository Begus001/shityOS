#ifndef KERNEL_PIT_Htwo
#define KERNEL_PIT_H

#include <stdbool.h>
#include <def/int.h>
#include <util.h>

#define PIT_FREQ 1193181

#define PIT_CH0 0x40
#define PIT_CMD  0x43

static inline void pit_init()
{
	out8(PIT_CMD, 0x34);
}

static inline void pit_load_hz(u32 hz)
{
	u16 val = PIT_FREQ / hz;
	out8(PIT_CH0, val & 0xFF);
	out8(PIT_CH0, val >> 8);
}

static inline void pit_load_val(u16 val)
{
	out8(PIT_CH0, val & 0xFF);
	out8(PIT_CH0, val >> 8);
}

#endif
