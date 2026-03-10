/*
 * Created by ckalo on 1/27/26.
 */

#include "buffer_internals.h"
#include "component.h"
#include "ecs.h"
#include "entity.h"
#include "logger.h"
#include "mem_man_interface.h"
#include "precision_timer.h"
#include "tests.h"


int
main(void)
{

    memman_init();
    buffer_init_table();
    timer_init_table();

    init_entity_table();
    init_pos_table();
    init_scale_table();
    init_col_table();
    init_all_attribs_table();

    // test_ecs();

    // test_create_entities(256);

    { // Test: add all 3 components in 3 way. SEE:test_ecs_print_3_loops_test
        const uint32 num = 128;
        test_ecs_print_3_loops_test();
        test_components_seperate_add(num);
        test_components_together_add(num);
        test_components_all_attribs(num);
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
