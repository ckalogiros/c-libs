/*
 * Created by ckalo on 3/16/26.
 */

#include "unit_test.h"
#include <assert.h>
#include <math.h>
#include <string.h>
#include "logger.h"
#include "mem_man_structs.h"
#include "my_macros.h"
#include "slab_allocator.h"
#include "slab_allocator_internals.h"
#include "mem_man_internals.h"
#include "slab_allocator_log.h"

#define TEST_NUM_PAGES 2
#define SOME_TYPE char

#define FOR(n) for(int32 i = 0; i < n; i++)
#define FOR_REV(n) for(int32 i = n-1; i >= 0; --i)


/***************************************************************************************************
 * HELPERS
 **************************************************************************************************/
void
print_buffer(void* buffer, const uint32 buffer_size)
{
    uint32 i = 0;
    char* buf = (char*) buffer;
    for(; i < buffer_size; i++) {
        print("%c", buf[i]);
    }
    print("\n");
}

void
check_buffer(void* buffer, const uint32 buffer_size, const char c, const uint32 index)
{
    bool failed = FALSE;

    print("[%s%u%s]", CYAN, index, CLEAR);

    uint32 i = 0;
    char* buf = (char*) buffer;

    for(; i < buffer_size; i++) {
        if(c == '\0') print("0");
        else print("%c", buf[i]);
        if((uint8) buf[i] != (uint8) c) {
            failed = TRUE;
        }
    }
    if(failed) print(" %sFAIL%s", RED, CLEAR);
    else print(" %sPASS%s", GREEN, CLEAR);

    print(" addr: %s%x%s\n", br2, REDUCE_ADDR(buffer, F5), CLEAR);
}


/*
 * these are a copies of the function(s) of slab_allocator, for testing purposes
 * MUST BE KEPT UPDATED TO THE ACTUAL SLAB ALLOCATOR'S FUNCTIONS
 */
struct slab_release_ret
mem_cache_free_slab_test_debug(struct slab_allocator_mcache* mcache, void* slab_address)
{
    struct slab_release_ret ret = {.freed = NULL, .state = 0x0};

    struct flist* released = (struct flist*) slab_address;
    struct flist* node = mcache->slabs_free_list;

    const char* free_buffer = (const char*)slab_address;

    memset(slab_address, 0x0, mcache->slab_size);

    if(node == NULL) {
        /* insert at head if free list is NULL */
        mcache->slabs_free_list = released;
        mcache->slabs_free_list->next = NULL;
        ret.freed = free_buffer;
        ret.state |= IS_HEAD | IS_ONLY_FREED;
    } else if(node > released) {
        /* insert at head and link to the next free */
        mcache->slabs_free_list = released;
        mcache->slabs_free_list->next = node;
        ret.freed = free_buffer;
        ret.state |= IS_HEAD | IS_LIST_NOT_EMPTY;
    } else if(node->next != NULL) {
        /* insert in between and link next and previous free */
        while(node->next != NULL && node->next < released)
            node = node->next;
        struct flist* temp = node->next;
        node->next = released;
        node->next->next = temp;
        ret.freed = free_buffer;
        if(temp == NULL) ret.state |= IS_LAST;
        else ret.state |= IS_BETWEEN;
    } else {
        /* insert at the end */
        node->next = (struct flist*) slab_address;
        node->next->next = NULL;
        ret.freed = free_buffer;
        ret.state |= IS_LAST;
    }

    /* update mcache */
    mcache->slabs_count--;

    return ret;
}

struct slab_release_ret
mem_cache_release_buffer_test_debug(const salltable_t inst_id, void* ptr, const slab_elems_t buffer_size, const uint16 type_size)
{
    const slab_size_t slab_size = buffer_size * type_size;

    struct slab_allocator_mcache* mcache = mcache_find_size(inst_id, slab_size);

    if(!mcache) // handle error: cache not found
        slab_allocator_print_error(SLAB_ALLOCATOR_MEMCACHE_NOT_FOUND);

    return mem_cache_free_slab_test_debug(mcache, ptr);
}



/***************************************************************************************************
 * TESTS
 **************************************************************************************************/
