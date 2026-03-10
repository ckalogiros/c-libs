/*
 * Created by ckalo on 1/22/26.
 */
#ifndef ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H
#define ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H

#include "slab_allocator_struct.h"


/*********************************************************************************************/
// nen implementation

void mem_cache_table_init(void);

void mem_cache_table_destroy(void);

void mem_cache_init(enum MEM_CACHE_SIZES_IDXS slab_size);

void mem_cache_table_destroy(void);
/*********************************************************************************************/



struct mem_cache* mem_cache_create(const buffer_size_t buffer_size, const uint16 type_size);

struct mem_cache* mem_cache_extend_allocation(struct mem_cache* const  mcache, const uint32 left_over_mem);

struct slab_flist* mem_cache_alloc_slab(struct mem_cache* mcache);

bool mem_cache_free_slab(struct mem_cache* mcache, void* slab_address);

struct mem_cache* mem_cache_find_cache(const slab_size_t slab_size);

struct mem_cache* mem_cache_get_next_free_cache(void);

bool mem_cache_does_address_belong_to_mcache(void* address);

/* DEBUG */
void* mem_cache_flist_peek_next_free_slab(const buffer_size_t buffer_size, const uint16 type_size);



#endif //ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H
