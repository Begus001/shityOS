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
	unsigned int available: 4;
	unsigned int page_addr: 20;
} page_table_entry_t ;

typedef struct {
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

typedef struct {
	page_table_entry_t entries[PAGE_TABLE_MAX];
} page_table_t;

typedef struct {
	page_directory_entry_t entries[PAGE_DIR_MAX];
} page_directory_t;

page_directory_t *vmm_create_directory(void);
bool vmm_map_page(void *paddr, void *vaddr);
void vmm_activate_paging(void);
void *vmm_alloc(void);
void *vmm_alloc_at(void *vaddr);
bool vmm_free(void *addr);
bool vmm_change_directory(page_directory_t *dir);
bool vmm_init(void);

#endif
