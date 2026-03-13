/*
 * Created by ckalo on 12/31/25.
 */
#ifndef ALLOCATORS_SLAB_ALLOCATOR_MEM_CACHE_H
#define ALLOCATORS_SLAB_ALLOCATOR_MEM_CACHE_H

#include "slab_allocator_struct.h"

void* slab_allocator_alloc(const slab_elems_t size, const uint16 type_size);

void slab_allocator_release(void* ptr, const slab_elems_t size, const uint16 type_size);

#endif // ALLOCATORS_SLAB_ALLOCATOR_MEM_CACHE_H
