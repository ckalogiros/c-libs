/*
 * Created by ckalo on 8/3/26.
 */

#include "string_mem_cache.h"
#include "string_allocator_internals.h"
#include "string_allocator_log.h"

void*
string_slab_alloc_allocate(const string_size_t string_size)
{
    const slab_size_t slab_size_bytes = string_size;
    struct string_mem_cache* mcache = string_mem_cache_find_cache(slab_size_bytes);

    if(!mcache) {
        mcache = string_mem_cache_create(string_size);
    }


    /* get next free slab */
    struct string_slab_flist* free_slab = string_mem_cache_alloc_slab(mcache);

    return (void*) free_slab;
}

void
string_slab_alloc_release(void* ptr, const string_size_t string_size)
{
    const slab_size_t slab_size = string_size;

    struct string_mem_cache* mcache = string_mem_cache_find_cache(slab_size);

    if(!mcache) // handle error: cache not found
        string_allocator_print_error(STRING_ALLOC_MEMCACHE_NOT_FOUND);

    string_mem_cache_free_slab(mcache, ptr);
}
