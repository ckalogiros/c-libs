/*
 * Created by ckalo on 3/14/26.
 */
#ifndef SLAB_ALLOCATOR_FPRINT_H
#define SLAB_ALLOCATOR_FPRINT_H

#include "slab_allocator_struct.h"

void fprint_mcache(struct slab_allocator_mcache* mcache, const char* operation);

void fprint_append_all_slabs_of_mcache(struct slab_allocator_mcache* mcache, const char* info);

void fprint_append_a_slab(void* slab_start, const uint32 slab_size, const char* info);

// void slall_fprint_mcaches(struct slab_allocator_mcache* mcache, const char* operation);


#endif //SLAB_ALLOCATOR_FPRINT_H