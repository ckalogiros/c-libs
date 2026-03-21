/*
 * Created by ckalo on 3/18/26.
 */

#include "mem_unit_test.h"
#include <assert.h>
#include <string.h>
#include "logger.h"
#include "mem_man_structs.h"
#include "mem_man_internals.h"


#define FOR(n) for(int32 i = 0; i < n; i++)
#define FOR_REV(n) for(int32 i = n-1; i >= 0; --i)


/***************************************************************************************************
 * HELPERS
 **************************************************************************************************/





/***************************************************************************************************
 * TESTS
 **************************************************************************************************/
/*
struct  mem_block* debug_mem_get_table                ();
struct  mem_block* mem_get_byid                       (const memid_t mem_id);
uint32  debug_mem_get_table_count               ();
void*   debug_mem_get_start_address             (const memid_t memid);
uint64  debug_mem_get_size                      (const memid_t memid);
struct  mem_chunk* debug_chunks_table_get       (const memid_t memid);
bool    debug_mem_freeing_already_freed_pages   (const struct memhnd_t hnd);
void    debug_mem_corrupted                     (const memid_t memid, const bool silent);
bool    debug_flist_check_continuity            (const memid_t memid);
void*   flist_peek_next_free_page               (memid_t mem_id);

*/

/* Basic Creation and Initial State Tests */
void test_mem_struct()
{
#define ALLOCPAGES 16
#define ALIGNMENT (PAGE_SIZE*4)
#define fl struct flist

    uint32 prev_table_count = debug_mem_get_table_count();

    memid_t memid = mem_allocate_block(ALLOCPAGES, ALIGNMENT, "");
    struct  mem_block* mem = mem_get_byid(memid);
    void* start = debug_mem_get_start_address(memid);
    assert(start == mem->start);

    uint32 table_count = debug_mem_get_table_count();
    assert(table_count == prev_table_count+1);

    fl* flist = flist_peek_next_free_page(memid);
    assert(flist == mem->flist);
    assert(debug_flist_check_continuity(memid));

    uint64 memsize = debug_mem_get_size(memid);
    assert(memsize == mem->size);

    /* allocate pages */
    struct memhnd_t handle = mem_page_request(2, memid);
    void* ptr = mem_get_addr_by_handle(handle);

    assert(ptr == mem->start);

    print_test_result("test_mem_struct", TRUE, __FILE__, __LINE__, "");

#undef fl
#undef ALIGNMENT
#undef ALLOCPAGES
}


/*Single Allocation Tests*/

/*Multiple Allocations Test*/
void
test_mem_multiple_allocations()
{
#define ALLOCPAGES 16
#define ALIGNMENT (PAGE_SIZE*4)
#define N 8
#define M 1 // number of pages per allocation request
#define fl struct flist
#define MARK (0x58)

    assert(N <= ALLOCPAGES);
    uint32 prev_table_count = debug_mem_get_table_count();

    struct mem_block* mems[N] = {NULL};
    memid_t memids[N] = {0};
    uint32 num_allocs[N] = {0};

    // create
    FOR(N) {
        num_allocs[i] = M;

        memids[i] = mem_allocate_block(ALLOCPAGES, ALIGNMENT, "test_mem_multiple_allocations()");
        mems[i] = mem_get_byid(memids[i]);

        assert(debug_flist_check_continuity(memids[i]));

        uint32 table_count = debug_mem_get_table_count();
        assert(table_count == prev_table_count+i+1);
    }
    // allocate
    FOR(N) {

        struct mem_block* mem = mems[i];

        uint64 alloced_size = num_allocs[i]*PAGE_SIZE;;
        char* ptr = mem->start;

        mem_page_request(num_allocs[i], memids[i]);

        memset(mems[i]->start, MARK, num_allocs[i]*PAGE_SIZE);
        assert(*ptr == MARK);

        char* end = ptr+mem->size;
        /* check last uint32 size is the already marked poison memory */
        assert(*(((uint32*) end)-4) == (uint32)(MEM_POISON_PATTERN&0xFFFFFFFF));
        char* alloced_end = ptr+alloced_size;
        assert(*(alloced_end-1) == MARK);
        /* end of allocated mem_block + a pointer size must equal to:'F' from flist's mark:'FREE" */
        assert(*(uint8*)((uint64*)alloced_end+1) == 'F');

        // print("mems[i]->start:%lx\n", REDUCE_ADDR(mems[i]->start, F6));
    }


    print_test_result("test_mem_multiple_allocations()", TRUE, __FILE__, __LINE__, "");

#undef MARK
#undef fl
#undef N
#undef M
#undef ALIGNMENT
#undef ALLOCPAGES
}

// TODO: implement - mem_block release, mem_block extend

/*Release Tests*/

/*Edge Case Tests*/

/*test_release_on_empty_allocator()*/

/*test_allocation_exceeds_capacity() */

/*Alignment Tests*/

/*Data Integrity Test*/

/*Multiple Instances Test*/


/*Test Runner*/
void
unit_test_runner_slab_allocator(void)
{
    print("Running Slab Allocator Tests\n");
    print("==============================\n\n");

    test_mem_struct();
    test_mem_multiple_allocations();

    print("\n[%s✓✓✓%s] All tests completed\n", GREEN, CLEAR);
}

#undef SOME_TYPE
#undef FOR(n)
#undef FOR_REV(n)
