%include "../kernel/impl/task/task.asm"

section .text


global clock_handler
extern task_switch
clock_handler:
    push ebp
    push edi
    push esi  ; save current regs
    push edx
    push ecx
    push ebx
	push eax

	call check_current_ring
	cmp al, 0
	jne .ring3
.ring0:
	push esp
	call save_state_ring0  ; write stack contents to current task's context
	add esp, 0x2C  ; remove saved values from stack
	call task_switch  ; set current task to next task
	jmp load_state_and_return  ; load current task's context and jump to its ip
.ring3:
	iret

global keyboard_handler
extern kb_handle_irq
extern kprintf
keyboard_handler:
	call kb_handle_irq
	iret