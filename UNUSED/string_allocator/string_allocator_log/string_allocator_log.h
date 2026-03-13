/*
 * Created by ckalo on 1/17/26.
 */
#ifndef ALLOCATORS_STRING_ALLOCATOR_LOG_H
#define ALLOCATORS_STRING_ALLOCATOR_LOG_H

#include "string_allocator_struct.h"

void print_string_mem_cache_struct(const struct string_mem_cache* const mem_cache);

void print_string_mem_cache_create(const struct string_mem_cache* const mem_cache);

void print_string_slabs_free_list(struct string_slab_flist* slabs_free_list);

// ERROR handling
void string_allocator_print_error(enum STRING_ALLOCATOR_ERROR_CODE code);

#endif //ALLOCATORS_STRING_ALLOCATOR_LOG_H
