/*
 * Created by ckalo on 1/17/26.
 */
#ifndef SLAB_ALLOCATOR_LOG_H
#define SLAB_ALLOCATOR_LOG_H

#include "slab_allocator_interface.h"

void print_mem_cache_struct(const struct slab_allocator_mcache* const mem_cache);

void print_mem_cache_create(const struct slab_allocator_mcache* const mem_cache);

void print_slabs_free_list(struct flist* slabs_free_list);
// ERROR handling
void slab_allocator_print_error(enum SLAB_ALLOCATOR_ERROR_CODE code);

#endif //SLAB_ALLOCATOR_LOG_H
