/*
 * Created by ckalo on 2/16/26.
 */
#ifndef G_INCLUDES_TEST_SLAB_ALLOCATOR_H
#define G_INCLUDES_TEST_SLAB_ALLOCATOR_H
#include "slab_allocator_struct.h"

void print_buffer(void* buffer, const uint32 buffer_size, const char c);

void check_buffer(void* buffer, const uint32 buffer_size, const char c);

bool mem_cache_free_slab_test_debug(struct mem_cache* mcache, void* slab_address, uint64* free_buffer, const uint32 free_idx);

void mem_cache_release_buffer_test_debug(void* ptr, const buffer_size_t buffer_size,
                                    const uint8 type_size, uint64* free_buffer, const uint32 free_idx);

enum CHAR_ARRAY {
    CHAR_A=65,
    CHAR_B,
    CHAR_C,
    CHAR_D,
    CHAR_E,
    CHAR_F,
    CHAR_G,
    CHAR_H,
    CHAR_I,
    CHAR_J,
    CHAR_K,
    CHAR_L,
    CHAR_M,
    CHAR_N,
    CHAR_O,
    CHAR_P,
    CHAR_Q,
    CHAR_R,
    CHAR_S,
    CHAR_T,
    CHAR_U,
    CHAR_V,
    CHAR_W,
    CHAR_X,
    CHAR_Y,
    CHAR_Z,
    CHAR_END,
};
static uint8 char_arr[] = {
    CHAR_A,
    CHAR_B,
    CHAR_C,
    CHAR_D,
    CHAR_E,
    CHAR_F,
    CHAR_G,
    CHAR_H,
    CHAR_I,
    CHAR_J,
    CHAR_K,
    CHAR_L,
    CHAR_M,
    CHAR_N,
    CHAR_O,
    CHAR_P,
    CHAR_Q,
    CHAR_R,
    CHAR_S,
    CHAR_T,
    CHAR_U,
    CHAR_V,
    CHAR_W,
    CHAR_X,
    CHAR_Y,
    CHAR_Z,
    };

#endif //G_INCLUDES_TEST_SLAB_ALLOCATOR_H