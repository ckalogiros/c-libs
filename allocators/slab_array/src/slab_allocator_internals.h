/*
 * Created by ckalo on 1/22/26.
 */
#ifndef ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H
#define ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H

#include "slab_allocator_struct.h"

struct slab_allocator_instance* slab_allocator_get_instance(const salltable_t inst_id);

void slab_allocator_init(void);

memid_t slab_allocator_create_new_instance(const pageid_t reserve_num_pages, const char* name);

uint16 slab_allocator_instance_create_new_pool(const salltable_t inst_id, const pageid_t num_pages,
    const slab_size_t slab_size, const char* name);

struct slab_allocator_mcache* slab_allocator_create_mcache(const salltable_t inst_id,
    const slab_elems_t num_elems, const uint16 type_size, const char* name);

struct slab_allocator_mcache* mcache_extend_allocation(struct slab_allocator_mcache* const mcache, const uint32 left_over_mem);

struct flist* mcache_alloc_slab(struct slab_allocator_mcache* mcache);

bool mcache_free_slab(struct slab_allocator_mcache* mcache, void* slab_address);

void mcache_table_destroy(void);


/* HELPER FUNCTIONS */
// memid_t pool_find_size(const slab_size_t slab_size);

int16 pool_find_size(const salltable_t inst_id, const slab_size_t slab_size);

struct slab_allocator_mcache* mcache_find_size(const salltable_t inst_id, const slab_size_t slab_size);

// struct slab_allocator_mcache* mcache_get_next_free(void);


/* DEBUG */
void* mcache_flist_peek_next_free_slab(const salltable_t inst_id, slab_elems_t num_elems, const uint16 type_size);







/***************************************************************************************************
 * TEST-DEBUG
 **************************************************************************************************/

struct slab_allocator_instance* debug_get_slab_allocator_table();
/*
 * these are a copies of the function(s) of slab_allocator, for testing purposes
 * MUST BE KEPT UPDATED TO THE ACTUAL SLAB ALLOCATOR'S FUNCTIONS
 */
// char* mcache_free_slab_test_debug(struct slab_allocator_mcache* mcache, void* slab_address);

// char* slab_allocator_release_test_debug(void* ptr, const slab_size_t string_size);

#endif //ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H
