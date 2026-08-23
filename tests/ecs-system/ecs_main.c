/*
 * Created by ckalo on 1/27/26.
 */

#include "buffer_arr_internals.h"
#include "component.h"
#include "ecs.h"
#include "entity.h"
#include "logger.h"
#include "mem_man_interface.h"
#include "precision_timer.h"
#include "tests.h"
#include "my_macros.h"


int
main(void)
{

    const uint32 capacity = 1024*8;
    const uint32 num_buffers = 14;
    const uint32 num_pages = (capacity * num_buffers * SIZE_F32) / PAGE_SIZE;
    print("%s NUM_PAGES = %u\nRAW_BYTES = %u %s\n",
        YELLOW, num_pages, capacity * num_buffers * SIZE_F32, CLEAR);

    memman_init();
    buffer_arr_init_table(num_pages);
    timer_init_table();

    init_entity_table(capacity*4);
    init_pos_table(capacity);
    init_scale_table(capacity);
    init_col_table(capacity);
    init_all_attribs_table(capacity);

    // test_ecs();

    // test_create_entities(256);

    /// For less repeat times change the "static const uint32 REPEAT "
    { // Test: add all 3 components in 3 way. SEE:test_ecs_print_3_loops_test
        test_ecs_print_3_loops_test();
        const uint32 avg1 =  test_components_seperate_add(capacity);
        const uint32 avg2 =  test_components_together_add(capacity);
        const uint32 avg3 =  test_components_all_attribs(capacity);

        /// PRINT RESULTS
        {
            // float ratio12 = (float)GET_MIN(avg1, avg2) / (float)GET_MAX(avg1, avg2);
            // float ratio13 = (float)GET_MIN(avg1, avg3) / (float)GET_MAX(avg1, avg3);
            // float ratio23 = (float)GET_MIN(avg3, avg2) / (float)GET_MAX(avg3, avg2);
            float ratio12 = ((float)avg1 / (float)avg2) * 100;
            float ratio21 = ((float)avg2 / (float)avg1) * 100;
            float ratio13 = ((float)avg1 / (float)avg3) * 100;
            float ratio31 = ((float)avg3 / (float)avg1) * 100;
            float ratio23 = ((float)avg2 / (float)avg3) * 100;
            float ratio32 = ((float)avg3 / (float)avg2) * 100;
            print("repeat: %u\n", REPEAT);
            print("avg 1: %u | avg 2: %u | avg 3: %u\n", avg1, avg2, avg3);
            print("buffer size: %u bytes\n", capacity);
            print("    1    | 2     | 3 \n");
            print(" 1: %.1f  | %.1f | %.1f\n", 0.0f, ratio12, ratio13);
            print(" 2: %.1f | %.1f   | %.1f\n", ratio21, 0.0f, ratio23);
            print(" 3: %.1f | %.1f  | %.1f\n", ratio31, ratio32, 0.0f);
            // print(" ratio 1-2: %.1f %\n ratio 1-3: %.1f %\n ratio 2-3: %.1f % \n", ratio12, ratio13, ratio23);
        }
    }



    // test_perf_ecs_insert_to_comp();

    {// Test: add all 3 components in 3 way. Different buffer capacities
        // print("%sTEST: test_perf_ecs_insert_to_comp_helper%s\n", GREEN, CLEAR);
        // uint32 repeat = 100000;
        //
        //
        // test_ecs_print_3_loops_test();

        // uint32 count = 128;
        // print("Entities count:%u\n", count);
        // test_perf_ecs_insert_to_comp_helper(count, repeat);
        //
        // count = 256;
        // print("Entities count:%u\n", count);
        // test_perf_ecs_insert_to_comp_helper(count, repeat);
        //
        // count = 512;
        // print("Entities count:%u\n", count);
        // test_perf_ecs_insert_to_comp_helper(count, repeat);
        //
        // count = 1024;
        // print("Entities count:%u\n", count);
        // test_perf_ecs_insert_to_comp_helper(count, repeat);
        //
        // // count = 2048;
        // // print("Entities count:%u\n", count);
        // // test_perf_ecs_insert_to_comp_helper(count, repeat);



    }


    memman_terminate();
    return 0;
}
