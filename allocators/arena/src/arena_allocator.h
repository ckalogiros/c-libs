/*
 * Created by ckalo on 3/14/26.
 */
#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

#include "arena_allocator_struct.h"


/* INTERFACE */
memid_t arena_allocator_create_new_instance(const pageid_t num_pages);

void* arena_allocator_alloc(const arena_table_t inst_id, const arena_size_t size, const typesize_t type_size);

void* arena_allocator_release(const arena_table_t inst_id);

memoffset_t arena_allocator_get_last_elem_size(const arena_table_t inst_id);

/* GETTERS */
arena_size_t arena_allocator_instance_get_size(const arena_table_t inst_id);

arena_size_t arena_allocator_instance_data_get_used_size(const arena_table_t inst_id);

memid_t arena_allocator_instance_data_get_memid(const arena_table_t inst_id);

/* TEST-DEBUG */
struct arena_allocator_instance* debug_arena_allocator_instance_get_table();
struct arena_allocator_instance* debug_arena_allocator_get_instance(const arena_table_t inst_id);
void* debug_arena_allocator_instance_get_data(const arena_table_t inst_id);
uint32 debug_arena_allocator_get_table_size();
#endif //STACK_ALLOCATOR_H