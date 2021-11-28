#ifndef KERNEL_VMM_H
#define KERNEL_VMM_H

#include <def/int.h>
#include <stddef.h>
#include <stdbool.h>

#define PAGE_TABLE_MAX 1024
#define PAGE_DIR_MAX   1024
#define PAGE_MAX       4096

#define EXTRA_MEM 0xE0000000

typedef struct
{
	bool present: 1;
	bool writable: 1;
	bool user_access: 1;
	bool write_through: 1;
	bool cache_disable: 1;
	bool accessed: 1;
	bool dirty: 1;
	bool reserved: 1;
	unsigned int available: 4;
	unsigned int page_addr: 20;
} page_table_entry_t;

typedef struct
{
	bool present: 1;
	bool writable: 1;
	bool user_access: 1;
	bool write_through: 1;
	bool cache_disable: 1;
	bool accessed: 1;
	bool ignored: 1;
	bool large_pages: 1;
	unsigned int available: 4;
	unsigned int page_table_addr: 20;
} page_directory_entry_t;

typedef struct
{
	page_table_entry_t entries[PAGE_TABLE_MAX];
} page_table_t;

typedef struct
{
	page_directory_entry_t entries[PAGE_DIR_MAX];
} page_directory_t;

bool vmm_map_page_current(void *paddr, void *vaddr, bool user);
bool vmm_map_page(page_directory_t *dir, void *paddr, void *vaddr, bool user);
void vmm_activate_paging(void);
void *vmm_alloc_at_ret_phys(void *vaddr, bool user);
void *vmm_alloc_size_at_ret_phys(void *vaddr, bool user, size_t size);
void *vmm_alloc_at(void *vaddr, bool user);
void *vmm_alloc_size_at(void *vaddr, bool user, size_t size);
bool vmm_free(void *addr);
void *vmm_create_directory(void);
bool vmm_change_directory(page_directory_t *dir);

void vmm_print_kernel_dir(void);
void vmm_print_table_kernel_dir(unsigned int num);

bool vmm_init(void);

#endif
