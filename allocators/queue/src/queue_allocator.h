/*
 * Created by ckalo on 17/6/26.
 */
#ifndef QUEUE_ALLOCATOR_H
#define QUEUE_ALLOCATOR_H

#include "queue_allocator_struct.h"
#include <stddef.h>


memid_t queue_allocator_create_new_instance(const pageid_t num_pages);

void* queue_allocator_alloc(const memid_t inst_id, const size_t size, const typesize_t alignment);

void* queue_allocator_dequeue(const memid_t inst_id);

void* queue_allocator_reset(const memid_t inst_id);

memoffset_t queue_allocator_get_last_elem_size(const memid_t inst_id);

/* GETTERS */
queue_size_t queue_allocator_offsets_table_get_size(const queue_table_t inst_id);
queue_size_t queue_allocator_offsets_table_get_count(const queue_table_t inst_id);
queue_size_t queue_allocator_instance_data_get_size(const queue_table_t inst_id);
queue_size_t queue_allocator_instance_data_get_used_size(const queue_table_t inst_id);


/* TEST-DEBUG */
struct queue_allocator_instance* debug_queue_allocator_get_table();
struct queue_allocator_instance* debug_queue_allocator_get_instance(const memid_t inst_id);
void* debug_queue_allocator_instance_get_data(const memid_t inst_id);
uint32 debug_queue_allocator_get_table_size();
#endif //QUEUE_ALLOCATOR_H