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
#include <string.h>
#include <unistd.h>

#include "mem_man_interface.h"
#include "slab_allocator_internals.h"
#include "slab_allocator_log.h"
#include "test_slab_allocator.h"
// #include "std_input.h"
// #include "std_input_public.h"

int32
has_input(void)
{
    struct pollfd pfd = {.fd = STDIN_FILENO, .events = POLLIN};
    return poll(&pfd, 1, 0) > 0; // 0 timeout = non-blocking
}

uint32 test_id = 0;



void
test_slab_allocator_alloc_release(void);

void test(void);
void test_mem_cache_alloc_release_slabs(void);



int
main(void)
{
    // test();
    test_mem_cache_alloc_release_slabs();


    // test_slab_allocator_alloc_release();

    // test performance measurements
    // test_perf_slab_allocation_release();



    return 0;
}

// some basic initialization of slab allocator
void test(void)
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

/**************************************************************************************************/
// some basic initialization of new implemetation slab allocator


void test_mem_cache_alloc_release_slabs(void)
{
    memman_init();
    mem_chache_table_init();

    print(" %sTEST-slab allocator%s\n", ry, CLEAR);


    const uint8 type_size = sizeof(uint8);
    const uint32 type_size_bytes = type_size*8;
    const buffer_size_t buffer_size = 16;
    const buffer_size_t slab_size = buffer_size*type_size;

    // print(" %sTEST-%s\n", ry, CLEAR);
    const uint32 page_size = PAGE_SIZE;
    const uint32 total_slabs = page_size/slab_size;
    uint8* buffer[page_size/slab_size];

    print(" type size in bytes: %s%u%s\n", YELLOW, type_size_bytes, CLEAR);
    print(" buffer size: %s%u%s\n", YELLOW, buffer_size, CLEAR);
    print(" slab size: %s%u%s\n", YELLOW, slab_size, CLEAR);
    print(" num slabs: %s%u%s\n", YELLOW, total_slabs, CLEAR);

    print("%s-- ALLOCATION + WRITE --%s\n", ry, CLEAR);
    uint32 c = CHAR_A;
    for(uint32 i = 0; i < total_slabs; i++){
        buffer[i] = (uint8*)mem_cache_alloc_buffer(buffer_size, type_size);
        memset(buffer[i], (char)c, slab_size);
        check_buffer(buffer[i], slab_size, c);
        c++;
        if(c>CHAR_END) c = CHAR_A;
    }


    // const void* buffer_start = &(buffer[0][0]);
    const char* buffer_start = (*buffer);
    // 0x7ffff7f1c7f8
    const char* buffer_end = ((*buffer)+(total_slabs*slab_size));
    print(" alloc start: %s%x%s\n", CYAN, REDUCE_ADDR(buffer_start, F5), CLEAR);
    print(" alloc end:   %s%x%s\n", CYAN, REDUCE_ADDR(buffer_end, F5), CLEAR);
    print(" alloc size:  %s%llu%s bytes\n", CYAN, buffer_end-buffer_start, CLEAR);
    if((uint64)buffer_end-(uint64)buffer_start == page_size)
        print(" Allocation size :: %sPASS%s\n", GREEN, CLEAR);
    else print(" Allocation size ::  %sFAIL%s\n", RED, CLEAR);

    print("\n\n");



    /*
     * . release all slabs of the allocated memory cache
     * . use a buffer to store all free slabs addresses for testing
     * . the release slab functions are copies of the implementation(for debugging)
     *   so make sure they are updated on any changes
     */
    uint64 free_buffer[total_slabs] = {};
    print("%s-- DEALLOCATION + ERASE --%s\n", ry, CLEAR);
    for(uint32 i = 0; i < total_slabs; i++){
        mem_cache_release_buffer_test_debug(buffer[i], buffer_size, type_size, free_buffer, i);
    }

    /*
     * . print all free slabs addresses
     * . all slabs ought to be free at this point
     * . if the difference between 2 consecutive slabs
     *   is the size of the slab in bytes: TEST PASSES, else TEST: FAILS
     */
    uint64 dif = free_buffer[0];
    print(" slab_size:%u\n", slab_size);
    for(uint32 i = 1; i < total_slabs; i++) {

        dif = (uint64)free_buffer[i] - dif;

        print("[%s%u%s]"
          " addr:%s%x%s"
          "",
            CYAN, i, CLEAR,
            CYAN, REDUCE_ADDR(free_buffer[i], F6), CLEAR
        );
        if(dif == slab_size) print(" %sPASS%s\n", GREEN, CLEAR);
        else print(" %sFAIL%s\n", RED, CLEAR);

        dif = free_buffer[i];
    }


    mem_cache_table_destroy();
    memman_terminate();
}


void
test_slab_allocator_alloc_release(void)
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

    memman_terminate();
}
