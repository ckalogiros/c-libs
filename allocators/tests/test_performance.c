/*
 * Created by ckalo on 1/1/26.
 */

#include "test_performance.h"
#include "mem_cache.h"
#include "mem_man_interface.h"
#include "mem_log.h"
#include "buffer_internals.h"
#include "logger.h"
#include "precision_timer.h"

void
timer_print(struct timer* t, const uint32 repeat, const uint32 num_allocations,
            const uint32 buffer_size, const uint32 type_size, const char* name)
{
    print("%s--- Perf Timer ---%s\n", CYAN, CLEAR);
    print("%s%s%s\n", GREEN, name, CLEAR);
    const uint32_t avg = perf_timer_get_avg(t);
    print("  avg: %s%d%s nanosec\n", YELLOW, avg, CLEAR);
    print(
        "  repeat: %s%u%s"
        "  num_allocations: %s%u%s\n"
        "  buffer_size: %s%u%s"
        "  type_size: %s%u%s\n",
        CYAN, repeat, CLEAR,
        CYAN, num_allocations, CLEAR,
        CYAN, buffer_size, CLEAR,
        CYAN, type_size, CLEAR
    );

    perf_timer_reset(t);
}


void
test_perf_slab_allocation_release()
{
    mem_init();

    buffer_init_table();

    timer_init_table();

    const uint32 repeat = 5000000;
    const uint32 num_allocations = 8;


    // print("%sAllocating:%s:%x%s\n", yg, TO_STRING(buffer32_1), REDUCE_ADDR(buffer32_1, F6), CLEAR);

    struct timer* t = timer_create_perf_timer();

    {
        const uint32 buffer_size = 32;
        const uint32 type_size = sizeof(uint32);
        perf_timer_start(t);

        for(uint32 r = 0; r < repeat; r++) {
            uint32* buffer32_1 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_2 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_3 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_4 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_5 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_6 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_7 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_8 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);

            mem_cache_release_buffer(buffer32_8, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_1, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_7, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_2, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_3, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_6, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_4, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_5, buffer_size, type_size);
        }

        perf_timer_stop_with_count(repeat * num_allocations, t);
        timer_print(t, repeat, num_allocations, buffer_size, type_size, "size: 32 bytes");
    }

    {
        const uint32 buffer_size = 64;
        const uint32 type_size = sizeof(uint32);
        perf_timer_start(t);

        for(uint32 r = 0; r < repeat; r++) {
            uint32* buffer32_1 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_2 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_3 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_4 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_5 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_6 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_7 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_8 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);

            mem_cache_release_buffer(buffer32_8, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_1, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_7, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_2, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_3, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_6, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_4, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_5, buffer_size, type_size);
        }

        perf_timer_stop_with_count(repeat * num_allocations, t);
        timer_print(t, repeat, num_allocations, buffer_size, type_size, "size: 64 bytes");
    }


    {
        const uint32 buffer_size = 128;
        const uint32 type_size = sizeof(uint32);
        perf_timer_start(t);

        for(uint32 r = 0; r < repeat; r++) {
            uint32* buffer32_1 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_2 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_3 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_4 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_5 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_6 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_7 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_8 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);

            mem_cache_release_buffer(buffer32_8, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_1, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_7, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_2, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_3, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_6, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_4, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_5, buffer_size, type_size);
        }

        perf_timer_stop_with_count(repeat * num_allocations, t);
        timer_print(t, repeat, num_allocations, buffer_size, type_size, "size: 128 bytes");
    }


    {
        const uint32 buffer_size = 256;
        const uint32 type_size = sizeof(uint32);
        perf_timer_start(t);

        for(uint32 r = 0; r < repeat; r++) {
            uint32* buffer32_1 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_2 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_3 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_4 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_5 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_6 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_7 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);
            uint32* buffer32_8 = (uint32*) mem_cache_alloc_buffer(buffer_size, type_size);

            mem_cache_release_buffer(buffer32_8, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_1, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_7, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_2, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_3, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_6, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_4, buffer_size, type_size);
            mem_cache_release_buffer(buffer32_5, buffer_size, type_size);
        }

        perf_timer_stop_with_count(repeat * num_allocations, t);
        timer_print(t, repeat, num_allocations, buffer_size, type_size, "size: 512 bytes");
    }


    memman_terminate();
}
