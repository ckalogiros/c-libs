/*
 * Created by ckalo on 1/22/26.
 */


#include "logger.h"
#include "mem_cache.h"
// #include "mem_log.h"
// #include "mem_man_interface.h"
#include "my_types.h"
#include <stdio.h>
#include "my_macros.h"
#include "test_allocator.h"
#include "test_performance.h"

#include <poll.h>
#include <unistd.h>

#include "mem_man_interface.h"
#include "slab_allocator_log.h"
// #include "std_input.h"
// #include "std_input_public.h"

int32
has_input()
{
    struct pollfd pfd = {.fd = STDIN_FILENO, .events = POLLIN};
    return poll(&pfd, 1, 0) > 0; // 0 timeout = non-blocking
}

uint32 test_id = 0;



void
test_slab_allocator_alloc_release();

void test();


int
main(void)
{
    test();


    // test_slab_allocator_alloc_release();

    // test performance measurements
    // test_perf_slab_allocation_release();



    return 0;
}

void test()
{
    memman_init();

    slab_alloc_print_structs_sizes();

    const uint32 buffer_size = 512;
    const uint32 type_size = sizeof(uint32);
    uint32* buffer = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
    for(uint32 i = 0; i < buffer_size; i++) {
        // buffer[i] = 'A'+'D'+'G'+'V';
        buffer[i] = 'ADGV';
    }
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer), REDUCE_ADDR(buffer, F6), CLEAR);

    memman_terminate();
}

void
test_slab_allocator_alloc_release()
{
    memman_init();


    const uint32 buffer_size = 16;
#define buffer_type_t uint64
    const uint32 type_size = sizeof(buffer_type_t);

    // different buffers to allocate
    buffer_type_t* buffer32_1 = NULL;
    buffer_type_t* buffer32_2 = NULL;
    buffer_type_t* buffer32_3 = NULL;
    buffer_type_t* buffer32_4 = NULL;
    buffer_type_t* buffer32_5 = NULL;
    buffer_type_t* buffer32_6 = NULL;
    buffer_type_t* buffer32_7 = NULL;
    buffer_type_t* buffer32_8 = NULL;

    buffer32_1 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_1), REDUCE_ADDR(buffer32_1, F6), CLEAR);
    buffer32_2 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_2), REDUCE_ADDR(buffer32_2, F6), CLEAR);
    buffer32_3 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_3), REDUCE_ADDR(buffer32_3, F6), CLEAR);
    buffer32_4 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_4), REDUCE_ADDR(buffer32_4, F6), CLEAR);
    buffer32_5 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_5), REDUCE_ADDR(buffer32_5, F6), CLEAR);
    buffer32_6 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_6), REDUCE_ADDR(buffer32_6, F6), CLEAR);
    buffer32_7 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_7), REDUCE_ADDR(buffer32_7, F6), CLEAR);
    buffer32_8 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_8), REDUCE_ADDR(buffer32_8, F6), CLEAR);

    print_all_slabs_free_list();


    mem_cache_release_buffer(buffer32_1, buffer_size, type_size);
    mem_cache_release_buffer(buffer32_2, buffer_size, type_size);
    mem_cache_release_buffer(buffer32_3, buffer_size, type_size);
    mem_cache_release_buffer(buffer32_4, buffer_size, type_size);
    mem_cache_release_buffer(buffer32_5, buffer_size, type_size);
    mem_cache_release_buffer(buffer32_7, buffer_size, type_size);
    mem_cache_release_buffer(buffer32_6, buffer_size, type_size);
    mem_cache_release_buffer(buffer32_8, buffer_size, type_size);

    print("%sAllocating:%s:ALL BUFFERS AGAIN%s\n", yg, TO_STRING(buffer32_1), CLEAR);
    buffer32_1 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    buffer32_2 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    buffer32_3 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    buffer32_4 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    buffer32_5 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    buffer32_6 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    buffer32_7 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);
    buffer32_8 = (buffer_type_t*) mem_cache_alloc_buffer(buffer_size, type_size);


    mem_cache_release_buffer(buffer32_8, buffer_size, type_size);
    mem_cache_release_buffer(buffer32_1, buffer_size, type_size);
    mem_cache_release_buffer(buffer32_7, buffer_size, type_size);
    mem_cache_release_buffer(buffer32_2, buffer_size, type_size);
    mem_cache_release_buffer(buffer32_3, buffer_size, type_size);
    mem_cache_release_buffer(buffer32_6, buffer_size, type_size);
    mem_cache_release_buffer(buffer32_4, buffer_size, type_size);
    mem_cache_release_buffer(buffer32_5, buffer_size, type_size);

    print_mem_cache_list();
    print_all_slabs_free_list();

    memman_terminate();
}
