#ifndef KERNEL_PMM_H
#define KERNEL_PMM_H

#include <sys/def/int.h>
#include <boot/multiboot.h>
#include <stddef.h>

void pmm_init(void *);

void pmm_memmap(void);
void pmm_blocks_used(void);
void pmm_blocks_free(void);

void *pmm_alloc(size_t size);
void pmm_free(void *addr, size_t size);

#endif
