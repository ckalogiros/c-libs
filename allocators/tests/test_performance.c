/*
 * Created by ckalo on 1/1/26.
 */

#include "test_performance.h"

#include "logger.h"
#include "mem_cache.h"
#include "mem_log.h"
#include "mem_man.h"
#include "my_macros.h"
#include "precision_timer.h"

void timer_print(const uint32 repeat,
    const uint32 num_allocations,
    const uint32 buffer_size,
    const uint32 type_size)
{
    print("%s--- Timer ---%s\n", CYAN, CLEAR);
    const uint32_t avg = perf_timer_get_avg();
    print("  avg: %s%d%s nanosec\n", YELLOW, avg, CLEAR);
    print(
        "  repeat: %s%u%s\n"
        "  num_allocations: %s%u%s\n"
        "  buffer_size: %s%u%s\n"
        "  type_size: %s%u%s\n",
        CYAN, repeat, CLEAR,
        CYAN, num_allocations, CLEAR,
        CYAN, buffer_size, CLEAR,
        CYAN, type_size, CLEAR
        );

    perf_timer_reset();
}


void test_perf_slab_allocation_release()
{
    memory_manager_init(4);
    mem_log_print_mem();

    const uint32 repeat = 5000000;
    const uint32 num_allocations = 8;
    const uint32 buffer_size = 32;
    const uint32 type_size = sizeof(uint32);

    // different buffers to allocate
    /*volatile*/ uint32* buffer32_1 = NULL;
    /*volatile*/ uint32* buffer32_2 = NULL;
    /*volatile*/ uint32* buffer32_3 = NULL;
    /*volatile*/ uint32* buffer32_4 = NULL;
    /*volatile*/ uint32* buffer32_5 = NULL;
    /*volatile*/ uint32* buffer32_6 = NULL;
    /*volatile*/ uint32* buffer32_7 = NULL;
    /*volatile*/ uint32* buffer32_8 = NULL;

    // print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_1), REDUCE_ADDR(buffer32_1, F6), CLEAR);

    perf_timer_start();

    for (uint32 r = 0; r < repeat; r++)
    {
        buffer32_1 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
        buffer32_2 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
        buffer32_3 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
        buffer32_4 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
        buffer32_5 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
        buffer32_6 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
        buffer32_7 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
        buffer32_8 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);

        mem_cache_release_buffer(buffer32_8, buffer_size, type_size);
        mem_cache_release_buffer(buffer32_1, buffer_size, type_size);
        mem_cache_release_buffer(buffer32_7, buffer_size, type_size);
        mem_cache_release_buffer(buffer32_2, buffer_size, type_size);
        mem_cache_release_buffer(buffer32_3, buffer_size, type_size);
        mem_cache_release_buffer(buffer32_6, buffer_size, type_size);
        mem_cache_release_buffer(buffer32_4, buffer_size, type_size);
        mem_cache_release_buffer(buffer32_5, buffer_size, type_size);

    }

    perf_timer_stop_with_count(repeat*num_allocations);
    timer_print(repeat, num_allocations, buffer_size, type_size);

    memory_manager_terminate();
}