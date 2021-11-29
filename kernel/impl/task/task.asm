section .text
global asm_load_kernel_task
asm_load_kernel_task:
	mov eax, dword[esp+0x4] ; context.eax (start)
	mov esp, dword[eax+0x30] ; context.esp
	mov ebp, dword[eax+0x18] ; context.ebp
	jmp dword[eax+0x24] ; context.eip