/*Basic Creation and Initial State Tests*/
void
test_slab_instance_struct()
{
    /*
     * struct slab_allocator_instance
     *      struct slab_allocator_pool* pool_table;
     *      struct slab_allocator_mcache* mcache_table;
     *      uint16 instance_id;
     *      uint16 pool_count;
     *      uint16 mcache_count;
     */
    memid_t inst_id = slab_allocator_create_new_instance(TEST_NUM_PAGES, "test_slab_instance_struct()");
    struct slab_allocator_instance* inst = slab_allocator_get_instance(inst_id);

    assert(inst->pool_table != NULL);
    assert(inst->mcache_table != NULL);
    assert(inst->pool_count == 0);
    assert(inst->mcache_count == 0);
    assert(inst->instance_id == inst_id);

    print_test_result("test_slab_instance_struct", TRUE, __FILE__, __LINE__, "");
}

void
test_slab_pool_struct()
{
    memid_t inst_id = slab_allocator_create_new_instance(TEST_NUM_PAGES, "test_slab_pool_struct()");
    struct slab_allocator_instance* inst = slab_allocator_get_instance(inst_id);
    uint32 numlems = 1024, typesize = sizeof(uint32);
    const uint32 slab_size = numlems * typesize;
    uint16 pool_id = slab_allocator_instance_create_new_pool(inst_id, TEST_NUM_PAGES, slab_size, "test_slab_pool_struct()");

    struct slab_allocator_pool pool = inst->pool_table[pool_id];


    // assert(pool.mem_id == inst_id);
    assert(pool.mcache_count == 0);
    assert(pool.max_mcache_size >= slab_size);
    assert(pool.min_mcache_size <= slab_size);

    print_test_result("test_slab_pool_struct", TRUE, __FILE__, __LINE__, "");
}

void
test_slab_mcache_struct()
{
#define ALLOCPAGES 4
    /*
     * struct slab_allocator_mcache
     *     struct flist* slabs_free_list;
     *     void* slabs_start;
     *     void* last_slab_end;
     *     slab_size_t slab_size;
     *     uint16 slabs_capacity;
     *     uint16 slabs_count;
     *     uint16 slabs_num_pages;
     *     struct memhnd_t mem_handle;
     *     uint8 idx;
     *     uint8 is_active;
     *     char_buf16_t name;
     */
    memid_t inst_id = slab_allocator_create_new_instance(ALLOCPAGES, "test_slab_mcache_struct()");
    struct slab_allocator_instance* inst = slab_allocator_get_instance(inst_id);

    uint32 numlems = 1024, typesize = sizeof(uint32);
    struct slab_allocator_mcache* mcache =
            slab_allocator_create_mcache(inst_id, numlems, typesize, "test_slab_mcache_struct()");
    const uint32 num_pages = ROUND_UP(numlems * typesize, PAGE_SIZE) / PAGE_SIZE;

    assert(inst->mcache_table != NULL);
    assert(mcache != NULL);
    assert(mcache->slabs_start != NULL);
    assert(mcache->slab_size == numlems * typesize);
    assert(mcache->slabs_count == 0);
    assert(mcache->slabs_num_pages == num_pages);
    assert(mcache->slabs_capacity == (mcache->slabs_num_pages*PAGE_SIZE)/mcache->slab_size);
    assert(mcache->slabs_free_list != NULL);
    assert(mcache->mem_handle.mem_id == inst->pool_table[0].mem_id);
    assert(mcache->mem_handle.num_pages == num_pages);
    assert(mcache->is_active == TRUE);
    assert(mcache->last_slab_end == mcache->slabs_start+mcache->slab_size*mcache->slabs_capacity);


    struct mem_block* mem = mem_get_byid(mcache->mem_handle.mem_id);
    assert(mcache->slabs_start == mem->start);
    assert(mcache->slabs_free_list == mcache->slabs_start);
    assert(mcache->mem_handle.page_id == calc_page_id_from_address(mcache->slabs_start, mcache->mem_handle.mem_id));

    print_test_result("test_slab_mcache_struct", TRUE, __FILE__, __LINE__, "");
#undef ALLOCPAGES
}

