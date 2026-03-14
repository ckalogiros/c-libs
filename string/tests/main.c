#include "perf_tests.h"
#include "stringc.h"
#include "slab_allocator_internals.h"
#include "string_internals.h"
#include "tests.h"

int
main(void)
{
    memman_init();
    // string_container_init();

    // test_string_create_and_extend_slabs();
    // test_string_set_and_realloc();
    test_string_append_and_realloc();

    /* performance tests */
#ifdef DEBUG_FPRINT_MCACHE
    // my_exit("DISABLE file printing for performance tests", __FILE__, __LINE__);
    // perf_test_string_set();
    // perf_test_string_append();
#endif


    memman_terminate();
    return 0;
}
