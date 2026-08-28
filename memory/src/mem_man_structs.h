/*
 * Created by ckalo on 2/7/26.
 */
#ifndef MEMORY_STRUCTS_H
#define MEMORY_STRUCTS_H

#include "my_types.h"
#include "mem_man_types.h"

enum MEMORY_INTERNAL {
    MEMORY_INTERNAL_STORAGE_INDEX      =  0,
    MEMORY_INTERNAL_STORAGE_NUM_PAGES  =  4,
    MEMORY_INTERNAL_POOL_INITIAL_COUNT = 32,
    MEMORY_INTERNAL_MEMORY_ALIGNMENT   = 64,
};

struct mem_block {
    void*    start;            /// allocated start address
    struct   flist* flist;     /// block's single linked free list, stored at the block
    uint64   size;             /// total allocated size
    uint64   data_size;        /// total data allocated size
    uint16   poison_data_size; /// a small portion to detect memory corruption
    pageid_t num_pages;        /// number of allocated pages
    char     name[32];         /// for DEBUG. THIS IS A BUG. TODO: use separate storage for strings
};


#endif //MEMORY_STRUCTS_H