/*Single Allocation Tests*/
void
test_slab_single_allocation()
{
    memid_t inst_id = slab_allocator_create_new_instance(TEST_NUM_PAGES, "test_slab_single_allocation()");
    size_t alloc_size = 100;
    uint32 slab_size = alloc_size * sizeof(SOME_TYPE);

    void* ptr = slab_allocator_alloc(inst_id, alloc_size, sizeof(SOME_TYPE), "test_slab_single_allocation()");
    struct slab_allocator_instance* inst = slab_allocator_get_instance(inst_id);
    struct slab_allocator_mcache* mcache = mcache_find_size(inst_id, slab_size);

    assert(ptr == mcache->slabs_start);
    assert(mcache->slab_size == alloc_size);

    print_test_result("test_slab_single_allocation", TRUE, __FILE__, __LINE__, "");
}

/*Multiple Allocations Test*/
void
test_slab_multiple_allocations_same_mcache()
{
#define NUM_ALLOCATIONS 6
#define ALLOCPAGES 12

    memid_t inst_id = slab_allocator_create_new_instance(ALLOCPAGES, "test_slab_multiple_allocations_same_mcache()");

    size_t mts = 8;
    size_t typesizes[NUM_ALLOCATIONS] = {0};
    // size_t mc = 256;
    size_t mc = pow(2, NUM_ALLOCATIONS + 2);
    size_t capacities[NUM_ALLOCATIONS] = {0};
    size_t sizes[NUM_ALLOCATIONS] = {0};
    void* ptrs[NUM_ALLOCATIONS];

    for(int i = 0; i < NUM_ALLOCATIONS; i++) {
        typesizes[i] = mts;
        capacities[i] = mc;
        mts *= 2;
        mc /= 2;
        assert(mc%2 == 0);
        assert(mc < 64536);
    }


    struct slab_allocator_mcache* mcaches[NUM_ALLOCATIONS] = {NULL};

    for(int i = 0; i < NUM_ALLOCATIONS; i++) {
        ptrs[i] = slab_allocator_alloc(inst_id, capacities[i], typesizes[i], "test_slab_multiple_allocations_same_mcache()");
    }

    struct slab_allocator_instance* inst = slab_allocator_get_instance(inst_id);

    size_t total = 0;
    for(int i = 0; i < NUM_ALLOCATIONS; i++) {
        sizes[i] = capacities[i] * typesizes[i];
        mcaches[i] = mcache_find_size(inst_id, sizes[i]);
        total += sizes[i];
    }

    // Verify mcache are the same
    for(int i = 0; i < NUM_ALLOCATIONS; i++)
        for(int j = 0; j < NUM_ALLOCATIONS; j++)
            assert(mcaches[i] == mcaches[j]);


    // verify distance between slabs is their slab_size
    for(int i = 0; i < NUM_ALLOCATIONS; i++) {
        if(i + 1 < NUM_ALLOCATIONS)
            assert((char*)ptrs[i+1] == (char*)ptrs[i] + sizes[i]);
    }

    // Verify total usage
#define ANY_MCACHE 0 // or 1-2-3...
    assert(mcaches[ANY_MCACHE]->last_slab_end == ptrs[0] + total);
    assert(mcaches[ANY_MCACHE]->last_slab_end == ptrs[NUM_ALLOCATIONS-1] + sizes[NUM_ALLOCATIONS-1]);
#undef ANY_MCACHE // or 1-2-3...

    print_test_result("test_slab_multiple_allocations_same_mcache", TRUE, __FILE__, __LINE__, "");

#undef NUM_ALLOCATIONS
#undef ALLOCPAGES
}

