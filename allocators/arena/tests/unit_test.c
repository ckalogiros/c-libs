/*
 * Created by ckalo on 3/15/26.
 */

#include "unit_test.h"
#include <assert.h>
#include "logger.h"
#include "mem_man_interface.h"
#include "mem_man_structs.h"
#include "my_macros.h"
#include "arena_allocator.h"

#define TEST_NUM_PAGES 1
#define SOME_TYPE char

/*Basic Creation and Initial State Tests*/
void
test_create_instance_basic()
{
    memid_t inst_id = arena_allocator_create_new_instance(TEST_NUM_PAGES);
    struct arena_allocator_instance* inst = debug_get_arena_allocator_instance(inst_id);

    assert(inst->data.start != NULL);
    assert(inst->data.size == TEST_NUM_PAGES * PAGE_SIZE);
    assert(inst->data.used_size == 0);

    print_test_result("test_create_instance_basic", TRUE, __FILE__, __LINE__, "");
}

/*Single Allocation Tests*/
void
test_single_allocation()
{
    memid_t inst_id = arena_allocator_create_new_instance(TEST_NUM_PAGES);
    size_t alloc_size = 100;

    void* ptr = arena_allocator_alloc(inst_id, alloc_size, sizeof(SOME_TYPE));
    struct arena_allocator_instance* inst = debug_get_arena_allocator_instance(inst_id);

    assert(ptr == inst->data.start); // First allocation should be at start
    assert(inst->data.used_size == alloc_size);

    print_test_result("test_single_allocation", TRUE, __FILE__, __LINE__, "");
}

/*Multiple Allocations Test*/
void
test_multiple_allocations()
{
    memid_t inst_id = arena_allocator_create_new_instance(TEST_NUM_PAGES);
    size_t sizes[] = {100, 200, 150, 50};
    void* ptrs[4];

    for(int i = 0; i < 4; i++) {
        ptrs[i] = arena_allocator_alloc(inst_id, sizes[i], sizeof(SOME_TYPE));
    }

    struct arena_allocator_instance* inst = debug_get_arena_allocator_instance(inst_id);

    // Verify pointers are contiguous
    assert((char*)ptrs[1] == (char*)ptrs[0] + sizes[0]);
    assert((char*)ptrs[2] == (char*)ptrs[1] + sizes[1]);
    assert((char*)ptrs[3] == (char*)ptrs[2] + sizes[2]);

    // Verify total usage
    size_t total = sizes[0] + sizes[1] + sizes[2] + sizes[3];
    assert(inst->data.used_size == total);

    print_test_result("test_multiple_allocations", TRUE, __FILE__, __LINE__, "");
}

/*Release (LIFO) Tests*/
void
test_release_lifo_order()
{
    memid_t inst_id = arena_allocator_create_new_instance(TEST_NUM_PAGES);

    void* ptr1 = arena_allocator_alloc(inst_id, 100, sizeof(SOME_TYPE));
    void* ptr2 = arena_allocator_alloc(inst_id, 200, sizeof(SOME_TYPE));
    void* ptr3 = arena_allocator_alloc(inst_id, 150, sizeof(SOME_TYPE));

    struct arena_allocator_instance* inst = debug_get_arena_allocator_instance(inst_id);
    uint32 before_used = inst->data.used_size;

    // Release in reverse order (LIFO)
    void* released3 = arena_allocator_release(inst_id);
    assert(released3 == ptr3);
    assert(inst->data.used_size == before_used - 150);
    // assert(inst->offsets_table_count == 2);

    void* released2 = arena_allocator_release(inst_id);
    assert(released2 == ptr2);
    assert(inst->data.used_size == before_used - 350);
    // assert(inst->offsets_table_count == 1);

    void* released1 = arena_allocator_release(inst_id);
    assert(released1 == ptr1);
    assert(inst->data.used_size == 0);

    print_test_result("test_release_lifo_order", TRUE, __FILE__, __LINE__, "");
}

/*Edge Case Tests*/
void
test_zero_size_allocation()
{
    memid_t inst_id = arena_allocator_create_new_instance(TEST_NUM_PAGES);
    struct arena_allocator_instance* table = debug_get_arena_allocator_table();
    uint32 before_used = table[inst_id].data.used_size;

    void* ptr = arena_allocator_alloc(inst_id, 0, sizeof(SOME_TYPE));
    struct arena_allocator_instance* inst = debug_get_arena_allocator_instance(inst_id);

    assert(ptr == inst->data.start); // Should still return a valid pointer?
    assert(inst->data.used_size == before_used); // Should not change?

    print_test_result("test_zero_size_allocation", TRUE, __FILE__, __LINE__, "");
}

