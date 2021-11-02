#include <mm/vmm.h>
#include <mm/mem.h>
#include <tty/serial.h>

#include <mm/heap.h>

heap_t *
heap_create(void *addr, void *heap_addr, size_t size, void *index_addr, u32 index_size, bool user)
{
	if ((u32) addr % 0x1000 || (u32) heap_addr % 0x1000 || (u32) index_addr % 0x1000)
		return NULL;
	
	heap_t *heap = vmm_alloc_at(addr, user);
	heap_index_item_t *index_head = vmm_alloc_size_at(index_addr, user, index_size);
	vmm_alloc_size_at(heap_addr, user, size);
	
	memset(heap, 0, sizeof(heap_t));
	memset(index_addr, 0, index_size);
	memset(heap_addr, 0, size);
	
	heap->index_addr = index_addr;
	heap->index_size = index_size;
	heap->size = size;
	
	index_head->addr = heap_addr;
	index_head->occupied = false;
	index_head->next = NULL;
	
	return heap;
}