/*Release Tests*/
void // ISSUES: impl {same/dif slabsize}, {arbitrary num allocations},
test_slab_release()
{
    /***********************************************************************************************
     * assign mixed values to capacity, type size and number of allocations for testing edge cases
     **********************************************************************************************/
#define ALLOCPAGES 32 // num allocations
#define N 128 // num allocations
#define C 46 // capacity
#define T sizeof(uint32) // type size
#define mc struct slab_allocator_mcache
#define fl struct flist
#define rt struct slab_release_ret

    salltable_t inst_id = slab_allocator_create_new_instance(ALLOCPAGES, "test_slab_release()");

    mc* mcache_store_for_debug = NULL;

    {
        mc* ptrs[N] = {NULL};
        mc* freed[N] = {NULL};
        uint32 capac[N] = {0};
        uint32 types[N] = {0};

        // init and allocate
        FOR(N) {
            ptrs[i] = NULL;
            freed[i] = NULL;
            capac[i] = C;
            types[i] = T;

            ptrs[i] = (mc*) slab_allocator_alloc(inst_id, capac[i], types[i], "test_slab_release(): #1");
            memset(ptrs[i], 0x46, capac[i] * types[i]);
        }

        // Release in allocation order
        FOR(N) {
            const slab_size_t slab_size = capac[i] * types[i];
            mc* mcache = mcache_find_size(inst_id, slab_size);
            assert(mcache != NULL);
            mcache_store_for_debug = mcache;

            mc* p = ptrs[i];
            rt ret = mem_cache_release_buffer_test_debug(inst_id, (void*) p, capac[i], types[i]);
            freed[i] = (mc*) (ret.freed);
            assert(freed[i] == ptrs[i]);
            assert(mcache->slabs_free_list == (fl*)ptrs[0]);
            if(mcache->slabs_free_list->next == NULL) // case freed is the only free in empty list
                assert(freed[i]->slabs_free_list == NULL);
            else if(mcache->slabs_free_list->next->next == NULL) // case freed is the last elem in the list
                assert(freed[i]->slabs_free_list == NULL); // the content of freed address is memset to 0x0 in the release() func
            else if(ret.state & (IS_LAST)) // case the freed is the last in the free list
                assert(freed[i]->slabs_free_list == NULL); // the content of freed address is memset to 0x0 in the release() func
            else // the content of freed address must equal the content of the pointer that was freed
                assert(freed[i]->slabs_free_list == (fl*)ptrs[i]->slabs_free_list);
        }

        print_test_result("test_slab_release - in order", TRUE, __FILE__, __LINE__, "");
    }


    /***********************************************************************************************
     * repeat the proccess, allocating at the same memory and releasing in reverse order
     **********************************************************************************************/

    mc* freed[N] = {NULL}; // store freed ouside, for testing later
    fl* flist = mcache_store_for_debug->slabs_free_list;
    {
        mc* ptrs[N] = {NULL};
        uint32 capac[N] = {0};
        uint32 types[N] = {0};

        // init and allocate all
        FOR(N) {
            ptrs[i] = NULL;
            freed[i] = NULL;
            capac[i] = C;
            types[i] = T;
            ptrs[i] = (mc*) slab_allocator_alloc(inst_id, capac[i], types[i], "test_slab_release(): #2");
            memset(ptrs[i], 0x47, capac[i] * types[i]);
        }


        // Release in order
        FOR_REV(N) {
            const slab_size_t slab_size = capac[i] * types[i];
            mc* mcache = mcache_find_size(inst_id, slab_size);
            assert(mcache != NULL);

            mc* p = ptrs[i];
            rt ret = mem_cache_release_buffer_test_debug(inst_id, (void*) p, capac[i], types[i]);
            freed[i] = (mc*) (ret.freed);
            assert(freed[i] == ptrs[i]);
            assert(mcache->slabs_free_list == (fl*)ptrs[i]);
            if(mcache->slabs_free_list->next == NULL) // case freed is the only free in empty list
                assert(freed[i]->slabs_free_list == NULL);
            else if(mcache->slabs_free_list->next->next == NULL) // case freed is the last elem in the list
                assert(freed[i]->slabs_free_list->next == NULL); // the content of freed address is memset to 0x0 in the release() func
            else if(ret.state & (IS_LAST)) // case the freed is the last in the free list
                assert(freed[i]->slabs_free_list == NULL); // the content of freed address is memset to 0x0 in the release() func
            else // the content of freed address must equal the content of the pointer that was freed
                assert(freed[i]->slabs_free_list == (fl*)ptrs[i]->slabs_free_list);
        }

        print_test_result("test_slab_release - reverse order", TRUE, __FILE__, __LINE__, "");
    }

    // 3. check if freed pointers match the free list
    uint32 k = 0;
    while(flist) {
        assert(flist == (fl*)freed[k]);
        k++;
        flist = flist->next;
    }
    print_test_result("test_slab_release - freed pointers match free list", TRUE, __FILE__, __LINE__, "");


#undef ALLOCPAGES
#undef N
#undef C
#undef T
#undef mc
#undef fl
#undef rt
}

