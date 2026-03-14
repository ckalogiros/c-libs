/*
 * Created by ckalo on 2/16/26.
 */
#ifndef ALLOCATORS_TEST_SLAB_ALLOCATOR_H
#define ALLOCATORS_TEST_SLAB_ALLOCATOR_H
#include "slab_allocator_struct.h"

void test_check_alloc_extend_pages(const uint32 num_pages);

void test_mem_cache_alloc_release_slabs(void);

enum CHAR_ARRAY {
    CHAR_A=65,
    CHAR_END = 90,
};

#endif //ALLOCATORS_TEST_SLAB_ALLOCATOR_H