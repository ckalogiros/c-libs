/*
 * Created by ckalo on 1/17/26.
 */
#ifndef ALLOCATORS_SLAB_ALLOCATOR_LOG_H
#define ALLOCATORS_SLAB_ALLOCATOR_LOG_H

#include "mem_cache.h"

void print_mem_cache_list(void);
void print_mem_cache_struct(const struct mem_cache* const mem_cache);
void print_mem_cache_create(const uint32 slab_size, const uint8 slabs_num_pages,
                            struct mem_cache_node* cache_node, void* free_mem_start_addr);
void print_slabs_free_list(struct slab_flist* slabs_free_list);
bool print_all_slabs_free_list(void);

// ERROR handling
void slab_allocator_print_error(enum SLAB_ALLOCATOR_ERROR_CODE code);

#endif //ALLOCATORS_SLAB_ALLOCATOR_LOG_H