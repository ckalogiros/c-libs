/*
 * Created by ckalo on 2/8/26.
 */
#ifndef MEMORY_MEM_MAN_INTERFACE_H
#define MEMORY_MEM_MAN_INTERFACE_H


#include "my_types.h"
#include "mem_structs.h"

#define NUM_CHUNKS 16
#define PAGE_SIZE 1024



enum MEM_ID_SELECTOR {
    MEM_ID_SELF_MEM_STORAGE = 0,
    MEM_ID_ALLOCATOR_SLAB,
    MEM_ID_ALLOCATOR_STRING,
    // leave initial chunks for specific allocators
    MEM_ID_MAX_ELEMS,
};

enum MEMORY_ERROR_CODE {
    MALLOC_FAILED_TO_ALLOC_MEMORY = BIT_MASK_FROM_UINT(0),
    NO_MEMORY_MANAGER_INIT = BIT_MASK_FROM_UINT(1),
    NO_FREE_MEMORY = BIT_MASK_FROM_UINT(2),
    MEMMAN_ALREADY_INITIALIZED = BIT_MASK_FROM_UINT(3),
    ALLOC_NOT_CONTIGUOUS_MEM = BIT_MASK_FROM_UINT(4),
    MEMORY_IS_IN_USE = BIT_MASK_FROM_UINT(5),
    FREE_MEMORY_FAILED = BIT_MASK_FROM_UINT(6),
    FREE_PAGE_ALREADY_FREED = BIT_MASK_FROM_UINT(7),
};


/* ************************************
 * allocation's return type to caller */
struct memhnd_t {
    memid_t mem_id;
    pageid_t page_id;
    memid_t num_pages;
};



#define PAGE_SIZE 1024

void memman_init(void);

void memman_terminate(void);

struct memhnd_t memman_alloc_pages(const uint16 num_pages, enum MEM_ID_SELECTOR);

bool memman_release(const struct memhnd_t hnd);

void* memman_get_alloc_mem_by_handle(const struct memhnd_t handle);

void* memman_temp_alloc_sys_call(const uint32 alignment, const uint32 total_size);

#endif //MEMORY_MEM_MAN_INTERFACE_H
