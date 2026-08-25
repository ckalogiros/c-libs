/*
 * Created by ckalo on 2/8/26.
 *
 * Memory manager:
 * # handles separate allocations with instances, stored in "mem_pool" buffer
 * # allocates memory in blocks of PAGE_SIZE, guaranteed contiguous memory
 * # handles these blocks with a free list, stored inplace
 * TODO:
 *   instance reallocation or extension
 *
 * The caller must allocate the memory with:  "memman_allocate_block()"
 * The caller must request part or whole of that memory with:  "memman_page_request()"
 * The caller handles the memory its self
 * The caller can return pages as freed with: "memman_page_release()"
 * The caller must call memory deinitialization with: "memman_terminate()"
 */
#ifndef MEMORY_INTERFACE_H
#define MEMORY_INTERFACE_H

#include "my_types.h"
#include "mem_man_types.h"

void memman_terminate(void);

/* where the actual memory is allocated */
memid_t memman_allocate_block(const uint16 num_pages, const uint32 alignment, const char* info);

memid_t memman_deallocate_block(const memid_t memid);

/* where the already allocated memory is consumed */
struct memhnd_t memman_page_request(const uint16 num_pages, const memid_t memid);

bool memman_page_release(const struct memhnd_t hnd);

/* How to get allocated memory back */
void* memman_get_alloc_mem_by_handle(const struct memhnd_t handle);

#endif //MEMORY_INTERFACE_H