void
test_slab_release_freelist()
{
#define N 10 // num allocations
#define C 66 // capacity
#define T sizeof(uint32) // type size
#define mc struct slab_allocator_mcache
#define fl struct flist

    salltable_t inst_id = slab_allocator_create_new_instance(TEST_NUM_PAGES, "test_slab_release_freelist()");

    // alloc and release in sequence. confirm that the same slab is allocated and released
    fl* first_released = NULL;
    FOR(N) {
        mc* ptr = (mc*) slab_allocator_alloc(inst_id, C, T, "test_slab_release_freelist()");
        mc* mcache = mcache_find_size(inst_id, C * T);
        assert(mcache != NULL);
        mem_cache_release_buffer_test_debug(inst_id, (void*) ptr, C, T);
        if(!first_released) first_released = mcache->slabs_free_list;
        // free list must always be the same
        assert(mcache->slabs_free_list == first_released);
    }

    print_test_result("test_slab_release_freelist()", TRUE, __FILE__, __LINE__, "");

#undef N
#undef C
#undef T
#undef mc
#undef fl
}

//
// /*Edge Case Tests*/
// void
// test_zero_size_allocation()
// {
//     memid_t inst_id = slab_allocator_create_new_instance(TEST_NUM_PAGES, "test_zero_size_allocation()");
//     struct slab_allocator_instance* table = slab_allocator_get_instance(inst_id);
//     uint32 before_used = table[inst_id].data.used_size;
//
//     void* ptr = slab_allocator_alloc(inst_id, 0, sizeof(SOME_TYPE));
//     struct slab_allocator_instance* inst = slab_allocator_get_instance(inst_id);
//
//     assert(ptr == inst->data.start); // Should still return a valid pointer?
//     assert(inst->data.used_size == before_used); // Should not change?
//     assert(inst->offsets_table_count == 1); // Should it track zero-size allocations?
//
//     print_test_result("test_zero_size_allocation", TRUE, __FILE__, __LINE__, "");
// }
//
// void
// test_release_on_empty_allocator()
// {
//     memid_t inst_id = slab_allocator_create_new_instance(TEST_NUM_PAGES, "test_release_on_empty_allocator()");
//
//     // should return NULL
//     void* ptr = slab_allocator_release(inst_id);
//     assert(ptr == NULL); // Or some error indicator
//
//     print_test_result("test_release_on_empty_allocator", TRUE, __FILE__, __LINE__, "");
// }
//
// void
// test_allocation_exceeds_capacity()
// {
//     // memid_t inst_id = slab_allocator_create_new_instance(1); // Only 1 page
//     // size_t too_big = PAGE_SIZE + 1; //// ISSUES: increment the size
//     //
//     // // This should fail - maybe return NULL or assert
//     // void* ptr = slab_allocator_alloc(inst_id, too_big, sizeof(SOME_TYPE));
//     // assert(ptr == NULL); // If you implement error handling
//
//     print_test_result("ISSUES: IMPLEMENT", TRUE, __FILE__, __LINE__, "");
// }
//
// /*Alignment Tests*/
// void
// test_allocation_alignment()
// {
//     memid_t inst_id = slab_allocator_create_new_instance(TEST_NUM_PAGES, "test_allocation_alignment()");
//
//     {
//         // void* ptr1 = slab_allocator_alloc(inst_id, 1, sizeof(char));
//         // *(char*) ptr1 = 'A';
//         // assert(((uintptr_t)ptr1 & (sizeof(void*) - 1)) == 0); // Should be pointer-aligned
//         //
//         // void* ptr2 = slab_allocator_alloc(inst_id, 3, sizeof(char));
//         // *((char*) ptr2 + 0) = 'A';
//         // *((char*) ptr2 + 1) = 'B';
//         // *((char*) ptr2 + 2) = 'C';
//         // assert((uintptr_t)ptr2 == (uintptr_t)ptr1 + 1);
//     }
//
//     {
//         // void* ptr1 = slab_allocator_alloc(inst_id, sizeof(uint32), sizeof(uint32));
//         // *(uint32*) ptr1 = HACK_LIT_STR_TO_NUM("AAA.", uint32);
//         // assert(((uintptr_t)ptr1 & (sizeof(void*) - 1)) == 0); // Should be pointer-aligned
//         //
//         // void* ptr2 = slab_allocator_alloc(inst_id, sizeof(uint32), sizeof(uint32));
//         // *((uint32*) ptr2 + 0) = HACK_LIT_STR_TO_NUM("BBB.", uint32);
//         // // assert((uintptr_t)ptr2 == (uintptr_t)ptr1 + 1);
//         // assert(((uintptr_t)ptr2 & (sizeof(uint32) - 1)) == 0);
//     }
//     void* ptr1 = slab_allocator_alloc(inst_id, sizeof(char), sizeof(char));
//     *(uint32*) ptr1 = HACK_LIT_STR_TO_NUM("A", uint32);
//     assert(((uintptr_t)ptr1 & (sizeof(void*) - 1)) == 0); // Should be pointer-aligned
//
//     void* ptr2 = slab_allocator_alloc(inst_id, sizeof(uint32), sizeof(uint32));
//     *((uint32*) ptr2 + 0) = HACK_LIT_STR_TO_NUM("BBB.", uint32);
//     // assert((uintptr_t)ptr2 == (uintptr_t)ptr1 + 1);
//     assert(((uintptr_t)ptr2 & (sizeof(uint32) - 1)) == 0);
//
//     print_test_result("test_allocation_alignment", TRUE, __FILE__, __LINE__, "");
// }
//
// /*Data Integrity Test*/
// void
// test_data_persistence()
// {
//     memid_t inst_id = slab_allocator_create_new_instance(TEST_NUM_PAGES, "test_data_persistence()");
//
//     // Write pattern to allocations
//     char* ptr1 = (char*) slab_allocator_alloc(inst_id, 100, sizeof(char));
//     char* ptr2 = (char*) slab_allocator_alloc(inst_id, 200, sizeof(char));
//
//     for(int i = 0; i < 100; i++) ptr1[i] = 'A' + (i % 26);
//     for(int i = 0; i < 200; i++) ptr2[i] = '0' + (i % 10);
//
//     // Verify patterns persist
//     for(int i = 0; i < 100; i++)
//         assert(ptr1[i] == 'A' + (i % 26));
//     for(int i = 0; i < 200; i++)
//         assert(ptr2[i] == '0' + (i % 10));
//
//     print_test_result("test_data_persistence", TRUE, __FILE__, __LINE__, "");
// }
//
// /*Multiple Instances Test*/
// void
// test_multiple_instances()
// {
//     memid_t inst1 = slab_allocator_create_new_instance(2);
//     memid_t inst2 = slab_allocator_create_new_instance(TEST_NUM_PAGES, "test_multiple_instances()");
//
//     void* ptr1 = slab_allocator_alloc(inst1, 100, sizeof(SOME_TYPE));
//     void* ptr2 = slab_allocator_alloc(inst2, 200, sizeof(SOME_TYPE));
//
//     struct slab_allocator_instance* table = slab_allocator_get_instance(inst1);
//
//     // Verify instances are independent
//     assert(table[inst1].data.used_size == 100);
//     assert(table[inst2].data.used_size == 200);
//
//     // Release from inst2 shouldn't affect inst1
//     slab_allocator_release(inst2);
//     assert(table[inst1].data.used_size == 100);
//
//     print_test_result("test_multiple_instances", TRUE, __FILE__, __LINE__, "");
// }
//
/*Test Runner*/
void
unit_test_runner_slab_allocator(void)
{
    print("Running Stack Allocator Tests\n");
    print("==============================\n\n");

    test_slab_instance_struct();
    test_slab_pool_struct();
    test_slab_mcache_struct();
    test_slab_single_allocation();
    // test_slab_multiple_allocations_same_mcache();
    test_slab_release();
    test_slab_release_freelist();

    print("\n[%s✓✓✓%s] All tests completed\n", GREEN, CLEAR);
}

#undef SOME_TYPE
#undef FOR
#undef FOR_REV
