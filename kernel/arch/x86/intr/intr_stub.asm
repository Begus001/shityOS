%macro exc_com_stub_macro 1
global exc_com_%1
exc_com_%1:
	push 0
	push %1
	jmp exc_com_handler
%endmacro

%macro exc_com_err_stub_macro 1
global exc_com_%1
exc_com_%1:
	push %1
	jmp exc_com_handler
%endmacro

exc_com_stub_macro 0x0
exc_com_stub_macro 0x1
exc_com_stub_macro 0x2
exc_com_stub_macro 0x3
exc_com_stub_macro 0x4
exc_com_stub_macro 0x5
exc_com_stub_macro 0x6
exc_com_stub_macro 0x7
exc_com_err_stub_macro 0x8
exc_com_stub_macro 0x9
exc_com_err_stub_macro 0xA
exc_com_err_stub_macro 0xB
exc_com_err_stub_macro 0xC
exc_com_err_stub_macro 0xD
exc_com_err_stub_macro 0xE

exc_com_stub_macro 0x10
exc_com_err_stub_macro 0x11
exc_com_stub_macro 0x12
exc_com_stub_macro 0x13
exc_com_stub_macro 0x14

exc_com_err_stub_macro 0x1E

extern intr_exc_com_handle
exc_com_handler:
	push ebp
	push edi
	push esi
	push edx
	push ecx
	push ebx
	push eax

	mov ax, 0x10  ; Switch to kernel data segment
	mov ds, ax
	mov es, ax

	push esp
	call intr_exc_com_handle

	cli
	hlt
	jmp $

global clock_handler
extern intr_clock_handle
clock_handler:
	call intr_clock_handle
	iret
