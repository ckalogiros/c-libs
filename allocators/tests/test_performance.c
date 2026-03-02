/*
 * Created by ckalo on 1/1/26.
 */

#include "test_performance.h"
#include "mem_cache.h"
#include "mem_man_interface.h"
#include "buffer_internals.h"
#include "logger.h"
#include "precision_timer.h"
#include "precision_timer_logger.h"
#include "precision_timer_struct.h"
#include "slab_allocator_internals.h"

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
    memman_init();
    mem_cache_table_init();
    buffer_init_table();
    timer_init_table();

    const uint32 repeat = 1000000;
    const uint32 num_allocations = 8;



    struct timer* t = timer_create_perf_timer();

    {
        const uint32 buffer_size = 32;
        const uint32 type_size = sizeof(uint32);

        print_test("Slab Allocator",
            " . Allocate slab buffers\n"
            " . \"Color\" them with a character\n"
            " . Check slab buffer insert() function\n"
            "   that inserts elements (coloring)\n"
            "   in the boundaries of each slab\n"
            " slab_size:   %s%u bytes%s\n"
            " type_size:   %s%u bytes%s\n"
            " buffer_size: %s%u%s elems\n"
            " repeat: %s%u%s\n"
            " num_allocations: %s%u%s\n",
            ry, buffer_size*type_size, CLEAR,
            ry, type_size, CLEAR,
            ry, buffer_size, CLEAR,
            ry, repeat, CLEAR,
            ry, num_allocations, CLEAR
        );

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

        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = buffer_size, .flags = PERF_T_USE_REPEAT|PERF_T_ALL
        }));
        perf_timer_reset(t);
    }

    {
        const uint32 buffer_size = 64;
        const uint32 type_size = sizeof(uint32);

        print_test("Slab Allocator",
            " . Allocate slab buffers\n"
            " . \"Color\" them with a character\n"
            " . Check slab buffer insert() function\n"
            "   that inserts elements (coloring)\n"
            "   in the boundaries of each slab\n"
            " slab_size:   %s%u bytes%s\n"
            " type_size:   %s%u bytes%s\n"
            " buffer_size: %s%u%s elems\n"
            " repeat: %s%u%s\n"
            " num_allocations: %s%u%s\n",
            ry, buffer_size*type_size, CLEAR,
            ry, type_size, CLEAR,
            ry, buffer_size, CLEAR,
            ry, repeat, CLEAR,
            ry, num_allocations, CLEAR
        );

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

        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = buffer_size, .flags = PERF_T_USE_REPEAT|PERF_T_ALL
        }));
        perf_timer_reset(t);
    }

    {
        const uint32 buffer_size = 128;
        const uint32 type_size = sizeof(uint32);

        print_test("Slab Allocator - performance",
            " . Measure allocation-release\n"
            " slab_size:   %s%u bytes%s\n"
            " type_size:   %s%u bytes%s\n"
            " buffer_size: %s%u%s elems\n"
            " repeat: %s%u%s\n"
            " num_allocations: %s%u%s\n",
            ry, buffer_size*type_size, CLEAR,
            ry, type_size, CLEAR,
            ry, buffer_size, CLEAR,
            ry, repeat, CLEAR,
            ry, num_allocations, CLEAR
        );

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

        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = buffer_size, .flags = PERF_T_USE_REPEAT|PERF_T_ALL
        }));
        perf_timer_reset(t);
    }

    {
        const uint32 buffer_size = 256;
        const uint32 type_size = sizeof(uint32);

        print_test("Slab Allocator",
            " . Allocate slab buffers\n"
            " . \"Color\" them with a character\n"
            " . Check slab buffer insert() function\n"
            "   that inserts elements (coloring)\n"
            "   in the boundaries of each slab\n"
            " slab_size:   %s%u bytes%s\n"
            " type_size:   %s%u bytes%s\n"
            " buffer_size: %s%u%s elems\n"
            " repeat: %s%u%s\n"
            " num_allocations: %s%u%s\n",
            ry, buffer_size*type_size, CLEAR,
            ry, type_size, CLEAR,
            ry, buffer_size, CLEAR,
            ry, repeat, CLEAR,
            ry, num_allocations, CLEAR
        );

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

        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = buffer_size, .flags = PERF_T_USE_REPEAT|PERF_T_ALL
        }));
        perf_timer_reset(t);
    }


    mem_cache_table_destroy();
    memman_terminate();
}
