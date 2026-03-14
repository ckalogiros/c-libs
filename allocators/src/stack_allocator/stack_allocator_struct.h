/*
 * Created by ckalo on 3/14/26.
 */
#ifndef ALLOCATORS_STACK_ALLOCATOR_STRUCT_H
#define ALLOCATORS_STACK_ALLOCATOR_STRUCT_H

#include "mem_structs.h"

typedef uint16 memoffset_t;

struct stack_allocator_data {
    void* start; // allocator's data memory address
    uint32 size; // total allocated memory block size
    uint16 used_size; // used size
    memid_t instance_id; // memory manager's table index
};

struct stack_allocator_instance {
    void* table_start; // memory address
    memid_t instance_id; // memory manager's table index
    memoffset_t table_size; // allocated memory block size
    memoffset_t table_count; // num allocations
    memoffset_t stack_ptr_offset;

    struct stack_allocator_data data;
};

#endif //ALLOCATORS_STACK_ALLOCATOR_STRUCT_H
