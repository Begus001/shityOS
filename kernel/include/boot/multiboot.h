#ifndef KERNEL_MULTIBOOT_H
#define KERNEL_MULTIBOOT_H

#include <def/int.h>

typedef struct multiboot_aout_symbol_table
{
	u32 tabsize;
	u32 strsize;
	u32 addr;
	u32 reserved;
} multiboot_aout_symbol_table_t;

typedef struct multiboot_elf_section_header_table
{
	u32 num;
	u32 size;
	u32 addr;
	u32 shndx;
} multiboot_elf_section_header_table_t;

typedef struct
{
	u32 flags;
	u32 mem_lower;
	u32 mem_upper;
	u32 boot_device;
	u32 cmdline;
	u32 mods_count;
	u32 mods_addr;
	union
	{
		multiboot_aout_symbol_table_t aout_sym;
		multiboot_elf_section_header_table_t elf_sec;
	} u;
	u32 mmap_length;
	u32 mmap_addr;
	u32 drives_length;
	u32 drives_addr;
	u32 config_table;
	u32 boot_loader_name;
	u32 apm_table;
	u32 vbe_control_info;
	u32 vbe_mode_info;
	u16 vbe_mode;
	u16 vbe_interface_seg;
	u16 vbe_interface_off;
	u16 vbe_interface_len;
	u64 framebuffer_addr;
	u32 framebuffer_pitch;
	u32 framebuffer_width;
	u32 framebuffer_height;
	u8 framebuffer_bpp;
	u8 framebuffer_type;
	union
	{
		struct
		{
			u32 framebuffer_palette_addr;
			u16 framebuffer_palette_num_colors;
		};
		struct
		{
			u8 framebuffer_red_field_position;
			u8 framebuffer_red_mask_size;
			u8 framebuffer_green_field_position;
			u8 framebuffer_green_mask_size;
			u8 framebuffer_blue_field_position;
			u8 framebuffer_blue_mask_size;
		};
	};
} multiboot_info_t;

typedef struct
{
	u32 size;
	u64 base_addr;
	u64 length;
	u32 type;
} multiboot_mmap_t;

typedef struct
{
	u32 mod_start;
	u32 mod_end;
	char *string;
	u32 reserved;
} multiboot_mod_t;

#endif
