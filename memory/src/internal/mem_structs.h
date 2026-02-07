/*
 * Created by ckalo on 2/7/26.
 */
#ifndef MEMORY_MEM_STRUCTS_H
#define MEMORY_MEM_STRUCTS_H
#include "my_types.h"

enum MEMORY_ERROR_CODE {
    NO_MEMORY_MANAGER_INIT = BIT_MASK_FROM_UINT(1),
    NO_FREE_MEMORY = BIT_MASK_FROM_UINT(2),
    MEMMAN_ALREADY_INITIALIZED = BIT_MASK_FROM_UINT(3),
    ALLOC_NOT_CONTIGUOUS_MEM = BIT_MASK_FROM_UINT(4),
    MEMORY_IS_IN_USE = BIT_MASK_FROM_UINT(5),
};

struct mem_block {
    void* addr;
    uint64 size;
    uint32 num_pages;
};

#endif //MEMORY_MEM_STRUCTS_H