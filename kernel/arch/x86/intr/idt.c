#include <sys/def/int.h>
#include <stdbool.h>
#include <intr/pic.h>
#include <tty/tty.h>
#include <tty/serial.h>
#include <gdt/gdt.h>
#include <intr/task.h>
#include <intr/pit.h>

#include <intr/intr.h>

#define IDT_MAX 256

#define IDT_TASK_GATE 0x5
#define IDT_INTR_GATE 0xE
#define IDT_TRAP_GATE 0xF

extern void intr_com_0(void);
extern void intr_com_1(void);
extern void intr_com_2(void);
extern void intr_com_3(void);
extern void intr_com_4(void);
extern void intr_com_5(void);
extern void intr_com_6(void);
extern void intr_com_7(void);
extern void intr_com_8(void);
extern void intr_com_9(void);
extern void intr_com_10(void);
extern void intr_com_11(void);
extern void intr_com_12(void);
extern void intr_com_13(void);
extern void intr_com_14(void);
extern void intr_com_15(void);
extern void intr_com_16(void);
extern void intr_com_17(void);
extern void intr_com_18(void);

extern void intr_com_32(void);
extern void intr_com_33(void);

extern void intr_com_48(void);

extern void intr_com_255(void);


struct idt_entry {
	u16 offset_lo;
	u16 cs_selector;
	u8 zero;
	u8 gate_type: 4;
	bool is_storage_segment: 1;
	u8 dpl: 2;
	bool present: 1;
	u16 offset_hi;
} __attribute__((packed));

struct idt_entry idt[IDT_MAX];
u64 idt2[IDT_MAX];

struct idt_pointer {
	u16 size;
	void *offset;
} __attribute__((packed)) p_idt = {
		.size = IDT_MAX * sizeof(struct idt_entry) - 1,
		.offset = idt,
};

static void set_entry(u8 i, void (*offset)(), u16 cs_selector, u8 gate_type, bool is_storage_segment, u8 dpl,
                      bool present)
{
	u32 handler = (u32) offset;
	idt[i].offset_lo = handler & 0xFFFF;
	idt[i].cs_selector = cs_selector;
	idt[i].zero = 0x0;
	idt[i].gate_type = gate_type & 0xF;
	idt[i].is_storage_segment = is_storage_segment & 0x1;
	idt[i].dpl = dpl & 0x3;
	idt[i].present = present & 0x1;
	idt[i].offset_hi = (handler >> 16) & 0xFFFF;
}

context_t *intr_com_handle(context_t *cntxt)
{
#ifdef DBG_INTR
	dbgprintf("Interrupt occurred\n");
#endif
	
	context_t *next_cntxt = cntxt;
	
	if (cntxt->intr <= 0x1F) {
		
		tty_set_color_tmp(COLOR_BLACK, COLOR_RED);
		
		kprintf("\nException %x (%d): %x\n", cntxt->intr, cntxt->intr, cntxt->err);
		kprintf("EAX: %x\nEBX: %x\nECX: %x\nEDX: %x\nESI: %x\nEDI: %x\nEBP: %x\nEIP: %x\nEFLAGS: %b\n",
		        cntxt->eax, cntxt->ebx, cntxt->ecx, cntxt->edx, cntxt->esi, cntxt->edi, cntxt->ebp,
		        cntxt->eip,
		        cntxt->eflags);
		
		dbgprintf("\nException %x (%d): %x\n", cntxt->intr, cntxt->intr, cntxt->err);
		dbgprintf("EAX: %x\nEBX: %x\nECX: %x\nEDX: %x\nESI: %x\nEDI: %x\nEBP: %x\nEIP: %x\nEFLAGS: %b\n",
		          cntxt->eax, cntxt->ebx, cntxt->ecx, cntxt->edx, cntxt->esi, cntxt->edi, cntxt->ebp,
		          cntxt->eip,
		          cntxt->eflags);
		
		tty_reset_color();
		
		kprintf("System halted");
		dbgprintf("System halted");
		
		intr_disable();
		__asm__ volatile("hlt");
	} else if (cntxt->intr == 32) {
#ifdef DBG_INTR
		dbgprintf("Clock interrupt\n");
#endif
	}
	
	return next_cntxt;
}

static void populate(void)
{
	set_entry(0, intr_com_0, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(1, intr_com_1, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(2, intr_com_2, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(3, intr_com_3, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(4, intr_com_4, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(5, intr_com_5, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(6, intr_com_6, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(7, intr_com_7, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(8, intr_com_8, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(9, intr_com_9, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(10, intr_com_10, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(11, intr_com_11, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(12, intr_com_12, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(13, intr_com_13, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(14, intr_com_14, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(15, intr_com_15, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(16, intr_com_16, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(17, intr_com_17, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(18, intr_com_18, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	
	set_entry(32, intr_com_32, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(33, intr_com_33, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	
	set_entry(48, intr_com_48, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);

#ifdef DBG_INTR
	dbgprintf("Interrupt descriptor table created\n");
#endif
}

static void idt_load(void)
{
	__asm__ volatile("lidt %0" : : "m"(p_idt));
#ifdef DBG_INTR
	dbgprintf("Interrupt descriptor table loaded\n");
#endif
}

void intr_init(void)
{
	pic_init();
	pit_init();
	pit_load_hz(65535);
	
	populate();
	idt_load();
}

void intr_enable(void)
{
	__asm__ volatile("sti");
#ifdef DBG_INTR
	dbgprintf("Interrupts enabled\n");
#endif
}

void intr_disable(void)
{
	__asm__ volatile("cli");
#ifdef DBG_INTR
	dbgprintf("Interrupts disabled\n");
#endif
}
