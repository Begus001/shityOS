MB_MAGIC: equ 0x1BADB002
MB_FLAGS: equ 0x3  ; Page aligned modules, give memory map info
MB_CHKSM: equ -(MB_MAGIC + MB_FLAGS)

section .multiboot

align 4
	dd MB_MAGIC
	dd MB_FLAGS
	dd MB_CHKSM


section .data

KERNEL_VIRT_BASE: equ 0xC0000000
KERNEL_PD_INDEX:  equ (KERNEL_VIRT_BASE >> 22)

align 0x1000
global kernel_page_dir
kernel_page_dir:
	dd (kernel_page_table_zero - KERNEL_VIRT_BASE + 0x00000083)
	times (KERNEL_PD_INDEX - 1) dd 0
	dd (kernel_page_table_hh - KERNEL_VIRT_BASE + 0x00000083)
	times (1024 - KERNEL_PD_INDEX - 1) dd 0

global kernel_page_table_zero
kernel_page_table_zero:
	times 1024 dd 0

kernel_page_table_hh:
	times 1024 dd 0

paging_setup_switch:
	db 0


section .text

init_paging:  ; Identity map first 4 MiB, map 4 MiB at 0xC0000000 to 0x0 and enable paging
	xor eax, eax
	xor edx, edx
.map:
		mov ecx, edx
		or ecx, 3
		cmp byte [paging_setup_switch - KERNEL_VIRT_BASE], 0
		je .identity_map

		mov [(kernel_page_table_hh - KERNEL_VIRT_BASE) + eax * 4], ecx
		jmp .increment_and_compare

.identity_map:
		mov [(kernel_page_table_zero - KERNEL_VIRT_BASE) + eax * 4], ecx

.increment_and_compare:
		add edx, 0x1000
		inc eax
		cmp eax, 1024
		jne .map

	cmp byte [paging_setup_switch - KERNEL_VIRT_BASE], 0
	jne .enable_paging
	mov byte [paging_setup_switch - KERNEL_VIRT_BASE], 1
	jmp init_paging

.enable_paging:
	xor eax, eax
	xor edx, edx

	mov eax, (kernel_page_dir - KERNEL_VIRT_BASE)
	mov cr3, eax

	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax

	lea eax, [.higher_half]
	jmp eax
.higher_half:

	xor eax, eax
.unmap_first_four_megs:
	mov dword [kernel_page_table_zero + eax * 4], 0
	inc eax
	cmp eax, 1024
	jne .unmap_first_four_megs
	jmp _start.after_paging_init

global _start
_start:
	jmp init_paging

.after_paging_init:
	mov esp, kernel_stack_top

	add ebx, KERNEL_VIRT_BASE
	push ebx
	extern init
	call init

.end:
	cli
	hlt
	jmp .end


section .bss
kernel_stack_bottom:
	resb 0x4000  ; 16 KiB
kernel_stack_top:
