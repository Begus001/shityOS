section .data

tmpesp: dd 0
tmpeip: dd 0


section .text

%define task.cntxt 0x5

%define c.eax 0x0
%define c.ebx 0x4
%define c.ecx 0x8
%define c.edx 0xC
%define c.esi 0x10
%define c.edi 0x14
%define c.ebp 0x18
%define c.esp 0x1C
%define c.err 0x20
%define c.eip 0x24
%define c.cs 0x28
%define c.eflags 0x2C

global clock_handler
extern task_switch
extern current_task
clock_handler:
	cli
	push 0
	push esp
	push ebp
	push edi
	push esi
	push edx
	push ecx
	push ebx
	push eax

	mov edi, [current_task]
	add edi, task.cntxt
	xor ecx, ecx
.save_state:
	mov esi, [esp+ecx*4]
	mov [edi+ecx*4], esi
	inc ecx
	cmp ecx, 12
	jne .save_state

	push esp
	call task_switch
.load_state:
	mov esp, eax
	add esp, task.cntxt

	mov esi, [current_task]
	add esi, task.cntxt
	mov edi, [esi+c.esp]
	mov dword[tmpesp], edi
	mov edi, [esi+c.eip]
	mov dword[tmpeip], edi

	pop eax
	pop ebx
	pop ecx
	pop edx
	pop esi
	pop edi
	pop ebp

	sti
	mov esp, [tmpesp]
	add esp, 0x10
	jmp [tmpeip]

global keyboard_handler
extern kb_handle_irq
extern kprintf
keyboard_handler:
	call kb_handle_irq
	iret