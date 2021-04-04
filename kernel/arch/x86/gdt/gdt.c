#include <sys/def/int.h>
#include <tty/serial.h>
#include <stdbool.h>

#include "gdt.h"

#define GDT_TYPE_READ_WRITE 0x02     //  code segment: read, data segment: write
#define GDT_TYPE_DIRECTION  0x04      /*  code segment: 1: can be executed by lower dpl, 0: can only
										be executed by current dpl
										data segment: 0: segment grows upwards, 1: segment grows
										downwards */
#define GDT_TYPE_IS_CODE_SEGMENT    0x08     // 1: code segment, 0: data segment
#define GDT_TYPE_IS_DATA_SEGMENT    0x00

#define GDT_MAX 5

struct gdt_entry {
	u16 limit_lo;
	u16 base_lo;
	u8 base_hi;
	u8 type: 4;
	bool segment: 1;
	u8 dpl: 2;
	bool present: 1;
	u8 limit_hi: 4;
	bool available: 1;
	bool long_mode: 1;
	bool protected_mode: 1;
	bool granularity: 1;
	u8 base_hi2;
} __attribute__((packed));

struct gdt_entry gdt[GDT_MAX];

struct gdt_pointer {
	u16 size;
	void *offset;
} __attribute__((packed)) p_gdt = {
		.size = GDT_MAX * sizeof(struct gdt_entry) - 1,
		.offset = gdt,
};

static void set_entry(u8 i, u32 base, u32 limit, u8 type, bool not_tss, u8 ring, bool present,
                      bool available, bool long_mode, bool protected_mode, bool granularity_4K)
{
	gdt[i].limit_lo = limit & 0xFFFF;
	gdt[i].base_lo = base & 0xFFFF;
	gdt[i].base_hi = (base >> 16) & 0xFF;
	gdt[i].type = type & 0xF;
	gdt[i].segment = not_tss & 0x1;
	gdt[i].dpl = ring & 0x3;
	gdt[i].present = present & 0x1;
	gdt[i].limit_hi = (limit >> 16) & 0xF;
	gdt[i].available = available & 0x1;
	gdt[i].long_mode = long_mode & 0x1;
	gdt[i].protected_mode = protected_mode & 0x1;
	gdt[i].granularity = granularity_4K & 0x1;
	gdt[i].base_hi2 = (base >> 24) & 0xFF;
}

static void gdt_load()
{
	__asm__ volatile("lgdt %0" : : "m"(p_gdt));
	__asm__ volatile(
	"mov ax, 0x10;"
	"mov ds, ax;"
	"mov es, ax;"
	"mov fs, ax;"
	"mov gs, ax;"
	"mov ss, ax;"
	"jmp 0x8:flush;"
	"flush:"
	);
	
	dbgprintf("Global descriptor table loaded and flushed\n");
}

void gdt_init()
{
	set_entry(0, 0, 0, 0, false, 0, false, false, false, false, false);
	set_entry(1, 0, 0xFFFFF, GDT_TYPE_READ_WRITE | GDT_TYPE_IS_CODE_SEGMENT, true, RING0,
	          true, true, false, true, true);
	set_entry(2, 0, 0xFFFFF, GDT_TYPE_READ_WRITE | GDT_TYPE_IS_DATA_SEGMENT, true, RING0,
	          true, true, false, true, true);
	set_entry(3, 0, 0xFFFFF, GDT_TYPE_READ_WRITE | GDT_TYPE_IS_CODE_SEGMENT, true, RING3,
	          true, true, false, true, true);
	set_entry(4, 0, 0xFFFFF, GDT_TYPE_READ_WRITE | GDT_TYPE_IS_DATA_SEGMENT, true, RING3,
	          true, true, false, true, true);
	
	dbgprintf("Global descriptor table created\n");
	
	gdt_load();
}
