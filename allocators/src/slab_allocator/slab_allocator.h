/*
 * Created by ckalo on 12/31/25.
 */
#ifndef ALLOCATORS_SLAB_ALLOCATOR_MEM_CACHE_H
#define ALLOCATORS_SLAB_ALLOCATOR_MEM_CACHE_H

#include "slab_allocator_struct.h"

void* slab_allocator_alloc(const salltable_t inst_id, const slab_elems_t size, const uint16 type_size, const char* name);

void slab_allocator_release(const salltable_t inst_id, void* ptr, const slab_elems_t size, const uint16 type_size);

memid_t slab_allocator_get_memid(const salltable_t inst_id, const slab_elems_t num_elems, const uint16 type_size);
#endif // ALLOCATORS_SLAB_ALLOCATOR_MEM_CACHE_H
