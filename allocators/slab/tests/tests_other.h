/*
 * Created by ckalo on 2/16/26.
 */
#ifndef TEST_OTHER
#define TEST_OTHER
#include "slab_allocator_struct.h"

void test_slab_allocator(const uint32 num_pages);

void test_check_alloc_extend_pages(const uint32 num_pages);

void test_mem_cache_alloc_release_slabs(void);


#endif //TEST_OTHER