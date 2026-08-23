/*
 * Created by ckalo on 17/6/26.
 * 
 * queue memory
 * [ BACK_PTR-> DATA FRONT_PTR FREE_MEM ]
 * 
 * start of data                                                                        end of data
 * ^----------------------------------------------------------------------------------------------^
 * [ [BACK_PTR + FISRT_ELEM_SIZE] [2nd ELEM] [3rd ELEM] ... [n-2 ELEM] [FRONT_PTR + LAST_ELEM_SIZE] [FREE_MEM] ]
 *
 * offsets table
 * [ [1rst offset] [2nd offset] [3rd offset] ... [n-2 offset] [last offset] [free]  [free] [free] ...]
 *
 *
 * ISSUES
 * 1. if the offsets are byte offsets, maybe we need to reconsider int32 type for memoffset_t
 */
#ifndef ALLOCATORS_QUEUE_ALLOCATOR_STRUCT_H
#define ALLOCATORS_QUEUE_ALLOCATOR_STRUCT_H

#include "mem_man_structs.h"

typedef uint16 memoffset_t;

struct queue_allocator_data {
    void* start; // allocator's data memory address
    uint32 size; // total allocated memory block size
    uint32 used_size; // used size
    memid_t instance_id; // memory manager's table index
};

struct queue_allocator_instance {
    void* offsets_table_start; // memory address
    memid_t instance_id; // memory manager's table index
    memoffset_t offsets_table_size; // allocated memory block size
    memoffset_t offsets_table_count; // num allocations
    // memoffset_t queue_ptr_offset;
    memoffset_t front_ptr_offset; // the front pointer, as an index offet to the queue buffer
    memoffset_t front_ptr_offset_index;  // the front pointer's index to the offsets table
    memoffset_t back_ptr_offset;  // the back pointer, as an index offet to the queue buffer
    memoffset_t back_ptr_offset_index;  // the back pointer's index to the offsets table

    struct queue_allocator_data data;
};

#endif //ALLOCATORS_QUEUE_ALLOCATOR_STRUCT_H
