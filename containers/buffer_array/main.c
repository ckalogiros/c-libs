/*
* Created by ckalo on 1/8/26.
 */


#include "sparse_arr.h"
#include "file_sys_debug.h"
#include "file_sys_interface.h"
#include "logger.h"
#include "mem_log.h"
#include "mem_man_interface.h"
#include "precision_timer.h"
#include "test_performance.h"
#include "tests_other.h"


#include <sanitizer/asan_interface.h>



int
main(void)
{
    /* enable santizers to the stderr out */
    __sanitizer_set_report_path("stderr");

    memman_init();
    {
        test_create_buffer(256);
        // test_sparse_arr_fill_remove_insert();        // todo: impl test
        // test_sparse_arr_different_capacity_buffers(); // todo: impl test
        // test_sparse_arr_remove_elements(256);        // todo: impl test
        // test_sparse_arr_insert_elements(256);        // todo: impl test
        // test_sparse_arr_realloc();                   // todo: impl test
        // test_destroy_buffer();
        // test_generic_type_buffer();              // todo: impl test
        // test_sparse_arr_find_tail();                 // todo: impl test and function
    }

    {
        // perf_test_sparse_arr_mixed_capacities_add();
        // perf_test_sparse_arr_mixed_capacities_add_remove();

        // perf_test_sparse_arr_with_children_add_remove();
        // perf_test_sparse_arr_mixed_capacities_with_children_add();
        // perf_test_sparse_arr_mixed_capacities_with_children_add_remove();

        // perf_test_sparse_arr_with_children_in_dif_slabs();
    }

    {
        // perf_test_sparse_arr_remove_insert();
        // perf_test_insert_abstruct_call();
        // perf_test_sparse_arr_create_generic_vs_uint32();
        // perf_test_sparse_arr_find_tail();
    }

    { /* test debugging buffer values to a file */
        // file_sys_test();
        // file_sys_test2();
        // test_sparse_arr_fprint_values();
    }


    memman_terminate();
    return 0;
}