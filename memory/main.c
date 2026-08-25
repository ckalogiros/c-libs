/*
 * Created by ckalo on 1/5/26.
 */

// #include "buffer_internals.h"
#include "mem_log.h"
#include "my_types.h"
#include "tests.h"
// #include "mem_test_bitfields.h"
#include "mem_unit_test.h"

uint32 test_id = 0;



int
main(void)
{
    // test_mem_instance_initialization();
    test_mem_page_request();

    // unit_test_runner_slab_allocator();


    // tests
    { // tests algorithm for non contiguous pages
        // memid_t memid = mem_create_inst(8, MEM_TESTS_MEM_ALIGNMENT, "");
        // mem_log_print_free_list(memid);
        //
        // test_mem_alloc();
    }

    test_alloc_release_insequence(2);
    // test_alloc_release_insequence(1);
    // test_alloc_release_insequence(1);
    // test_alloc_release_insequence(4);
    //
    test_alloc_first_release_after(8);
    //
    test_alloc_first_release_after_backwards(4);
    //
    test_alloc_release_mixed_all();
    test_alloc_release_fragment_mem();


    memman_terminate();
    return 0;
}

