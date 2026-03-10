/*
 * Created by ckalo on 1/22/26.
 */


#include "logger.h"
#include "mem_cache.h"
// #include "mem_log.h"
// #include "mem_man_interface.h"
#include "my_types.h"
#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <unistd.h>

#include "test_performance.h"
#include "test_slab_allocator.h"
#include "test_string_allocator.h"

int32
has_input(void)
{
    struct pollfd pfd = {.fd = STDIN_FILENO, .events = POLLIN};
    return poll(&pfd, 1, 0) > 0; // 0 timeout = non-blocking
}





int
main(void)
{
    // test_check_alloc_extend_pages(1);
    // test_mem_cache_alloc_release_slabs();



    // test performance measurements
    // test_perf_slab_allocation_release();


    // string allocator
    test_string_mem_cache_alloc_release_slabs();



    return 0;
}


