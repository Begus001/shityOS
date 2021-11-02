#include <mm/vmm.h>
#include <mm/mem.h>
#include <tty/serial.h>

#include <mm/heap.h>

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
	} while (current_item != NULL);
	
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
