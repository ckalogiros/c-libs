/*
 * Created by ckalo on 1/1/26.
 */

#include "test_performance.h"
#include "slab_allocator.h"
#include "mem_man_interface.h"
#include "sparse_arr_internals.h"
#include "logger.h"
#include "precision_timer.h"
#include "precision_timer_logger.h"
#include "precision_timer_struct.h"
#include "slab_allocator_internals.h"
#include "stack_allocator.h"
#include "string_internals.h"

void
timer_print(struct timer* t, const uint32 repeat, const uint32 num_allocations,
            const uint32 sparse_arr_size, const uint32 type_size, const char* name)
{
    print("%s--- Perf Timer ---%s\n", CYAN, CLEAR);
    print("%s%s%s\n", GREEN, name, CLEAR);
    const uint32_t avg = perf_timer_get_avg(t);
    print("  avg: %s%d%s nanosec\n", YELLOW, avg, CLEAR);
    print(
        "  repeat: %s%u%s"
        "  num_allocations: %s%u%s\n"
        "  sparse_arr_size: %s%u%s"
        "  type_size: %s%u%s\n",
        CYAN, repeat, CLEAR,
        CYAN, num_allocations, CLEAR,
        CYAN, sparse_arr_size, CLEAR,
        CYAN, type_size, CLEAR
    );

    perf_timer_reset(t);
}


