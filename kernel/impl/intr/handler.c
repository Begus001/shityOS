#include <tty/tty.h>
#include <io/serial.h>
#include <task/task.h>
#include <io/keyboard.h>

#include <intr/intr.h>
#include <def/assert.h>

void intr_exc_com_handle(exc_context_t *cntxt)
{
	tty_set_color(COLOR_BLACK, COLOR_RED);
	
	kprintf("\nException %x (%d): %x\n", cntxt->intr, cntxt->intr, cntxt->err);
	kprintf("EAX: %x\nEBX: %x\nECX: %x\nEDX: %x\nESI: %x\nEDI: %x\nEBP: %x\nEIP: %x\nEFLAGS: %b\n",
	        cntxt->eax, cntxt->ebx, cntxt->ecx, cntxt->edx, cntxt->esi, cntxt->edi, cntxt->ebp,
	        cntxt->eip, cntxt->eflags);
	
	dbgprintf("\nException %x (%d): %x\n", cntxt->intr, cntxt->intr, cntxt->err);
	dbgprintf(
			"EAX: %x\nEBX: %x\nECX: %x\nEDX: %x\nESI: %x\nEDI: %x\nEBP: %x\nEIP: %x\nEFLAGS: %b\n",
			cntxt->eax, cntxt->ebx, cntxt->ecx, cntxt->edx, cntxt->esi, cntxt->edi, cntxt->ebp,
			cntxt->eip, cntxt->eflags);
	
	kprintf("\nSystem halted\n");
	dbgprintf("\nSystem halted\n");
	
	intr_disable();
	asm volatile("hlt;"
	             "jmp $;"::);
}

void intr_syscall(u32 eax, u32 ebx)
{
	switch (eax) {
		case 0:
			kprintf("%s", ebx);
			break;
			
		default:
			tty_set_color_tmp(COLOR_LIGHT_MAGENTA, COLOR_BLACK);
			kprintf("Invalid syscall called by %d\n", task_current->pid);
			dbgprintf("Invalid syscall called by %d\n", task_current->pid);
			tty_reset_color();
			break;
	}
}
