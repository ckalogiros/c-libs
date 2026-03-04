/*
 * Created by ckalo on 1/8/26.
 */


#include "buffer.h"
#include "file_sys_interface.h"
#include "logger.h"
#include "mem_log.h"
#include "mem_man_interface.h"
#include "precision_timer.h"
#include "perf_test_buffer.h"
#include "test_buffer.h"

#define GET_UINT8_FROM_STR(str)  ((uint8)  * ((uint8*) str))
#define GET_UINT16_FROM_STR(str) ((uint16) * ((uint16*)str))
#define GET_UINT32_FROM_STR(str) ((uint32) * ((uint32*)str))
#define GET_UINT64_FROM_STR(str) ((uint64) * ((uint64*)str))


/* Transform a unsigned integer value back
* to a literal string (e.x "Sample!"), */
#define GET_STR_FROM_UINT(uintVal)  ((char)&uintVal)


int
main(void)
{
    // uint8  s8  = 11;
    // uint16 s16 = 22;
    // uint32 s32 = 33;
    // uint64 s64 = 44;

    // char* s1 = "11";
    // char* s2 = "22";
    // char* s3 = "33";
    // char* s4 = "44";
    //
    // print("%u\n", GET_UINT8_FROM_STR(&s1[0]));
    // print("%u\n", GET_UINT16_FROM_STR(s1));
    // print("%u\n", GET_UINT32_FROM_STR(s1));
    // print("%u\n", GET_UINT64_FROM_STR(s1));
    //
    // print("\n");
    //
    // // char* r1 = GET_STR_FROM_UINT(s8);
    //
    // print("%u\n", GET_STR_FROM_UINT(s1));
    // print("%u\n", GET_STR_FROM_UINT(s16));
    // print("%u\n", GET_STR_FROM_UINT(s32));
    // print("%u\n", GET_STR_FROM_UINT(s64));


    {
        // test_create_buffer(16);
        // test_destroy_buffer();
        // test_buffer_realloc(); // todo: impl test
        // test_buffer_fill_remove_insert(); // todo: impl test
        // test_buffer_different_capacity_buffers(); // todo: impl test
        // test_buffer_remove_elements(256);  // todo: impl test
        // test_buffer_insert_elements(256); // todo: impl test
        // test_generic_type_buffer(); // todo: impl test
        // test_buffer_free_elem();
        // test_buffer_with_children();
        // test_buffer_find_tail(); // todo: impl test and function
    }

    {
        // test_buffer_with_children();
        perf_test_buffer_add_remove_with_children();
        // perf_test_buffer_add_with_children_mixed_capacities();
        // perf_test_buffer_add_remove_with_children_mixed_capacities();
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
