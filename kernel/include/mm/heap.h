#ifndef KERNEL_HEAP_H
#define KERNEL_HEAP_H

#include <stddef.h>
#include <stdbool.h>
#include <def/int.h>

typedef struct heap_index_item heap_index_item_t;

typedef struct {
	heap_index_item_t *index;
	size_t index_size;
	size_t size;
} heap_t;

struct heap_index_item {
	void *addr;
	bool occupied;
	size_t size;
	heap_index_item_t *next;
};

heap_t *
heap_create(void *addr, void *heap_addr, size_t size, heap_index_item_t *index, u32 index_size,
            bool user);

void *heap_alloc(heap_t *heap, size_t size);

#endif
