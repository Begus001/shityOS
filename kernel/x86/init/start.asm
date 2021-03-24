MB_MAGIC equ 0x1BADB002
MB_FLAGS equ 0x0
MB_CHKSM equ -(MB_MAGIC + MB_FLAGS)

section .multiboot

align 4
	dd MB_MAGIC
	dd MB_FLAGS
	dd MB_CHKSM


section .text

global _start
_start:
	mov esp, kernel_stack_top

	extern kernel_init
	call kernel_init

	cli
loop:
	hlt
	jmp loop


section .bss
	kernel_stack_bottom:
	resb 0x4000  ; 16 KiB
	kernel_stack_top:
