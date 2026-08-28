/*
 * Created by ckalo on 3/14/26.
 */
#ifndef STACK_ALLOCATOR_STRUCT_H
#define STACK_ALLOCATOR_STRUCT_H

#include "stack_allocator_types.h"
#include "mem_man_structs.h"


enum STACK_ALLOCATOR {
    STACK_ALLOCATOR_INSTANCE_INITIAL_TABLE_COUNT = 16,
    STACK_ALLOCATOR_OFFSETS_TABLE_COUNT          = 16,
    STACK_ALLOCATOR_MEMORY_ALIGNMENT             = 64,
    STACK_ALLOCATOR_OFFSETS_INITIAL_NUM_PAGES    = 1,
};


struct stack_allocator_data {
    void*        start;       // allocator's data memory address
    stack_size_t size;        // total allocated memory block size
    stack_size_t used_size;   // used size
    memid_t      instance_id; // memory manager's table index
};

struct stack_allocator_instance {
    void* offsets_table_start;         // memory address
    stack_table_t instance_id;         // memory manager's table index
    memoffset_t   offsets_table_size;  // allocated memory block size
    memoffset_t   offsets_table_count; // num allocations
    memoffset_t   sp_offset;
    struct stack_allocator_data data;
};

#endif //STACK_ALLOCATOR_STRUCT_H
