#ifndef KERNEL_HEAP_H
#define KERNEL_HEAP_H

#include <stddef.h>
#include <stdbool.h>
#include <def/int.h>

typedef struct {
	void *index_addr;
	size_t index_size;
	size_t size;
} heap_t;

typedef struct heap_index_item heap_index_item_t;
struct heap_index_item {
	void *addr;
	bool occupied;
	heap_index_item_t *next;
};

heap_t *
heap_create(void *addr, void *heap_addr, size_t size, void *index_addr, u32 index_size, bool user);

#endif
