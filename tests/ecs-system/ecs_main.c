/*
 * Created by ckalo on 1/27/26.
 */

#include "logger.h"
#include "mem_man_interface.h"
#include "tests.h"


int
main(void)
{
    // test_ecs();

    // test_create_entities(256);

    uint32 count = 128;
    // test_components_seperate_add(count);
    // test_components_together_add(count);
    // test_components_all_attribs(count);

    test_perf_ecs_insert_to_comp();

    // print("Entities count:%u\n", count);
    // test_perf_ecs_insert_to_comp_helper(count);
    //
    // count = 256;
    // print("Entities count:%u\n", count);
    // test_perf_ecs_insert_to_comp_helper(count);
    //
    // count = 512;
    // print("Entities count:%u\n", count);
    // test_perf_ecs_insert_to_comp_helper(count);
    //
    // count = 124;
    // print("Entities count:%u\n", count);
    // test_perf_ecs_insert_to_comp_helper(count);


    return 0;
}
