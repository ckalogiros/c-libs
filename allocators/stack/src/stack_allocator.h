/*
 * Created by ckalo on 3/14/26.
 */
#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

#include "stack_allocator_struct.h"

/* INTERFACE */
stack_table_t stack_allocator_create_new_instance(const pageid_t num_pages);

void* stack_allocator_alloc(const stack_table_t inst_id, const stack_size_t size, const typesize_t type_size);

void* stack_allocator_pop(const stack_table_t inst_id);

void stack_allocator_destroy(const stack_table_t inst_id);

memoffset_t stack_allocator_get_last_elem_size(const stack_table_t inst_id);

/* GETTERS */
stack_size_t stack_allocator_offsets_table_get_size(const stack_table_t inst_id);
stack_size_t stack_allocator_offsets_table_get_count(const stack_table_t inst_id);
stack_size_t stack_allocator_instance_data_get_size(const stack_table_t inst_id);
stack_size_t stack_allocator_instance_data_get_used_size(const stack_table_t inst_id);

/* TEST-DEBUG */
struct stack_allocator_instance* debug_get_stack_allocator_table();
struct stack_allocator_instance* debug_get_stack_allocator_instance(const stack_table_t inst_id);
void* debug_get_stack_allocator_instance_start_address(const stack_table_t inst_id);
uint32 debug_get_stack_allocator_init_table_size();
#endif //STACK_ALLOCATOR_H