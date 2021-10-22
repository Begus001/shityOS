#ifndef KERNEL_KEYBOARD_H
#define KERNEL_KEYBOARD_H

#include <def/int.h>

void kb_handle_irq(void);

void kb_init(void);

u8 kb_get_key(void);

#endif
