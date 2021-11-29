section .text

global clock_handler
extern intr_clock_handle
clock_handler:
	call intr_clock_handle
	iret


global keyboard_handler
extern kb_handle_irq
extern kprintf
keyboard_handler:
	call kb_handle_irq
	iret