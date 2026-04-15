/*
 * Created by ckalo on 2/7/26.
 */
#ifndef MEMORY_STRUCTS_H
#define MEMORY_STRUCTS_H

#include "my_types.h"
#include "mem_man_types.h"


#define MEM_POISON_PATTERN 0x0abccbcabeffdfdac


enum MEMORY_INTERNAL {
    MEMORY_INTERNAL_STORAGE_INDEX = 0,

    MEMORY_INTERNAL_STORAGE_NUM_PAGES = 4,

    MEMORY_INTERNAL_MEM_TABLE_CAPACITY=16,
    MEMORY_INTERNAL_SLAB_ALLOCATOR_TABLE_CAPACITY=16,
    MEMORY_INTERNAL_STACK_ALLOCATOR_TABLE_CAPACITY=16,
};

struct mem_block {
    void* start; // allocated start address
    struct flist* flist;
    const char* name;
    uint64 size; // total allocated size
    uint64 data_size; // total data allocated size
    uint16 poison_data_size; // a small portion to detect memory corruption
    pageid_t num_pages; // number of allocated pages
};


#endif //MEMORY_STRUCTS_H
