/*
 * Created by ckalo on 3/16/26.
 */
#ifndef ALLOCATORS_SLAB_UNIT_TEST_H
#define ALLOCATORS_SLAB_UNIT_TEST_H
#include "my_types.h"
#include "slab_allocator_struct.h"


enum FREED_ELEM_FLAGS {
    IS_HEAD=0x1,
    IS_LAST=0x2,
    IS_BETWEEN=0x4,
    IS_ONLY_FREED=0x8,
    IS_LIST_NOT_EMPTY=0x10,
    // IS_HEAD=0x8,
};

struct slab_release_ret {
    char* freed;
    enum FREED_ELEM_FLAGS state;
};

void unit_test_runner_slab_allocator(void);



/* HELPERS */
void print_buffer(void* buffer, const uint32 buffer_size);

void check_buffer(void* buffer, const uint32 buffer_size, const char c, const uint32 index);


/*
 * these are a copies of the function(s) of slab_allocator, for testing purposes
 * MUST BE KEPT UPDATED TO THE ACTUAL SLAB ALLOCATOR'S FUNCTIONS
 */
struct slab_release_ret mem_cache_free_slab_test_debug(struct slab_allocator_mcache* mcache, void* slab_address);

struct slab_release_ret mem_cache_release_buffer_test_debug(const salltable_t inst_id, void* ptr, const slab_elems_t buffer_size, const uint16 type_size);


#endif //ALLOCATORS_SLAB_UNIT_TEST_H
