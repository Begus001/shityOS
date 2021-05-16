#include <mm/mem.h>
#include <mm/pmm.h>

#include <mm/vmm.h>

#define PAGE_TABLE_ADDR_MAX 0x400000
#define PAGE_DIR_ADDR_MAX   0x100000000

#define PAGE_OFFSET(x)      ((x) & 0xFFF)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3FF)
#define PAGE_DIR_INDEX(x)   ((x) >> 22)
#define STRIP_12_LSB(x)     ((x) >> 12)
#define ADD_12_LSB(x)       ((x) << 12)

extern page_directory_t *kernel_page_dir;

static page_directory_t *current_dir = NULL;
static page_directory_t *kernel_dir;

static inline page_table_entry_t *pte_from_virt_addr(page_table_t *table, void *addr)
{
	if (!table)
		return NULL;
	return &table->entries[PAGE_TABLE_INDEX((u32) addr)];
	
}

static inline page_directory_entry_t *pde_from_virt_addr(page_directory_t *dir, void *addr)
{
	if (!dir)
		return NULL;
	return &dir->entries[PAGE_DIR_INDEX((u32) addr)];
}

static inline page_table_entry_t *pte_from_virt_addr_absolute(void *addr)
{
	page_directory_entry_t *dir_entry = pde_from_virt_addr(current_dir, addr);
	page_table_t *table = (page_table_t *) ADD_12_LSB(dir_entry->page_table_addr);
	return pte_from_virt_addr(table, addr);
}

static inline void inval_tlb_entry(void *vaddr)
{
	__asm__ volatile("invlpg %0"::"m"(vaddr));
}

static inline page_table_t *create_table(void)
{
	page_table_t *table = pmm_alloc_block();
	if (!table)
		return NULL;
	memset(table, 0, sizeof(page_table_t));
	return table;
}

page_directory_t *vmm_create_directory(void)
{
	page_directory_t *dir = pmm_alloc_block();
	if (!dir)
		return NULL;
	memset(dir, 0, sizeof(page_directory_t));
	dir->entries[1023].present = true;
	dir->entries[1023].writable = true;
	dir->entries[1023].page_table_addr = STRIP_12_LSB((u32) dir);
	return dir;
}

bool vmm_map_page_current(void *paddr, void *vaddr, bool user)
{
	return vmm_map_page(current_dir, paddr, vaddr, user);
}

bool vmm_map_page(page_directory_t *dir, void *paddr, void *vaddr, bool user)
{
	if (!dir)
		return false;
	
	page_directory_entry_t *dir_entry = pde_from_virt_addr(dir, vaddr);
	
	if (!dir_entry)
		return false;
	
	page_table_t *table;
	
	if (dir_entry->present) {
		table = (page_table_t *) (ADD_12_LSB((u32) dir_entry->page_table_addr));
	} else {
		if (!(table = create_table()))
			return false;
		
		dir_entry->present = true;
		dir_entry->writable = true;
		dir_entry->user_access = true;
		dir_entry->page_table_addr = STRIP_12_LSB((u32) table);
	}
	
	page_table_entry_t *table_entry = pte_from_virt_addr(table, vaddr);
	table_entry->present = true;
	table_entry->writable = true;
	table_entry->user_access = user;
	table_entry->page_addr = STRIP_12_LSB((u32) paddr);
	
	inval_tlb_entry(vaddr);
	
	return true;
}

bool vmm_change_directory(page_directory_t *dir)
{
	if (!dir)
		return false;
	current_dir = dir;
	__asm__ volatile("mov cr3, %0"::"r"(dir));
	return true;
}

void vmm_activate_paging(void)
{
	__asm__ volatile(
	"mov eax, cr0;"
	"or eax, 0x80000000;"
	"mov cr0, eax;"
	:);
}

void *vmm_alloc_at(void *vaddr, bool user)
{
	void *paddr = pmm_alloc_block();
	if (!paddr)
		return NULL;
	if (!vmm_map_page_current(paddr, vaddr, user))
		return NULL;
	return vaddr;
}

bool vmm_free(void *vaddr)
{
	if (!current_dir)
		return false;
	page_table_entry_t *page = pte_from_virt_addr_absolute(vaddr);
	pmm_free_block((void *) ADD_12_LSB((page->page_addr)));
	page->present = false;
	page->writable = false;
	page->page_addr = 0;
	return true;
}

bool vmm_init(void)
{
	kernel_dir = kernel_page_dir;
	
	current_dir = kernel_dir;
	
	return true;
}
