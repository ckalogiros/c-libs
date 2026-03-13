/*
 * Created by ckalo on 12/31/25.
 */

#include "slab_allocator_mcache.h"
#include "logger.h"
#include "slab_allocator_internals.h"
#include "slab_allocator_log.h"

void*
slab_allocator_alloc(const slab_elems_t num_elems, const uint16 type_size)
{
    const slab_size_t slab_size_bytes = num_elems * type_size;
    struct slab_allocator_mcache* mcache = mcache_find_size(slab_size_bytes);

    if(!mcache)
        mcache = slab_allocator_create_mcache(num_elems, type_size);

    /* get next free slab */
    struct flist* free_slab = mcache_alloc_slab(mcache);

    return (void*) free_slab;
}

void
slab_allocator_release(void* ptr, const slab_elems_t size, const uint16 type_size)
{
    const slab_size_t slab_size = size * type_size;

    struct slab_allocator_mcache* mcache = mcache_find_size(slab_size);

    if(!mcache) /* handle error: cache not found */
        slab_allocator_print_error(SLAB_ALLOCATOR_MEMCACHE_NOT_FOUND);

    mcache_free_slab(mcache, ptr);
}
