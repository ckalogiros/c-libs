/*
 * Created by ckalo on 1/8/26.
 */


#include "buffer.h"
#include "logger.h"
#include "mem_log.h"
#include "mem_man.h"
#include "precision_timer.h"
#include "perf_test_buffer.h"
#include "test_buffer.h"



int main(void)
{
    {
        // test_create_buffer(16);

        // test_buffer_fill_remove_insert();
        // test_buffer_different_capacity_buffers();

        // test_buffer_remove_elements(256);
        // test_buffer_insert_elements(256);

        // test_buffer_realloc();

        // test_destroy_buffer();

        // test_generic_type_buffer();
        test_vec_type_buffer();
    }

    {
        // perf_test_buffer_remove_insert();
        // perf_test_insert_abstruct_call();
    }

    return 0;
}
