%define cntxt.ebp 0x18
%define cntxt.eip 0x1C
%define cntxt.esp 0x28

section .text
global asm_load_kernel_task
asm_load_kernel_task:
	mov eax, dword[esp+0x4] ; context.eax (start)
	mov esp, dword[eax+cntxt.esp] ; context.esp
	mov ebp, dword[eax+cntxt.ebp] ; context.ebp
	jmp dword[eax+cntxt.eip] ; context.eip