/*
 * Created by ckalo on 12/31/25.
 */
#ifndef SLAB_ALLOCATOR_H
#define SLAB_ALLOCATOR_H

#include "slab_allocator_struct.h"

void* slab_allocator_alloc(const slab_table_t inst_id, const slab_elems_t num_elems, const uint16 type_size, const char* name);

void slab_allocator_release(const slab_table_t inst_id, void* ptr, const slab_elems_t num_elems, const uint16 type_size);

memid_t slab_allocator_get_memid(const slab_table_t inst_id, const slab_elems_t num_elems, const uint16 type_size);
#endif // SLAB_ALLOCATOR_H
