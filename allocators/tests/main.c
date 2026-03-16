/*
 * Created by ckalo on 1/22/26.
 */


#include "test_performance.h"
#include "test_slab_allocator.h"
#include "test_stack_allocator.h"
#include "stack_unit_test.h"


int
main(void)
{

    /***************************************************************************************************
     * SLAB ALLOCATOR TESTS
     **************************************************************************************************/

    // test_check_alloc_extend_pages(1);
    // test_mem_cache_alloc_release_slabs();

    /* test performance measurements */
    // test_perf_slab_allocation_release();
    test_perf_slab_vs_stack();


    /***************************************************************************************************
    * STACK ALLOCATOR TESTS
     **************************************************************************************************/
    // test_stack_allocator();
    // test_stack_allocator_alignment();
    // unit_test_runner_stack_allocator();


    return 0;
}


