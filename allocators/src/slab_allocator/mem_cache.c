/*
 * Created by ckalo on 12/31/25.
 */

#include "mem_cache.h"
#include "logger.h"
#include "slab_allocator_internals.h"
#include "slab_allocator_log.h"

void*
mem_cache_alloc_buffer(const buffer_size_t buffer_size, const uint16 type_size)
{
    const slab_size_t slab_size_bytes = buffer_size * type_size;
    struct mem_cache* mcache = mem_cache_find_cache(slab_size_bytes);

    if(!mcache)
        mcache = mem_cache_create(buffer_size, type_size);

    // get next free slab
    struct slab_flist* free_slab = mem_cache_alloc_slab(mcache);

    return (void*) free_slab;
}

void
mem_cache_release_buffer(void* ptr, const buffer_size_t buffer_size, const uint16 type_size)
{
    const slab_size_t slab_size = buffer_size * type_size;

    struct mem_cache* mcache = mem_cache_find_cache(slab_size);

    if(!mcache) // handle error: cache not found
        slab_allocator_print_error(SLAB_ALLOC_MEMCACHE_NOT_FOUND);

    mem_cache_free_slab(mcache, ptr);
}
