#include <tty/serial.h>
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

static page_directory_t *current_dir = NULL;
static page_directory_t *kernel_dir;

bool vmm_alloc_page(page_table_entry_t *entry)
{
	void *phys = pmm_alloc_block();
	if (!phys)
		return false;
	
	entry->page_addr = STRIP_12_LSB((u32) phys);
	entry->present = true;
	
	return true;
}

void vmm_free_page(page_table_entry_t *entry)
{
	entry->present = false;
	
	void *phys = (void *) ADD_12_LSB(entry->page_addr);
	if (phys)
		pmm_free_block(phys);
}

static inline page_table_entry_t *pte_from_virt_addr(page_table_t *table, void *addr)
{
	if (table)
		return &table->entries[PAGE_TABLE_INDEX((u32) addr)];
	return NULL;
}

static inline page_directory_entry_t *pde_from_virt_addr(page_directory_t *dir, void *addr)
{
	if (dir)
		return &dir->entries[PAGE_DIR_INDEX((u32) addr)];
	return NULL;
}

static inline void inval_tlb_entry(void *vaddr)
{
	__asm__ volatile("invlpg %0"::"m"(vaddr));
}

bool vmm_map_page(void *paddr, void *vaddr)
{
#pragma region DBG_PRINT
#ifdef DBG_VMM
	dbgprintf("vmm_map_page: mapping %x to %x\n", (u32) paddr, (u32) vaddr);
#endif
#pragma endregion
	page_directory_entry_t *dir_entry = pde_from_virt_addr(current_dir, vaddr);
	
	if (!dir_entry) {
#pragma region DBG_PRINT
#ifdef DBG_VMM
		dbgprintf("vmm_map_page: current_dir NULL\n");
#endif
#pragma endregion
		return false;
	}
	
	page_table_t *table;
	
	if (dir_entry->present) {
#pragma region DBG_PRINT
#ifdef DBG_VMM
		dbgprintf("vmm_map_page: dir_entry present\n");
#endif
#pragma endregion
		table = (page_table_t *) (ADD_12_LSB((u32) dir_entry->page_table_addr));
	} else {
#pragma region DBG_PRINT
#ifdef DBG_VMM
		dbgprintf("vmm_map_page: dir_entry not present, allocating new page table\n");
#endif
#pragma endregion
		table = kamalloc(sizeof(page_table_t));
		if (!table)
			return false;
		
		memset(table, 0, sizeof(page_table_t));
		
		dir_entry->present = true;
		dir_entry->writable = true;
		dir_entry->page_table_addr = STRIP_12_LSB((u32) table);
	}
	
	page_table_entry_t *table_entry = pte_from_virt_addr(table, vaddr);
	table_entry->present = true;
	table_entry->writable = true;
	table_entry->page_addr = STRIP_12_LSB((u32) paddr);
	
	inval_tlb_entry(vaddr);

#pragma region DBG_PRINT
#ifdef DBG_VMM
	dbgprintf("vmm_map_page: done\n");
#endif
#pragma endregion
	
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

void vmm_init(void)
{
	kernel_dir = kamalloc(sizeof(page_directory_t));
	memset(kernel_dir, 0, sizeof(page_directory_t));
	vmm_change_directory(kernel_dir);
	
	for (int i = 0; (u32) i < 1024 * 4096; i += 4096) {
		vmm_map_page((void *) i, (void *) i);
	}
	
	vmm_activate_paging();
}