void
test_release_on_empty_allocator()
{
    memid_t inst_id = arena_allocator_create_new_instance(TEST_NUM_PAGES);

    // should return NULL
    void* ptr = arena_allocator_release(inst_id);
    assert(ptr == NULL); // Or some error indicator

    print_test_result("test_release_on_empty_allocator", TRUE, __FILE__, __LINE__, "");
}

void
test_allocation_exceeds_capacity()
{
    // memid_t inst_id = arena_allocator_create_new_instance(1); // Only 1 page
    // size_t too_big = PAGE_SIZE + 1; //// TODO: increment the size
    //
    // // This should fail - maybe return NULL or assert
    // void* ptr = arena_allocator_alloc(inst_id, too_big, sizeof(SOME_TYPE));
    // assert(ptr == NULL); // If you implement error handling

    print_test_result("TODO: IMPLEMENT", TRUE, __FILE__, __LINE__, "");
}

/*Alignment Tests*/
void
test_allocation_alignment()
{
    memid_t inst_id = arena_allocator_create_new_instance(TEST_NUM_PAGES);

    void* ptr1 = arena_allocator_alloc(inst_id, sizeof(uint32)*2, sizeof(char));
    *(char*) ptr1 = HACK_LITERAL_STR_TO_NUM("A", char);
    assert(((uintptr_t)ptr1 & (sizeof(void*) - 1)) == 0); // Should be pointer-aligned

    void* ptr2 = arena_allocator_alloc(inst_id, sizeof(uint32), sizeof(uint32));
    *((uint32*) ptr2 + 0) = HACK_LITERAL_STR_TO_NUM("BBB.", uint32);
    assert(((uintptr_t)ptr2 & (sizeof(uint32) - 1)) == 0);

    print_test_result("test_allocation_alignment", TRUE, __FILE__, __LINE__, "");
}

/*Data Integrity Test*/
void
test_data_persistence()
{
    memid_t inst_id = arena_allocator_create_new_instance(TEST_NUM_PAGES);

    // Write pattern to allocations
    char* ptr1 = (char*) arena_allocator_alloc(inst_id, 100, sizeof(char));
    char* ptr2 = (char*) arena_allocator_alloc(inst_id, 200, sizeof(char));

    for(int i = 0; i < 100; i++) ptr1[i] = 'A' + (i % 26);
    for(int i = 0; i < 200; i++) ptr2[i] = '0' + (i % 10);

    // Verify patterns persist
    for(int i = 0; i < 100; i++)
        assert(ptr1[i] == 'A' + (i % 26));
    for(int i = 0; i < 200; i++)
        assert(ptr2[i] == '0' + (i % 10));

    print_test_result("test_data_persistence", TRUE, __FILE__, __LINE__, "");
}

/*Multiple Instances Test*/
void
test_multiple_instances()
{
    memid_t inst1 = arena_allocator_create_new_instance(2);
    memid_t inst2 = arena_allocator_create_new_instance(TEST_NUM_PAGES);

    void* ptr1 = arena_allocator_alloc(inst1, 100, sizeof(SOME_TYPE));
    void* ptr2 = arena_allocator_alloc(inst2, 200, sizeof(SOME_TYPE));

    struct arena_allocator_instance* table = debug_get_arena_allocator_table();

    // Verify instances are independent
    assert(table[inst1].data.used_size == 100);
    assert(table[inst2].data.used_size == 200);

    // Release from inst2 shouldn't affect inst1
    arena_allocator_release(inst2);
    assert(table[inst1].data.used_size == 100);

    print_test_result("test_multiple_instances", TRUE, __FILE__, __LINE__, "");
}

/*Test Runner*/
void
unit_test_runner_arena_allocator(void)
{
    print("Running Stack Allocator Tests\n");
    print("==============================\n\n");

    test_create_instance_basic();
    test_single_allocation();
    test_multiple_allocations();
    test_release_lifo_order();
    test_zero_size_allocation();
    test_release_on_empty_allocator();
    test_allocation_exceeds_capacity(); // TODO: implement realloc()
    test_allocation_alignment();
    test_data_persistence();
    test_multiple_instances();

    print("\n✓ All tests completed\n");

}

#undef SOME_TYPE
