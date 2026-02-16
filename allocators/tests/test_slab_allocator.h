/*
 * Created by ckalo on 2/16/26.
 */
#ifndef G_INCLUDES_TEST_SLAB_ALLOCATOR_H
#define G_INCLUDES_TEST_SLAB_ALLOCATOR_H
#include "slab_allocator_struct.h"

void print_buffer(void* buffer, const uint32 buffer_size);

void check_buffer(void* buffer, const uint32 buffer_size, const char c);

bool mem_cache_free_slab_test_debug(struct mem_cache* mcache, void* slab_address, uint64* free_buffer, const uint32 free_idx);

void mem_cache_release_buffer_test_debug(void* ptr, const buffer_size_t buffer_size,
                                    const uint8 type_size, uint64* free_buffer, const uint32 free_idx);

enum CHAR_ARRAY {
    CHAR_A=65,
    CHAR_END = 90,
};

#endif //G_INCLUDES_TEST_SLAB_ALLOCATOR_H