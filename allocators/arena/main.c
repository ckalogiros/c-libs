#include <stdio.h>

#include "mem_man_interface.h"
#include "tests_other.h"
#include "unit_test.h"

void unit_test_runner_arena_allocator(void);

int
main(void)
{
    // test_arena_allocator();

    /// TODO: IMPLEMENT
    // test_arena_allocator_alignment();

    /// TODO: IMPLEMENT
    unit_test_runner_arena_allocator();

    memman_terminate();
    return 0;
}
