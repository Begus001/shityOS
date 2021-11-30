#include <mm/mem.h>
#include <mm/pmm.h>
#include <io/serial.h>
#include <def/assert.h>
#include <def/kernel.h>

#include <mm/vmm.h>

#define PAGE_TABLE_ADDR_MAX 0x400000
#define PAGE_DIR_ADDR_MAX   0x100000000

#define PAGE_OFFSET(x)      ((x) & 0xFFF)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3FF)
#define PAGE_DIR_INDEX(x)   ((x) >> 22)
#define STRIP_12_LSB(x)     ((x) >> 12)
#define ADD_12_LSB(x)       ((x) << 12)

extern page_directory_t kernel_page_dir;
extern const void kernel_end;

static page_directory_t *current_dir = NULL;

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

static inline page_table_t *pt_from_virt_addr_recursive(void *vaddr)
{
	return (page_table_t *) (0xFFC00000 + (PAGE_DIR_INDEX((u32) vaddr) << 12));
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
	
	return table;
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
	
	page_table_t *table = pt_from_virt_addr_recursive(vaddr);
	
	if (!dir_entry->present) {
		void *table_phys_addr = create_table();
		if (!table_phys_addr)
			return false;
		
		dir_entry->present = true;
		dir_entry->writable = true;
		dir_entry->user_access = true;
		dir_entry->page_table_addr = STRIP_12_LSB((u32) table_phys_addr);
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

void *vmm_alloc_at_ret_phys(void *vaddr, bool user)
{
	void *paddr = pmm_alloc_block();
	if (!paddr)
		return NULL;
	if (!vmm_map_page_current(paddr, vaddr, user))
		return NULL;
	return paddr;
}

void *vmm_alloc_size_at_ret_phys(void *vaddr, bool user, size_t size)
{
	if (size % 0x1000 || (u32) vaddr % 0x1000)
		return NULL;
	
	if (size <= 0x1000)
		return vmm_alloc_at_ret_phys(vaddr, user);
	
	void *current_addr = vaddr;
	
	while (size > 0) {
		vmm_alloc_at_ret_phys(current_addr, user);
		current_addr += 0x1000;
		size -= 0x1000;
	}
	return vmm_alloc_at_ret_phys(vaddr, user);
}

void *vmm_alloc_at(void *vaddr, bool user)
{
	vmm_alloc_at_ret_phys(vaddr, user);
	return vaddr;
}

void *vmm_alloc_size_at(void *vaddr, bool user, size_t size)
{
	vmm_alloc_size_at_ret_phys(vaddr, user, size);
	return vaddr;
}

bool vmm_unmap(void *vaddr)
{
	if (!current_dir)
		return false;
	page_table_t *table = pt_from_virt_addr_recursive(vaddr);
	table->entries[PAGE_TABLE_INDEX((u32) vaddr)].present = false;
	table->entries[PAGE_TABLE_INDEX((u32) vaddr)].writable = false;
	table->entries[PAGE_TABLE_INDEX((u32) vaddr)].page_addr = 0;
	return true;
}

bool vmm_free(void *vaddr)
{
	if (!current_dir)
		return false;
	page_table_t *table = pt_from_virt_addr_recursive(vaddr);
	page_table_entry_t *table_entry = pte_from_virt_addr(table, vaddr);
	pmm_free_block((void *) ADD_12_LSB((table_entry->page_addr)));
	table_entry->present = false;
	table_entry->writable = false;
	table_entry->page_addr = 0;
	return true;
}

void *vmm_create_directory(void)
{
	void *paddr = vmm_alloc_at_ret_phys((void *) EXTRA_MEM, false);
	page_directory_t *dir = (void *) EXTRA_MEM;
	memset(dir, 0, sizeof(page_directory_t));
	
	dir->entries[1023].present = true;
	dir->entries[1023].writable = true;
	dir->entries[1023].page_table_addr = STRIP_12_LSB((u32) paddr);
	
	for (u32 i = PAGE_DIR_INDEX(KERNEL_VIRT_BASE); i < 1023; i++) {
		if (kdir->entries[i].present) {
			dir->entries[i].present = true;
			dir->entries[i].writable = true;
			dir->entries[i].page_table_addr = kdir->entries[i].page_table_addr;
		}
	}
	
	vmm_unmap(dir);
	return paddr;
}

void vmm_print_kernel_dir(void)
{
	for (u32 i = 0; i < 1024; ++i) {
		if (!(i % 8)) {
			dbgprintf("\n%x(%d): ", i * 1024 * 4096, i);
		}
		
		dbgprintf("%d ", kdir->entries[i].present);
	}
	
	dbgprintf("\n0x100000000\n");
}

void vmm_print_table_kernel_dir(unsigned int num)
{
	page_table_t *table = (void *) (0xFFC00000 + (num << 12));
	
	for (u32 i = 0, addr = num * 1024 * 4096; i < 1024; ++i, addr += 4096) {
		if (!(i % 8)) {
			dbgprintf("\n%x(%d): ", addr, i);
		}
		
		dbgprintf("%d %x, ", table->entries[i].present,
		          ADD_12_LSB((u32) table->entries[i].page_addr));
	}
	
	dbgprintf("\n%x\n", num * 1024 * 4096 + (1024 * 4096));
}

bool vmm_init(void)
{
	kdir = &kernel_page_dir;
	
	current_dir = kdir;
	
	return true;
}
