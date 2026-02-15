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
void mem_cache_init2(enum MEM_CACHE_SIZES type);

/*********************************************************************************************/


struct mem_cache_list*
mem_cache_get_list(void);

void
mem_cache_init(void);

struct slab_flist*
slab_alloc(const uint32 slab_size_bytes, struct mem_cache* mem_cache);


struct mem_cache*
mem_cache_create(const buffer_size_t buffer_size, const uint8 type_size);

void
mem_cache_ctor( struct mem_cache* const mem_cache, const char* name,
                const uint16 slab_size, const uint16 num_pages);

struct mem_cache*
mem_cache_find_cache(const slab_size_t slab_size);

// TEMPORARY
char_buf16_t
number_to_string(const char* str, const uint32 integer);


#endif //ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H
