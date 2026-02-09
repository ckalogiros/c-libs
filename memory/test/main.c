/*
 * Created by ckalo on 1/5/26.
 */

#include "buffer_internals.h"
#include "mem_structs.h"
#include "mem_log.h"
#include "mem_man.h"
#include "my_types.h"
#include "precision_timer.h"
#include "precision_timer_logger.h"
#include "tests.h"
#include "mem_test.h"

uint32 test_id = 0;



int
main(void)
{

    temp_print_mem_structs_sizes();
    // mem_init();
    // mem_test();
    // temp_print_mem_structs_sizes();
    // perf_mem_big_chunk_allocation();

    // Some bit tests, see everything is working
    // test_calc_bitmask_from_start_end_bits();
    // test_bitfield_set();
    // test_macro_impl_bitfield_set();


    struct memhnd_t memhnd = mms_allocate_pages(1, MEM_CHUNK_ALLOCATOR_SLAB);



    // tests
    // test_alloc_release_insequence_at_start_by_1();
    // test_alloc_release_insequence_at_start_by_2();
    // test_alloc_release_insequence_at_start_by_3();
    // test_alloc_release_insequence_at_start_by_4();

    // test_alloc_release_insequence_at_start_mix();


    // test_alloc_release_unordered_at_start_by_1();
    // test_alloc_release_unordered_at_start_by_2();
    // test_alloc_release_unordered_at_start_by_3();
    // test_alloc_release_unordered_at_start_by_4();

    // test_alloc_release_mixed_all();
    // test_alloc_release_fragment_mem();

    // test_logger_to_html();


    return 0;
}
