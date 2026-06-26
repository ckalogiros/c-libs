/*
 * Created by ckalo on 17/6/26.
 */
#ifndef ALLOCATORS_QUEUE_ALLOCATOR_H
#define ALLOCATORS_QUEUE_ALLOCATOR_H

#include "queue_allocator_struct.h"
#include <stddef.h>


memid_t queue_allocator_create_new_instance(const pageid_t num_pages);

void* queue_allocator_alloc(const memid_t inst_id, const size_t size, const typesize_t alignment, const uint64 debug_flag);

void* queue_allocator_release(const memid_t inst_id);

memoffset_t queue_allocator_get_last_elem_size(const memid_t inst_id);

/* TEST-DEBUG */
struct queue_allocator_instance* debug_get_queue_allocator_table();
struct queue_allocator_instance* debug_get_queue_allocator_instance(const memid_t inst_id);
void* debug_get_queue_allocator_instance_start_address(const memid_t inst_id);
uint32 debug_get_queue_allocator_init_table_size();
uint32 debug_get_queue_allocator_init_instance_num_pages();
#endif //ALLOCATORS_QUEUE_ALLOCATOR_H