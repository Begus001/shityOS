
#include <sys/int.h>
#include <stdbool.h>

#define IDT_MAX 256

#define IDT_TASK_GATE 0x5
#define IDT_INTR_GATE 0x6
#define IDT_TRAP_GATE 0x7

struct idt_entry {
	u16 offset_lo;
	u16 selector;
	u8 zero;
	u8 gate_type: 4;
	bool is_storage_segment;
	u8 dpl: 2;
	bool present;
	u16 offset_hi;
} __attribute__((packed));

struct idt_entry IDT[IDT_MAX];
