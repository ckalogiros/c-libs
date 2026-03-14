/*
 * Created by ckalo on 3/14/26.
 */
#ifndef ALLOCATORS_STACK_ALLOCATOR_H
#define ALLOCATORS_STACK_ALLOCATOR_H

#include "stack_allocator_struct.h"
#include <stddef.h>

memid_t stack_allocator_create_new_instance(const pageid_t num_pages);

void* stack_allocator_alloc(const memid_t inst_id, const size_t size);

void* stack_allocator_release(const memid_t inst_id);

#endif //ALLOCATORS_STACK_ALLOCATOR_H