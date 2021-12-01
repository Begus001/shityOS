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

section .text
global asm_load_kernel_task
asm_load_kernel_task:
	mov eax, dword[esp+4] ; context.eax (start)
	mov esp, dword[eax+c.esp] ; context.esp
	mov dword[eax+c.ebp], esp
	mov ebp, dword[eax+c.ebp] ; context.ebp
	jmp dword[eax+c.eip] ; context.eip