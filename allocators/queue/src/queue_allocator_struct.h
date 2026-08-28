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
#ifndef QUEUE_ALLOCATOR_STRUCT_H
#define QUEUE_ALLOCATOR_STRUCT_H

#include "queue_allocator_types.h"

enum QUEUE_ALLOCATOR {
    QUEUE_ALLOCATOR_INSTANCE_INITIAL_TABLE_COUNT = 64,
    QUEUE_ALLOCATOR_OFFSETS_TABLE_COUNT          = 16,
    QUEUE_ALLOCATOR_MEMORY_ALIGNMENT             = 64,
    QUEUE_ALLOCATOR_OFFSETS_INITIAL_NUM_PAGES    = 1,
};

struct queue_allocator_data {
    void*         start;       // allocator's data memory address
    queue_size_t  size;        // total allocated memory block size
    queue_size_t  used_size;   // used size
    memid_t       mem_id;      // memory manager's table index
};

struct queue_allocator_instance {
    void* offsets_table_start;             // memory address
    queue_table_t instance_id;             // memory manager's table index
    queue_table_t offsets_table_size;      // allocated memory block size
    queue_table_t offsets_table_count;     // num allocations
    queue_table_t front_ptr_offset;        // the front pointer, as an index offet to the queue buffer
    queue_table_t front_ptr_elems_counter; // the front pointer's index to the offsets table
    queue_table_t back_ptr_offset;         // the back pointer, as an index offet to the queue buffer
    queue_table_t back_ptr_elems_counter;  // the back pointer's index to the offsets table

    struct queue_allocator_data data;
};

#endif //QUEUE_ALLOCATOR_STRUCT_H
