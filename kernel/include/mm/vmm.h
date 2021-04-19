#ifndef KERNEL_VMM_H
#define KERNEL_VMM_H

#include <def/int.h>
#include <stdbool.h>

#define PAGE_TABLE_MAX 1024
#define PAGE_DIR_MAX   1024
#define PAGE_MAX       4096

typedef struct {
	bool present: 1;
	bool writable: 1;
	bool user_access: 1;
	bool write_through: 1;
	bool cache_disable: 1;
	bool accessed: 1;
	bool dirty: 1;
	bool reserved: 1;
	u8 available: 4;
	u32 page_addr: 20;
} __attribute__((packed)) page_table_entry_t ;

typedef struct {
	bool present: 1;
	bool writable: 1;
	bool user_access: 1;
	bool write_through: 1;
	bool cache_disable: 1;
	bool accessed: 1;
	bool ignored: 1;
	bool large_pages: 1;
	u8 available: 4;
	u32 page_table_addr: 20;
} __attribute__((packed)) page_directory_entry_t;

typedef struct {
	page_table_entry_t entries[PAGE_TABLE_MAX];
} page_table_t;

typedef struct {
	page_directory_entry_t entries[PAGE_DIR_MAX];
} page_directory_t;

bool vmm_alloc_page(page_table_entry_t *entry);
void vmm_free_page(page_table_entry_t *entry);
bool vmm_map_page(void *paddr, void *vaddr);
bool vmm_change_directory(page_directory_t *dir);
void vmm_activate_paging(void);
void vmm_init(void);

#endif
