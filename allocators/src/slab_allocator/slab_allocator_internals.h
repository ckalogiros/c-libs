/*
 * Created by ckalo on 1/22/26.
 */
#ifndef ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H
#define ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H

#include "slab_allocator_struct.h"


/*********************************************************************************************/
// nen implementation

void mem_chache_table_init(void);

void mem_cache_table_destroy(void);

void mem_cache_init(enum MEM_CACHE_SIZES slab_size);

void mem_cache_table_destroy(void);
/*********************************************************************************************/



struct mem_cache* mem_cache_create(const buffer_size_t buffer_size, const uint8 type_size);

struct slab_flist* mem_cache_alloc_slab(struct mem_cache* mcache);

bool mem_cache_free_slab(struct mem_cache* mcache, void* slab_address);

struct mem_cache* mem_cache_find_cache(const slab_size_t slab_size);


// TEMPORARY
char_buf16_t number_to_string(const char* str, const uint32 integer);


#endif //ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H
