#include "mem_man_interface.h"
#include "tests_other.h"
#include "unit_test.h"

int
main(void)
{
    // test_queue_allocator();

    // test_queue_allocator_typesize_arbitrary();

    test_queue_allocator_alignment();

    // unit_test_runner_queue_allocator();

    memman_terminate();
    return 0;
}
