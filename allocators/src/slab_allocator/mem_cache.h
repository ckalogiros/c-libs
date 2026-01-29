/*
 * Created by ckalo on 12/31/25.
 */
#ifndef ALLOCATORS_SLAB_ALLOCATOR_MEM_CACHE_H
#define ALLOCATORS_SLAB_ALLOCATOR_MEM_CACHE_H

#include "slab_allocator_struct.h"

void *mem_cache_alloc_buffer(const buffer_size_t buffer_size,  const uint8 type_size);
void mem_cache_release_buffer(void* ptr, const buffer_size_t buffer_size, const uint8 type_size);

#endif // ALLOCATORS_SLAB_ALLOCATOR_MEM_CACHE_H
