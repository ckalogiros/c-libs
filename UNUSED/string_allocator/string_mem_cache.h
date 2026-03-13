/*
 * Created by ckalo on 8/3/26.
 */
#ifndef ALLOCATORS_STRING_ALLOCATOR_MEM_CACHE_H
#define ALLOCATORS_STRING_ALLOCATOR_MEM_CACHE_H

#include "string_allocator_struct.h"

void* string_slab_alloc_allocate(const string_size_t string_size);

void string_slab_alloc_release(void* ptr, const uint16 string_size);

#endif // ALLOCATORS_STRING_ALLOCATOR_MEM_CACHE_H
