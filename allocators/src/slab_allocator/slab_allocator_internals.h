/*
 * Created by ckalo on 1/22/26.
 */
#ifndef ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H
#define ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H

#include "slab_allocator_struct.h"


void slab_allocator_init(void);

memid_t slab_allocator_create_new_instance(const pageid_t num_pages, const char* name);

void slab_allocator_create_new_pool(const pageid_t num_pages);

struct slab_allocator_mcache* slab_allocator_create_mcache(const slab_elems_t num_elems, const uint8 type_size);

struct slab_allocator_mcache* mcache_extend_allocation(struct slab_allocator_mcache* const mcache, const uint32 left_over_mem);

struct flist* mcache_alloc_slab(struct slab_allocator_mcache* mcache);

bool mcache_free_slab(struct slab_allocator_mcache* mcache, void* slab_address);

void mcache_table_destroy(void);


/* HELPER FUNCTIONS */
memid_t pool_find_size(const slab_size_t slab_size);

struct slab_allocator_mcache* mcache_find_size(const slab_size_t slab_size);

struct slab_allocator_mcache* mcache_get_next_free(void);


/* DEBUG */
void* mcache_flist_peek_next_free_slab(slab_elems_t num_elems, const uint8 type_size);


/*
 * these are a copies of the function(s) of slab_allocator, for testing purposes
 * MUST BE KEPT UPDATED TO THE ACTUAL SLAB ALLOCATOR'S FUNCTIONS
 */
char* mcache_free_slab_test_debug(struct slab_allocator_mcache* mcache, void* slab_address);

char* slab_allocator_release_test_debug(void* ptr, const slab_size_t string_size);

#endif //ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H
