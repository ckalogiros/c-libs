/*
 * Created by ckalo on 12/31/25.
 */

#include "mem_cache.h"
#include "logger.h"
#include "slab_allocator_internals.h"
#include "slab_allocator_log.h"
#include <string.h>


struct slab_list*
mem_cache_update_slabs_free_list(void* address, struct slab_list* slabs_free_list)
{
    if(address == slabs_free_list) {
        print("%sERROR%s\n"
              "releasing slab:%x which is at the free_list:%x\n",
              RED, CLEAR,
              REDUCE_ADDR(address, F6),
              REDUCE_ADDR(slabs_free_list, F6)
        );
    }

    struct slab_list* free_list = (struct slab_list*) address;
    free_list->next = slabs_free_list;
    slabs_free_list = free_list;

    return slabs_free_list;
}

void*
mem_cache_alloc_buffer(const buffer_size_t buffer_size, const uint8 type_size)
{
    const slab_size_t slab_size_bytes = buffer_size * type_size;
    struct mem_cache* mcache = mem_cache_find_cache(slab_size_bytes);

    if(!mcache)
        mcache = mem_cache_create(buffer_size, type_size);

    // TODO: if there are no more free slabs, allocate new page and setup slabs
    if(mcache->slabs_free_list == NULL) {
        slab_alloc(slab_size_bytes, mcache);
    }

    mcache->slabs_used++;

    struct slab_list* free_slab = mcache->slabs_free_list;
    // point mem_cache free_list to the "next" free slab in the list
    mcache->slabs_free_list = mcache->slabs_free_list->next;

    return (void*) free_slab;
}

void
mem_cache_release_buffer(void* ptr, const buffer_size_t buffer_size, const uint8 type_size)
{
    // todo: find a way to access slabs_free_list without searching the mem_cache's list
    const slab_size_t slab_size = buffer_size * type_size;

    struct mem_cache* mcache = mem_cache_find_cache(slab_size);

    // handle error: cache not found
    if(!mcache)
        slab_allocator_print_error(SLAB_ALLOC_MEMCACHE_NOT_FOUND);

    mcache->slabs_free_list = mem_cache_update_slabs_free_list(ptr, mcache->slabs_free_list);
}
