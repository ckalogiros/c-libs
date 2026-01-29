/*
 * Created by ckalo on 1/22/26.
 */
#ifndef ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H
#define ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H

#include "slab_allocator_struct.h"


struct mem_cache_list *mem_cache_get_list();

void mem_cache_init();

struct slab_list *slab_alloc(const uint32 slab_size_bytes, struct mem_cache *mem_cache);

struct mem_cache *mem_cache_create(const buffer_size_t buffer_size, const uint8 type_size);

void mem_cache_ctor(struct mem_cache *const mem_cache,
                    const char *name, const uint16 slot_size,
                    const uint16 num_pages);


// TEMPORARY
char_buf16_t number_to_string(const char *str, const uint32 integer);

struct mem_cache *mem_cache_find_cache(const slab_size_t slab_size);

#endif //ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H
