/*
 * Created by ckalo on 3/14/26.
 */
#ifndef ARENA_STRUCT_H
#define ARENA_STRUCT_H

#include "arena_allocator_types.h"
#include "mem_man_structs.h"



enum ARENA_ALLOCATOR {
    ARENA_ALLOCATOR_INSTANCE_INITIAL_TABLE_COUNT = 64,
    ARENA_ALLOCATOR_MEMORY_ALIGNMENT             = 64,
    ARENA_ALLOCATOR_INSTANCE_INITIAL_NUM_PAGES   = 1,
};

struct arena_allocator_data {
    void*         start;       // allocator's data memory address
    arena_size_t  size;        // total allocated memory block size
    arena_size_t  used_size;   // used size
    memid_t       mem_id;      // memory manager's table index
};

struct arena_allocator_instance {
    memid_t instance_id; // memory manager's table index
    memoffset_t sp_offset;
    struct arena_allocator_data data;
};

#endif //ARENA_STRUCT_H
