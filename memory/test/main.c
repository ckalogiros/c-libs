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

    mem_init();
    // temp_print_mem_structs_sizes();
    // memman_init();
    // // struct memhnd_t hnd = mem_alloc(1, MEM_CHUNK_ALLOCATOR_SLAB);
    // // mem_log_print_free_list_of_chunk(hnd.chunk_id);
    // // mem_release(hnd);
    // // mem_test();
    // memman_terminate();

    // perf_mem_big_chunk_allocation();

    // Some bit tests, see everything is working
    // test_calc_bitmask_from_start_end_bits();
    // test_bitfield_set();
    // test_macro_impl_bitfield_set();


    // struct memhnd_t memhnd = memman_alloc_pages(1, MEM_CHUNK_ALLOCATOR_SLAB);

    // mem_log_print_free_list_table();

    // tests
    // test_alloc_release_insequence(2, TRUE);
    // test_alloc_release_insequence(1, TRUE);
    // test_alloc_release_insequence(1, TRUE);
    // test_alloc_release_insequence(4, TRUE);

    test_alloc_first_release_after(1, TRUE);
    // test_alloc_first_release_after(3, TRUE);


    // test_alloc_first_release_after_backwards(4, TRUE);


    // test_alloc_release_unordered_pages_1();
    // test_alloc_release_unordered_pages_2();
    // test_alloc_release_unordered_pages_3();
    // test_alloc_release_unordered_pages_4();

    // test_alloc_release_mixed_all();
    // test_alloc_release_fragment_mem();

    // test_logger_to_html();

    memman_terminate();

    return 0;
}
