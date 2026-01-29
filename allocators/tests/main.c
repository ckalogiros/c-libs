/*
 * Created by ckalo on 1/22/26.
 */


#include "logger.h"
#include "mem_cache.h"
#include "mem_log.h"
#include "mem_man.h"
#include "mem_man_internals.h"
#include "my_types.h"
#include <stdio.h>
#include "my_macros.h"
#include "test_allocator.h"
#include "test_performance.h"

#include <poll.h>
#include <unistd.h>

#include "slab_allocator_log.h"
#include "std_input.h"
#include "std_input_public.h"

int32 has_input() {
    struct pollfd pfd = { .fd = STDIN_FILENO, .events = POLLIN };
    return poll(&pfd, 1, 0) > 0; // 0 timeout = non-blocking
}

uint32 test_id = 0;

void print_free_list() { mem_log_print_free_list(free_list_get_head()); }

void print_used_memory() { mem_log_print_all_used_mem(); }

void test_mem_init_alloc_dealloc();
void test_slab_allocator_alloc_release();


int main(void)
{
    // test_mem_init_alloc_dealloc();

    test_slab_allocator_alloc_release();

    // test performance measurements
    // test_perf_slab_allocation_release();




    struct pollfd pfd = { .fd = STDIN_FILENO, .events = POLLIN };
    char c = '\0';
    const uint16 read_num_bytes = 1;
    const uint16 num_discriptors = 1;
    bool running = TRUE;
    const uint16 buffer_size = 32;

    // ste callbacks
    void* list_head = (void*)free_list_get_head();
    op_set_callback((void*)mem_log_print_free_list, list_head, TO_STRING(mem_log_print_free_list));
    op_set_callback((void*)print_all_slabs_free_list, (void*)0, TO_STRING(print_all_slabs_free_list));


    // while (running) {
    //
    //     if (poll(&pfd, num_discriptors, 0) > 0) {
    //
    //         uint32 k = 0;
    //         char buffer[32] = {0};
    //
    //         // while read function returns input, read characters to buffer
    //         while (1){
    //
    //             read(STDIN_FILENO, &c, read_num_bytes);
    //             if (c == '\n' || k > buffer_size) break;
    //             buffer[k++] = c;
    //         }
    //
    //         buffer[k] = '\0';
    //
    //         print("input: %s\n", buffer);
    //
    //         running = find_op(buffer);
    //     }
    // }

    return 0;
}

void test_slab_allocator_alloc_release()
{
    memory_manager_init(5);
    mem_log_print_mem();

    const uint32 buffer_size = 16;
#define buffer_type_t uint64
    const uint32 type_size   = sizeof(buffer_type_t);

    // different buffers to allocate
    buffer_type_t* buffer32_1 = NULL;
    buffer_type_t* buffer32_2 = NULL;
    buffer_type_t* buffer32_3 = NULL;
    buffer_type_t* buffer32_4 = NULL;
    buffer_type_t* buffer32_5 = NULL;
    buffer_type_t* buffer32_6 = NULL;
    buffer_type_t* buffer32_7 = NULL;
    buffer_type_t* buffer32_8 = NULL;

    buffer32_1 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_1), REDUCE_ADDR(buffer32_1, F6), CLEAR);
    buffer32_2 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_2), REDUCE_ADDR(buffer32_2, F6), CLEAR);
    buffer32_3 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_3), REDUCE_ADDR(buffer32_3, F6), CLEAR);
    buffer32_4 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_4), REDUCE_ADDR(buffer32_4, F6), CLEAR);
    buffer32_5 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_5), REDUCE_ADDR(buffer32_5, F6), CLEAR);
    buffer32_6 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_6), REDUCE_ADDR(buffer32_6, F6), CLEAR);
    buffer32_7 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
    print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_7), REDUCE_ADDR(buffer32_7, F6), CLEAR);
    buffer32_8 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
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
    buffer32_1 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
    buffer32_2 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
    buffer32_3 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
    buffer32_4 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
    buffer32_5 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
    buffer32_6 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
    buffer32_7 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);
    buffer32_8 = (buffer_type_t*)mem_cache_alloc_buffer(buffer_size, type_size);


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

    memory_manager_terminate();


}

void test_mem_init_alloc_dealloc()
{
    // print test info
    {
        const char* test_name = "Allocate memory block\
\nSet up used memory control\
\nSet up free list control\
\nAllocate-release-use memory pages\
\nDestroy memory block";
        printf("-- -- -- TEST:%u -- -- --\n", test_id);
        print("%s %s %s\n", ry, test_name, CLEAR);
        printf("-- -- -- TEST:%u -- -- --\n\n", test_id);
        test_id++;
    }

    memory_manager_init(4);
    mem_log_print_mem();
    print_used_memory();
    print_free_list();

    // use memory in buffers sizeof(page)
    {
        const uint32 num_allocations = 4; // total allocations
        const uint32 num_pages       = 1; // pages per allocation
        char ch                      = 'a';
        for (uint32 i = 0; i < num_allocations; i++) {
            const pageid_t buffer_id = allocator_test_alloc(num_pages);
            uint64* const buffer     = Temp_get_address_by_id(buffer_id);
            const uint32 count       = (PAGE_SIZE * num_pages) / sizeof(uint64);
            for (uint32 j = 0; j < count; j++)
                buffer[j] = (uint64)ch;
            ch++;
        }
        print("\n");
    }

    print_used_memory();

    // release pages - inserts to free list
    {
        // manually pass id of allocated object(should be 'buffer_id:see above')
        TEMP_free_list_insert(2);
        TEMP_free_list_insert(0);
        TEMP_free_list_insert(1);
        TEMP_free_list_insert(3);
        print("\n");
    }

    // removes
    // // todo: release memory
    // // todo: enable perf timers, and all other external

    memory_manager_terminate();
}