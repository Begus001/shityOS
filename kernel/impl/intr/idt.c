#include <def/int.h>
#include <stdbool.h>
#include "intr/pic.h"
#include <tty/tty.h>
#include <tty/serial.h>
#include <gdt.h>
#include "intr/task.h"
#include "intr/pit.h"

#include <intr/intr.h>

#define IDT_MAX 256

#define IDT_TASK_GATE 0x5
#define IDT_INTR_GATE 0xE
#define IDT_TRAP_GATE 0xF

extern void exc_com_0x0(void);
extern void exc_com_0x1(void);
extern void exc_com_0x2(void);
extern void exc_com_0x3(void);
extern void exc_com_0x4(void);
extern void exc_com_0x5(void);
extern void exc_com_0x6(void);
extern void exc_com_0x7(void);
extern void exc_com_0x8(void);
extern void exc_com_0x9(void);
extern void exc_com_0xA(void);
extern void exc_com_0xB(void);
extern void exc_com_0xC(void);
extern void exc_com_0xD(void);
extern void exc_com_0xE(void);

extern void exc_com_0x10(void);
extern void exc_com_0x11(void);
extern void exc_com_0x12(void);
extern void exc_com_0x13(void);
extern void exc_com_0x14(void);

extern void exc_com_0x1E(void);

extern void clock_handler(void);
extern void keyboard_handler(void);

struct idt_entry {
	u16 offset_lo;
	u16 cs_selector;
	u8 zero;
	unsigned int gate_type: 4;
	bool is_storage_segment: 1;
	unsigned int dpl: 2;
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

static void
set_entry(u8 i, void (*offset)(), u16 cs_selector, u8 gate_type, bool is_storage_segment, u8 dpl,
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

static void populate(void)
{
	set_entry(0x0, exc_com_0x0, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0x1, exc_com_0x1, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0x2, exc_com_0x2, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0x3, exc_com_0x3, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0x4, exc_com_0x4, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0x5, exc_com_0x5, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0x6, exc_com_0x6, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0x7, exc_com_0x7, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0x8, exc_com_0x8, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0x9, exc_com_0x9, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0xA, exc_com_0xA, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0xB, exc_com_0xB, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0xC, exc_com_0xC, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0xD, exc_com_0xD, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0xE, exc_com_0xE, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);

	set_entry(0x10, exc_com_0x10, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0x11, exc_com_0x11, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0x12, exc_com_0x12, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0x13, exc_com_0x13, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0x14, exc_com_0x14, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);

	set_entry(0x1E, exc_com_0x1E, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);

	set_entry(0x20, clock_handler, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
	set_entry(0x21, keyboard_handler, GDT_RING0_CODE, IDT_INTR_GATE, false, RING0, true);
}

static void idt_load(void)
{
	__asm__ volatile("lidt %0" : : "m"(p_idt));
}

void intr_init(void)
{
	pic_init();
	pit_init();
	pit_load_hz(2000);

	populate();
	idt_load();
}

void intr_enable(void)
{
	__asm__ volatile("sti");
}

void intr_disable(void)
{
	__asm__ volatile("cli");
}
