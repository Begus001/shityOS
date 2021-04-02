%macro intr_com_stub_macro 1
global intr_com_%1
intr_com_%1:
	push 0
	push %1
	jmp intr_com_handler
%endmacro

%macro intr_com_err_stub_macro 1
global intr_com_%1
intr_com_%1:
	push %1
	jmp intr_com_handler
%endmacro

intr_com_stub_macro 0
intr_com_stub_macro 1
intr_com_stub_macro 2
intr_com_stub_macro 3
intr_com_stub_macro 4
intr_com_stub_macro 5
intr_com_stub_macro 6
intr_com_stub_macro 7
intr_com_err_stub_macro 8
intr_com_stub_macro 9
intr_com_err_stub_macro 10
intr_com_err_stub_macro 11
intr_com_err_stub_macro 12
intr_com_err_stub_macro 13
intr_com_err_stub_macro 14
intr_com_stub_macro 15
intr_com_stub_macro 16
intr_com_err_stub_macro 17
intr_com_stub_macro 18

intr_com_stub_macro 32
intr_com_stub_macro 33

intr_com_stub_macro 48

extern intr_com_handle
intr_com_handler:
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

	push esp
	call intr_com_handle
	mov esp, eax

	pop eax
	pop ebx
	pop ecx
	pop edx
	pop esi
	pop edi
	pop ebp

	add esp, 8

	iret
