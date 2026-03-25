#include <stdio.h>

#include "mem_man_interface.h"
#include "unit_test.h"
#include "test_performance.h"
#include "tests_other.h"
#include "unit_test.h"

int
main(void)
{
    memman_init();
    /***************************************************************************************************
     * SLAB ALLOCATOR TESTS
     **************************************************************************************************/

    test_mem_cache_alloc_release_slabs();
    // test_check_alloc_extend_pages(1);

    /* test performance measurements */
    // test_perf_slab_allocation_release();
    // test_perf_slab_vs_stack();

    // unit_test_runner_slab_allocator();


    /***************************************************************************************************
    * STACK ALLOCATOR TESTS
     **************************************************************************************************/
    // test_stack_allocator();
    // test_stack_allocator_alignment();
    // unit_test_runner_stack_allocator();

    memman_terminate();
    return 0;
}