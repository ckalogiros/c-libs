/*
 * Created by ckalo on 1/22/26.
 */


#include "logger.h"
#include "slab_allocator_mcache.h"
#include "my_types.h"
#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <unistd.h>
#include "test_performance.h"
#include "test_slab_allocator.h"
#include "test_stack_allocator.h"

int32
has_input(void)
{
    struct pollfd pfd = {.fd = STDIN_FILENO, .events = POLLIN};
    return poll(&pfd, 1, 0) > 0; // 0 timeout = non-blocking
}





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



    /***************************************************************************************************
    * STACK ALLOCATOR TESTS
     **************************************************************************************************/
    test_stack_allocator();


    return 0;
}


