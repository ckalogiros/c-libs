/*
 * Created by ckalo on 1/5/26.
 */

// #include "buffer_internals.h"
#include "mem_structs.h"
#include "mem_log.h"
#include "mem_man_interface.h"
#include "my_types.h"
#include "precision_timer.h"
#include "precision_timer_logger.h"
#include "tests.h"
#include "mem_test.h"

uint32 test_id = 0;



int
main(void)
{

    mem_table_init();
    mem_create();


    // Some bit tests, see everything is working
    {
        // test_calc_bitmask_from_start_end_bits();
        // test_bitfield_set();
        // test_macro_impl_bitfield_set();
    }


    // tests
    { // basic allocations
        // mem_log_print_free_list(1);
        // test_mem_alloc(3);
        // test_mem_alloc(3);
        // test_mem_alloc(2);
    }
    { // test algorithm for non contiguous pages
        mem_log_print_free_list(1);
        test_mem_alloc1();
    }

    // test_alloc_release_insequence(2, TRUE);
    // test_alloc_release_insequence(1, TRUE);
    // test_alloc_release_insequence(1, TRUE);
    // test_alloc_release_insequence(4, TRUE);

    // test_alloc_first_release_after(1, TRUE);
    // test_alloc_first_release_after(3, TRUE);

    // test_alloc_first_release_after_backwards(4, TRUE);

    // test_alloc_release_mixed_all();
    // test_alloc_release_fragment_mem();


    mem_table_destroy();
    return 0;
}
