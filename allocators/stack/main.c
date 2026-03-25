#include <stdio.h>

#include "mem_man_interface.h"
#include "tests_other.h"
#include "unit_test.h"

int
main(void)
{
    memman_init();

    // test_stack_allocator();
    // test_stack_allocator_alignment();

    unit_test_runner_stack_allocator();

    memman_terminate();
    return 0;
}
