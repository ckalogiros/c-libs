/*
 * Created by ckalo on 2/8/26.
 */
#ifndef MEMORY_MEM_MAN_INTERFACE_H
#define MEMORY_MEM_MAN_INTERFACE_H

#include "my_types.h"
#include "mem_man_types.h"



enum MEMORY_ERROR_CODE {
    MALLOC_FAILED_TO_ALLOC_MEMORY = BIT_MASK_FROM_UINT(0),
    NO_MEMORY_MANAGER_INIT = BIT_MASK_FROM_UINT(1),
    NO_FREE_MEMORY = BIT_MASK_FROM_UINT(2),
    MEMMAN_ALREADY_INITIALIZED = BIT_MASK_FROM_UINT(3),
    ALLOC_NOT_CONTIGUOUS_MEM = BIT_MASK_FROM_UINT(4),
    MEMORY_IS_IN_USE = BIT_MASK_FROM_UINT(5),
    FREE_MEMORY_FAILED = BIT_MASK_FROM_UINT(6),
    FREE_PAGE_ALREADY_FREED = BIT_MASK_FROM_UINT(7),
    MEMORY_CORRUPTED = BIT_MASK_FROM_UINT(7),
};


void memman_init(void);

void memman_terminate(void);

memid_t memman_allocate_block(const uint16 num_pages, const uint32 alignment, const char* info);

memid_t memman_deallocate_block(const memid_t memid);

struct memhnd_t memman_page_request(const uint16 num_pages, const memid_t memid);

void* memman_get_mem_start_by_memid(const memid_t memid);

bool memman_page_release(const struct memhnd_t hnd);

void* memman_get_alloc_mem_by_handle(const struct memhnd_t handle);

#endif //MEMORY_MEM_MAN_INTERFACE_H
