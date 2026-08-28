/*
 * Created by ckalo on 12/31/25.
 */

#include <assert.h>
#include <stdlib.h>

#include "slab_allocator_interface.h"
#include "logger.h"
#include "slab_allocator_internals.h"
#include "slab_allocator_log.h"


void*
slab_allocator_alloc(const slab_table_t inst_id, const slab_elems_t num_elems,
    const uint16 type_size, const char* name)
{
    // IMPORTANT! mcache find() checks the exact slab_size, and creates new mcaches for: 86, 162, 196,etc
    const slab_size_t slab_size_bytes = num_elems * type_size;
    /* less than 8 bytes slab is not allowed, because of the freelist pointer bytesize(=8) */
    assert(slab_size_bytes > SIZE_I64);

    struct slab_allocator_mcache* mcache = mcache_find_size(inst_id, slab_size_bytes);

    if(!mcache)
        mcache = slab_allocator_create_mcache(inst_id, num_elems, type_size, name);

    /* get next free slab */
    struct flist* free_slab = mcache_alloc_slab(mcache);
    // print("Slab address:%x\n", REDUCE_ADDR(free_slab, F8));
    return (void*) free_slab;
}

void
slab_allocator_release(const slab_table_t inst_id, void* ptr, const slab_elems_t num_elems, const uint16 type_size)
{
    const slab_size_t slab_size = num_elems * type_size;

    struct slab_allocator_mcache* mcache = mcache_find_size(inst_id, slab_size);

    if(!mcache) /* handle error: cache not found */
        slab_allocator_print_error(SLAB_ALLOCATOR_MEMCACHE_NOT_FOUND);

    mcache_free_slab(mcache, ptr);
}

memid_t
slab_allocator_get_memid(const slab_table_t inst_id, const slab_elems_t num_elems, const uint16 type_size)
{
    const slab_size_t slab_size_bytes = num_elems * type_size;
    struct slab_allocator_mcache* mcache = mcache_find_size(inst_id, slab_size_bytes);
    if(!mcache) /* handle error: cache not found */
        slab_allocator_print_error(SLAB_ALLOCATOR_MEMCACHE_NOT_FOUND);

    return mcache->mem_handle.mem_id;
}