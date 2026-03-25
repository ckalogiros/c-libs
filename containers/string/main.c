#include <stdio.h>

#include "mem_man_interface.h"
#include "test_performance.h"
#include "tests_other.h"
#include "debug_defines.h"

int
main(void)
{
    memman_init();


    test_string_create_and_extend_slabs();
    // test_string_set_and_realloc();
    // test_string_append_and_realloc();

    /* performance tests */
    // #if DEBUG_FPRINT_MCACHE == 0
    // my_exit("DISABLE file printing for performance tests", __FILE__, __LINE__);
    // perf_test_string_set();
    // perf_test_string_append();
    // #endif

    memman_terminate();
    return 0;
}
