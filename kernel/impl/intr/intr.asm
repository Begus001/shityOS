section .text

extern task_switch
global clock_handler
clock_handler:
	push ebp
	push edi
	push esi
	push edx
	push ecx
	push ebx
	push eax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	push esp
	call task_switch
	mov esp, eax

	mov ax, 0x23
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	pop eax
	pop ebx
	pop ecx
	pop edx
	pop esi
	pop edi
	pop ebp

	iret

global system_call
extern intr_syscall
system_call:
    push ebx
	push eax
	call intr_syscall
	add esp, 8

	iret

global keyboard_handler
extern kb_handle_irq
extern kprintf
keyboard_handler:
	call kb_handle_irq
	iret