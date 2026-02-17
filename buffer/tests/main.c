/*
 * Created by ckalo on 1/8/26.
 */


#include "buffer.h"
#include "logger.h"
#include "mem_log.h"
#include "mem_man_interface.h"
#include "precision_timer.h"
#include "perf_test_buffer.h"
#include "test_buffer.h"


int
main(void)
{
    {
        // test_create_buffer(16);

        // test_buffer_fill_remove_insert();
        test_buffer_different_capacity_buffers();

        // test_buffer_remove_elements(256);
        // test_buffer_insert_elements(256);
        //
        // test_buffer_realloc();

        // test_destroy_buffer();

        // test_generic_type_buffer();

        // test_buffer_find_tail();
    }

    {

        // test_buffer_with_children();
        // perf_test_buffer_with_children();
        // perf_test_buffer_with_children_in_dif_slabs();
    }

    {
        // perf_test_buffer_remove_insert();
        // perf_test_insert_abstruct_call();
        // perf_test_buffer_create_generic_vs_uint32();
        // perf_test_buffer_find_tail();
    }

    return 0;
}
