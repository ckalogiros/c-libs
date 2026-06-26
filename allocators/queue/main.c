#include "mem_man_interface.h"
#include "tests_other.h"
#include "unit_test.h"

int
main(void)
{
    memman_init();

    // test_queue_allocator_typesize_byte();
    test_queue_allocator_typesize_arbitrary();
    // test_queue_allocator_alignment();

    // unit_test_runner_queue_llocator();

    memman_terminate();
    return 0;
}