void
test_perf_slab_allocation_release()
{
    const uint32 repeat = 1000;
    const uint32 num_allocations = 8;

    struct timer* t = timer_create_perf_timer();

    uint16 slab_inst_id = slab_allocator_create_new_instance(32, "test_mem_cache_alloc_release_slabs");

    {
        const uint32 sparse_arr_size = 32;
        const uint32 type_size = sizeof(uint32);

        print_test("Slab Allocator",
            " . Allocate slab buffers\n"
            " . \"Color\" them with a character\n"
            " . Check slab buffer insert() function\n"
            "   that inserts elements (coloring)\n"
            "   in the boundaries of each slab\n"
            " slab_size:   %s%u bytes%s\n"
            " type_size:   %s%u bytes%s\n"
            " sparse_arr_size: %s%u%s elems\n"
            " repeat: %s%u%s\n"
            " num_allocations: %s%u%s\n",
            ry, sparse_arr_size*type_size, CLEAR,
            ry, type_size, CLEAR,
            ry, sparse_arr_size, CLEAR,
            ry, repeat, CLEAR,
            ry, num_allocations, CLEAR
        );

        perf_timer_start(t);
        for(uint32 r = 0; r < repeat; r++) {
            uint32* buffer32_1 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
            uint32* buffer32_2 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
            uint32* buffer32_3 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
            uint32* buffer32_4 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
            uint32* buffer32_5 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
            uint32* buffer32_6 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
            uint32* buffer32_7 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
            uint32* buffer32_8 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");

            slab_allocator_release(slab_inst_id, buffer32_8, sparse_arr_size, type_size);
            slab_allocator_release(slab_inst_id, buffer32_1, sparse_arr_size, type_size);
            slab_allocator_release(slab_inst_id, buffer32_7, sparse_arr_size, type_size);
            slab_allocator_release(slab_inst_id, buffer32_2, sparse_arr_size, type_size);
            slab_allocator_release(slab_inst_id, buffer32_3, sparse_arr_size, type_size);
            slab_allocator_release(slab_inst_id, buffer32_6, sparse_arr_size, type_size);
            slab_allocator_release(slab_inst_id, buffer32_4, sparse_arr_size, type_size);
            slab_allocator_release(slab_inst_id, buffer32_5, sparse_arr_size, type_size);
        }

        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = sparse_arr_size, .flags = PERF_T_USE_REPEAT|PERF_T_ALL
        }));
        perf_timer_reset(t);
    }

    // {
    //     const uint32 sparse_arr_size = 64;
    //     const uint32 type_size = sizeof(uint32);
    //
    //     print_test("Slab Allocator",
    //         " . Allocate slab buffers\n"
    //         " . \"Color\" them with a character\n"
    //         " . Check slab buffer insert() function\n"
    //         "   that inserts elements (coloring)\n"
    //         "   in the boundaries of each slab\n"
    //         " slab_size:   %s%u bytes%s\n"
    //         " type_size:   %s%u bytes%s\n"
    //         " sparse_arr_size: %s%u%s elems\n"
    //         " repeat: %s%u%s\n"
    //         " num_allocations: %s%u%s\n",
    //         ry, sparse_arr_size*type_size, CLEAR,
    //         ry, type_size, CLEAR,
    //         ry, sparse_arr_size, CLEAR,
    //         ry, repeat, CLEAR,
    //         ry, num_allocations, CLEAR
    //     );
    //
    //     perf_timer_start(t);
    //     for(uint32 r = 0; r < repeat; r++) {
    //         uint32* buffer32_1 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_2 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_3 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_4 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_5 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_6 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_7 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_8 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //
    //         slab_allocator_release(slab_inst_id, buffer32_8, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_1, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_7, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_2, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_3, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_6, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_4, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_5, sparse_arr_size, type_size);
    //     }
    //
    //     perf_timer_stop(t);
    //     perf_timer_print(timer_print_config((timer_print_params){
    //         .t = t, .repeat = repeat, .buf_capacity = sparse_arr_size, .flags = PERF_T_USE_REPEAT|PERF_T_ALL
    //     }));
    //     perf_timer_reset(t);
    // }
    //
    // {
    //     const uint32 sparse_arr_size = 128;
    //     const uint32 type_size = sizeof(uint32);
    //
    //     print_test("Slab Allocator - performance",
    //         " . Measure allocation-release\n"
    //         " slab_size:   %s%u bytes%s\n"
    //         " type_size:   %s%u bytes%s\n"
    //         " sparse_arr_size: %s%u%s elems\n"
    //         " repeat: %s%u%s\n"
    //         " num_allocations: %s%u%s\n",
    //         ry, sparse_arr_size*type_size, CLEAR,
    //         ry, type_size, CLEAR,
    //         ry, sparse_arr_size, CLEAR,
    //         ry, repeat, CLEAR,
    //         ry, num_allocations, CLEAR
    //     );
    //
    //     perf_timer_start(t);
    //     for(uint32 r = 0; r < repeat; r++) {
    //         uint32* buffer32_1 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_2 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_3 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_4 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_5 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_6 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_7 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_8 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //
    //         slab_allocator_release(slab_inst_id, buffer32_8, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_1, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_7, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_2, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_3, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_6, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_4, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_5, sparse_arr_size, type_size);
    //     }
    //
    //     perf_timer_stop(t);
    //     perf_timer_print(timer_print_config((timer_print_params){
    //         .t = t, .repeat = repeat, .buf_capacity = sparse_arr_size, .flags = PERF_T_USE_REPEAT|PERF_T_ALL
    //     }));
    //     perf_timer_reset(t);
    // }
    //
    // {
    //     const uint32 sparse_arr_size = 256;
    //     const uint32 type_size = sizeof(uint32);
    //
    //     print_test("Slab Allocator",
    //         " . Allocate slab buffers\n"
    //         " . \"Color\" them with a character\n"
    //         " . Check slab buffer insert() function\n"
    //         "   that inserts elements (coloring)\n"
    //         "   in the boundaries of each slab\n"
    //         " slab_size:   %s%u bytes%s\n"
    //         " type_size:   %s%u bytes%s\n"
    //         " sparse_arr_size: %s%u%s elems\n"
    //         " repeat: %s%u%s\n"
    //         " num_allocations: %s%u%s\n",
    //         ry, sparse_arr_size*type_size, CLEAR,
    //         ry, type_size, CLEAR,
    //         ry, sparse_arr_size, CLEAR,
    //         ry, repeat, CLEAR,
    //         ry, num_allocations, CLEAR
    //     );
    //
    //     perf_timer_start(t);
    //     for(uint32 r = 0; r < repeat; r++) {
    //         uint32* buffer32_1 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_2 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_3 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_4 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_5 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_6 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_7 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //         uint32* buffer32_8 = (uint32*) slab_allocator_alloc(slab_inst_id, sparse_arr_size, type_size, "test_perf_slab_allocation_release()");
    //
    //         slab_allocator_release(slab_inst_id, buffer32_8, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_1, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_7, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_2, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_3, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_6, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_4, sparse_arr_size, type_size);
    //         slab_allocator_release(slab_inst_id, buffer32_5, sparse_arr_size, type_size);
    //     }
    //
    //     perf_timer_stop(t);
    //     perf_timer_print(timer_print_config((timer_print_params){
    //         .t = t, .repeat = repeat, .buf_capacity = sparse_arr_size, .flags = PERF_T_USE_REPEAT|PERF_T_ALL
    //     }));
    //     perf_timer_reset(t);
    // }


    // mem_cache_table_destroy();
    
}


