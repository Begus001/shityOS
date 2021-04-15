#ifndef KERNEL_MULTIBOOT_H
#define KERNEL_MULTIBOOT_H

#include <def/int.h>

typedef struct {
	u32 flags;
	u32 mem_lower;
	u32 mem_upper;
	u32 boot_device;
	u32 cmdline;
	u32 mods_count;
	void *mods_addr;
	u32 syms[4];
	u32 mmap_length;
	void *mmap_addr;
	u32 drives_length;
	void *drives_addr;
	u32 config_table;
	u32 boot_loader_name;
} __attribute__((packed)) multiboot_info_t;

typedef struct {
	u32 size;
	u64 base_addr;
	u64 length;
	u32 type;
} __attribute__((packed)) multiboot_mmap_t;

#endif
