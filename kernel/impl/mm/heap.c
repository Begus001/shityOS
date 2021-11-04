#include <mm/vmm.h>
#include <mm/mem.h>
#include <tty/serial.h>

#include <mm/heap.h>

const u32 heap_addr = (u32) &kernel_end;
const u32 heap_index_addr = heap_addr + 0x1000;
const size_t heap_index_size = 0x1000000;
const u32 heap_mem_addr = heap_index_addr + heap_index_size + 0x1000;
const size_t heap_mem_size = 0x1000000;

heap_t *
heap_create(void *addr, void *heap_addr, size_t size, heap_index_item_t *index, u32 index_size,
            bool user)
{
	if ((u32) addr % 0x1000 || (u32) heap_addr % 0x1000 || (u32) index % 0x1000)
		return NULL;
	
	heap_t *heap = vmm_alloc_at(addr, user);
	heap_index_item_t *index_head = vmm_alloc_size_at(index, user, index_size);
	vmm_alloc_size_at(heap_addr, user, size);
	
	memset(heap, 0, sizeof(heap_t));
	memset(index, 0, index_size);
	memset(heap_addr, 0, size);
	
	heap->index = index;
	heap->index_size = index_size;
	heap->size = size;
	
	index_head->addr = heap_addr;
	index_head->occupied = false;
	index_head->size = size;
	index_head->next = NULL;
	
	return heap;
}

void *heap_alloc(heap_t *heap, size_t size)
{
	if (!heap || size <= 0 || size > heap->size)
		return NULL;
	
	heap_index_item_t *current_item = heap->index;
	
	do {
		if (!current_item->occupied && size <= current_item->size)
			break;
		current_item = current_item->next;
	} while (current_item);
	
	if (!current_item)
		return NULL;
	
	current_item->occupied = true;
	current_item->size = size;
	
	heap_index_item_t *next_item = current_item + sizeof(heap_index_item_t);
	next_item->addr = current_item->addr + size;
	next_item->occupied = false;
	next_item->size = heap->index->addr + heap->size - next_item->addr;
	next_item->next = NULL;
	
	current_item->next = next_item;
	
	return current_item->addr;
}

int heap_index_merge(heap_t *heap)
{
	if (!heap)
		return 0;
	
	int i = 0;
	
	heap_index_item_t *current_item = heap->index;
	
	while (current_item->next) {
		if (!current_item->occupied && !current_item->next->occupied) {
			current_item->size += current_item->next->size;
			current_item->next = current_item->next->next;
			i++;
		} else {
			current_item = current_item->next;
		}
	}
	
	return i;
}

void heap_free(heap_t *heap, void *addr)
{
	if (!heap || (u32) addr < (u32) heap->index->addr ||
	    (u32) addr > ((u32) heap->index->addr + (u32) heap->index_size))
		return;
	
	heap_index_item_t *current_item = heap->index;
	
	do {
		if (current_item->addr == addr && current_item->occupied)
			break;
		
		current_item = current_item->next;
	} while (current_item);
	
	if (!current_item)
		return;
	
	current_item->occupied = false;
	
	heap_index_merge(heap);
}

void heap_index_print(heap_t *heap)
{
	if (!heap)
		return;
	
	int i = 0;
	
	dbgprintf("HEAP: Index Printout:\n");
	
	heap_index_item_t *current_item = heap->index;
	do {
		dbgprintf("HEAP: Index %d\n", i);
		dbgprintf("HEAP:   addr: %x\n", current_item->addr);
		dbgprintf("HEAP:   occu: %d\n", current_item->occupied);
		dbgprintf("HEAP:   size: %d(%x)\n", current_item->size, current_item->size);
		dbgprintf("HEAP:   next: %x\n", current_item->next);
		
		i++;
		current_item = current_item->next;
	} while (current_item);
	dbgprintf("\n");
}

void heap_init_kheap(void)
{
	kheap = heap_create((void *) heap_addr, (void *) heap_mem_addr, heap_mem_size,
	                    (void *) heap_index_addr, heap_index_size, false);
}

void *kmalloc(size_t size)
{
	return heap_alloc(kheap, size);
}

void kfree(void *addr)
{
	heap_free(kheap, addr);
}
