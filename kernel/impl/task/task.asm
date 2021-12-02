%define cntxt.eax 0x0
%define cntxt.ebx 0x4
%define cntxt.ecx 0x8
%define cntxt.edx 0xC
%define cntxt.esi 0x10
%define cntxt.edi 0x14
%define cntxt.ebp 0x18
%define cntxt.eip 0x1C
%define cntxt.cs 0x20
%define cntxt.eflags 0x24
%define cntxt.esp 0x28
%define cntxt.ss 0x2C

%define cntxt.save.counter.num 10

extern task_current
extern task_cntxt_off
extern task_user_off

section .data
eip_buf: dd 0
esp_buf: dd 0

section .text

global asm_load_kernel_task
asm_load_kernel_task:
	mov eax, [esp+4] ; context.eax (start)
	mov esp, [eax+cntxt.esp] ; context.esp
	mov [eax+cntxt.ebp], esp
	mov ebp, [eax+cntxt.ebp] ; context.ebp
	jmp [eax+cntxt.eip] ; context.eip


check_current_ring:
	enter 0, 0

	mov esi, [task_current]
	add esi, [task_user_off]
	mov al, [esi]

	leave
	ret

save_state_ring0:
	enter 0, 0

	mov edi, [task_current]
	add edi, [task_cntxt_off]

	add esp, 8

	pop eax
	add eax, 0x28
	mov [edi+cntxt.esp], eax

	xor ecx, ecx
.loop:
	mov esi, [esp+ecx*4]
	mov [edi+ecx*4], esi
	inc ecx
	cmp ecx, cntxt.save.counter.num
	jne .loop

	sub esp, 0xC
	leave
	ret

load_state_and_return:
	call check_current_ring
	mov esi, [task_current]
	add esi, [task_cntxt_off]

	mov eax, [esi+cntxt.esp]
	mov dword[esp_buf], eax
	mov eax, [esi+cntxt.eip]
	mov dword[eip_buf], eax

	mov esp, esi
	pop eax
	pop ebx
	pop ecx
	pop edx
	pop esi
	pop edi
	pop ebp

	mov esp, [esp_buf]
	sti
	jmp [eip_buf]