void
test_perf_slab_vs_stack()
{
    // const uint32 repeat = 800000;
    const uint32 repeat = 8000;
    const uint32 num_allocations = 8;
    const uint32 num_pages = 32;

    sparse_arr_table_init(num_pages);
    struct timer* t = timer_create_perf_timer();



    /* TEST: type:uint32, capacity:64 */
    {
        const uint32 num_elems = 512;
        const uint32 type_size = sizeof(uint16);

        print_test("Slab Allocator VS Stack Allocator",
            " . use allocate() function\n"
            " . use release() function\n"
            " type_size:   %s%u bytes%s\n"
            " num_elems: %s%u%s elems\n"
            " repeat: %s%u%s\n"
            " num allocs/releases: %s%u%s per iteration\n",
            ry, type_size, CLEAR,
            ry, num_elems, CLEAR,
            ry, repeat, CLEAR,
            ry, num_allocations, CLEAR
        );


        const memid_t slab_inst_id = slab_allocator_create_new_instance(num_pages, "test_perf_slab_vs_stack()");
        perf_timer_start(t);
        sparse_arr_t* buffer[num_allocations];
        for(uint32 r = 0; r < repeat; r++) {
            for(uint32 i = 0; i < num_allocations; i++) {
                buffer[i] = slab_allocator_alloc(slab_inst_id, num_elems, type_size, "test_perf_slab_vs_stack()");
            }
            for(uint32 i = 0; i < num_allocations; i++) {
                slab_allocator_release(slab_inst_id, buffer[i], num_elems, type_size);
            }
        }

        perf_timer_stop(t);
        const uint32 slab_timer_avg = perf_timer_calc_avg_from_count(t, repeat);
        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = num_elems, .flags = PERF_T_USE_REPEAT|PERF_T_ALL
        }));
        perf_timer_reset(t);

        const memid_t stack_inst_id = stack_allocator_create_new_instance(num_pages);
        perf_timer_start(t);
        for(uint32 r = 0; r < repeat; r++) {
            for(uint32 i = 0; i < num_allocations; i++) {
                stack_allocator_alloc(stack_inst_id, num_elems, type_size);
            }
            for(uint32 i = 0; i < num_allocations; i++) {
                stack_allocator_release(stack_inst_id);
            }
        }

        perf_timer_stop(t);
        const uint32 stack_timer_avg = perf_timer_calc_avg_from_count(t, repeat);
        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = num_elems, .flags = PERF_T_USE_REPEAT|PERF_T_ALL
        }));
        perf_timer_reset(t);

        print(
            "ratio: %f\n",
            (float)LESS_THAN(slab_timer_avg, stack_timer_avg) /
            (float)GREATER_THAN(slab_timer_avg, stack_timer_avg)
        );

    }


    

}