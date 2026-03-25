/*
 * Created by ckalo on 3/14/26.
 */
#ifndef ALLOCATORS_STACK_ALLOCATOR_H
#define ALLOCATORS_STACK_ALLOCATOR_H

#include "stack_allocator_struct.h"
#include <stddef.h>


memid_t stack_allocator_create_new_instance(const pageid_t num_pages);

void* stack_allocator_alloc(const memid_t inst_id, const size_t size, const typesize_t type_size);

void* stack_allocator_release(const memid_t inst_id);

/* TEST-DEBUG */
struct stack_allocator_instance* debug_get_stack_allocator_table();
struct stack_allocator_instance* debug_get_stack_allocator_instance(const memid_t inst_id);
void* debug_get_stack_allocator_instance_start_address(const memid_t inst_id);
uint32 debug_get_stack_allocator_init_table_size();
uint32 debug_get_stack_allocator_init_instance_num_pages();
#endif //ALLOCATORS_STACK_ALLOCATOR_H