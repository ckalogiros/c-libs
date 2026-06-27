/*
 * Created by ckalo on 3/14/26.
 */
#ifndef ALLOCATORS_STACK_ALLOCATOR_H
#define ALLOCATORS_STACK_ALLOCATOR_H

#include "arena_allocator_struct.h"
#include <stddef.h>


memid_t arena_allocator_create_new_instance(const pageid_t num_pages);

void* arena_allocator_alloc(const memid_t inst_id, const size_t size, const typesize_t alignment);

void* arena_allocator_release(const memid_t inst_id);

memoffset_t arena_allocator_get_last_elem_size(const memid_t inst_id);

/* TEST-DEBUG */
struct arena_allocator_instance* debug_get_arena_allocator_table();
struct arena_allocator_instance* debug_get_arena_allocator_instance(const memid_t inst_id);
void* debug_get_arena_allocator_instance_start_address(const memid_t inst_id);
uint32 debug_get_arena_allocator_init_table_size();
uint32 debug_get_arena_allocator_init_instance_num_pages();
#endif //ALLOCATORS_STACK_ALLOCATOR_H