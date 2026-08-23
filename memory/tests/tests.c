/*
 * Created by ckalo on 2/6/26.
 */

#include "tests.h"
#include <assert.h>
#include <string.h>
#include "logger.h"
#include "logger_color.h"
#include "mem_log.h"
#include "mem_man_interface.h"
#include "file_sys_interface.h"
#include "sys_utils.h"


/***************************************************************************************************
 * Helper functions
 * 1. test if memory is released, by checking mem.flist list
 * 2. test if released memory is iserted in order in the mem.flist list
 */
void
test_mem_free_list_is_freed(const struct memhnd_t hnd, char* allocated_addr)
{
    struct flist* flist = mem_table_get_head(hnd.mem_id);
    char* alloc_addr = (char*) mem_get_addr_by_handle(hnd);
    bool are_pages_freed = FALSE;

    char* next_free_addr = NULL;

    if(alloc_addr == allocated_addr) {
        while((char*) flist <= allocated_addr) {
            assert(flist);

            if((char*) flist == allocated_addr) {
                are_pages_freed = TRUE;
                uint32 num_pages = hnd.num_pages - 1;
                while(num_pages) {
                    if((char*) flist + PAGE_SIZE != (char*) flist->next)
                        are_pages_freed = FALSE;

                    flist = flist->next;
                    num_pages--;
                }

                next_free_addr = (char*) flist;
                break;
            }

            flist = flist->next;
        }
    }

    bool failed = FALSE;
    uint32 dif = (uint32) (next_free_addr - allocated_addr);
    dif /= PAGE_SIZE;

    const char* test_name = "mem_block is released";
    const char* description = "{match}: is allocated memory released";

    if(are_pages_freed) {
        print_test_result(test_name, TRUE, __FILE__, __LINE__, description);
    } else {
        print_test_result(test_name, FALSE, __FILE__, __LINE__, description);
        failed = TRUE;
    }

    if(failed) {
        print(
            "allocated_addr: %x\n"
            "next_free_addr: %x\n"
            "ptr dif: %u\n"
            "num pages: %u\n",
            allocated_addr,
            next_free_addr,
            dif,
            hnd.num_pages
        );
    }
}

void
test_mem_free_list_check_continuity(const struct memhnd_t hnd)
{
    bool failed = FALSE;
    bool is_contiguous = debug_flist_check_continuity(hnd.mem_id);;
    const char* test_name = "mem_block release continuity";
    const char* description = "{match}: released memory pages be in continuity";

    if(is_contiguous) {
        print_test_result(test_name, TRUE, __FILE__, __LINE__, description);
    } else {
        print_test_result(test_name, FALSE, __FILE__, __LINE__, description);
        failed = TRUE;
    }

    if(failed) {
        print(
            "allocated_addr: %x\n"
            "next_free_addr: %x\n"
            "ptr dif: %u\n"
            "num pages: %u\n",
            hnd.num_pages
        );
    }
}

void
test_of_tests(const pageid_t num_pages, const bool print_list)
{
    print("%sTest:test_alloc_release-insequence-pages:%u %s\n", GREEN, num_pages, CLEAR);

    const memid_t memid = memman_allocate_block(num_pages, 1024, "test_of_tests");
    char* allocated_addr = memman_get_mem_start_by_memid(memid);
    struct memhnd_t hnd;
    hnd = mem_page_request(num_pages, memid);

    mem_page_release(hnd);

    test_mem_free_list_is_freed(hnd, allocated_addr);
    test_mem_free_list_check_continuity(hnd);
}


/***************************************************************************************************
 * simple allocations
 */

/* tests the allocation algorithm when allocation does not fit in some free pages  */
void
test_mem_alloc(void)
{
    const memid_t memid = memman_allocate_block(8, 1024, "test_mem_alloc");
    {
        print("%stest the allocation algorithm when allocation does not fit in some free pages:2 %s\n", GREEN, CLEAR);

        print("%s 1. allocate(2 pages) %s\n", YELLOW, CLEAR);
        struct memhnd_t hnd1 = mem_page_request(2, memid);
        print("%s 2. allocate(2 pages) %s\n", YELLOW, CLEAR);
        struct memhnd_t hnd2 = mem_page_request(2, memid);
        print("%s 3. allocate(2 pages) %s\n", YELLOW, CLEAR);
        struct memhnd_t hnd3 = mem_page_request(2, memid);
        print("%s 4. allocate(2 pages) %s\n", YELLOW, CLEAR);
        struct memhnd_t hnd4 = mem_page_request(2, memid);

        print("%s release(alloc 1) %s\n", YELLOW, CLEAR);
        mem_page_release(hnd1);
        mem_log_print_free_list(hnd1.mem_id);
        print("%s release(alloc 3) %s\n", YELLOW, CLEAR);
        mem_page_release(hnd3);
        mem_log_print_free_list(hnd1.mem_id);
        print("%s release(alloc 4) %s\n", YELLOW, CLEAR);
        mem_page_release(hnd4);
        mem_log_print_free_list(hnd1.mem_id);

        print("%s 5. allocate(3 pages) %s\n", YELLOW, CLEAR);
        struct memhnd_t hnd5 = mem_page_request(4, memid);
        mem_log_print_free_list(hnd1.mem_id);

        /* expected result
         * free list: 0->1->8->9->10->11
         *            4000 4400 6000 6400 6800 6c00
         * allocated: [2,3],[4,5,6,7]
         *           4800 4c00 5000 5400 5800 5c00
         */
        print("%sExpected out put %s\n"
              "free list: 4000 4400 6000 6400 6800 6c00\n"
              "allocated: 4800 4c00 5000 5400 5800 5c00\n",
              GREEN, CLEAR);

        mem_page_release(hnd2);
        mem_page_release(hnd5);
        print("%s ALL MEMORY RELEASED %s\n", YELLOW, CLEAR);
        mem_log_print_free_list(hnd1.mem_id);
    }
}


/***************************************************************************************************
 * allocate then release in sequence
 * alloc(1)-release(), alloc(2)-release, ...
 */
void
test_alloc_release_insequence(const pageid_t num_pages, const bool print_list)
{
    struct memhnd_t hnd;
    const memid_t memid = memman_allocate_block(4, 1024, "test_alloc_release_insequence");
    char* allocated_addr = NULL;

    print("%sTest:test_alloc_release-insequence-pages:%u %s\n", GREEN, num_pages, CLEAR);

    hnd = mem_page_request(num_pages, memid);

    // for tests
    allocated_addr = (char*) mem_get_addr_by_handle(hnd);

    mem_page_release(hnd);
    test_mem_free_list_is_freed(hnd, allocated_addr);
    test_mem_free_list_check_continuity(hnd);
}

void
test_alloc_first_release_after(const pageid_t num_pages, const bool print_list)
{
    const memid_t memid = memman_allocate_block(8, 1024, "test_alloc_first_release_after");
    struct memhnd_t hnd1;
    struct memhnd_t hnd2;
    char* allocated_addr1 = NULL;
    char* allocated_addr2 = NULL;

    print("%sTest:alloc first - release after. 2 allocations. pages:%u %s\n", GREEN, num_pages, CLEAR);

    hnd1 = mem_page_request(num_pages, memid);

    hnd2 = mem_page_request(num_pages, memid);

    const char* start = calc_address_from_page_id(hnd1.page_id, hnd1.mem_id);
    const char str[8] = {'H', 'E', 'L', 'L', 'O', '!', '!', '!'};
    const uint64 strint = make_uint64_from_string_max_8chars(str, 8);
    memset_64bit(start, strint, PAGE_SIZE);
    struct fs_t* fs = fs_create("memory/test_alloc_first_release_after.txt");
    fs_append_mem(fs, start, PAGE_SIZE, "occupied memory");

    // for tests
    allocated_addr1 = (char*) mem_get_addr_by_handle(hnd1);
    allocated_addr2 = (char*) mem_get_addr_by_handle(hnd2);

    mem_page_release(hnd2);

    mem_page_release(hnd1);

    fs = fs_create("memory/test_alloc_first_release_after.txt");
    fs_write_list(fs, flist_peek_next_free_page(memid), "");


    test_mem_free_list_is_freed(hnd1, allocated_addr1);
    test_mem_free_list_check_continuity(hnd1);


    test_mem_free_list_is_freed(hnd2, allocated_addr2);
    test_mem_free_list_check_continuity(hnd2);
}


/***************************************************************************************************
 * allocate many then release many, not in order
 * alloc(1), alloc(2), alloc(3), ... then release(2), release(1), release(3)
 */
void
test_alloc_first_release_after_backwards(const pageid_t num_pages, const bool print_list)
{
    const memid_t memid = memman_allocate_block(num_pages*3, 1024, "test_alloc_first_release_after_backwards");
    uint16 cnt1 = 0;
    uint16 cnt2 = 1;
    uint16 cnt3 = 2;
    struct memhnd_t hnd[3] = {0};
    char* allocated_addr[3] = {NULL};

    print("%sTest:alloc first - release after backwards.\n 3 allocations. pages:%u %s\n", GREEN, num_pages, CLEAR);

    hnd[cnt3] = mem_page_request(num_pages, memid);
    hnd[cnt2] = mem_page_request(num_pages, memid);
    hnd[cnt1] = mem_page_request(num_pages, memid);

    // for tests
    allocated_addr[cnt1] = (char*) mem_get_addr_by_handle(hnd[cnt1]);
    allocated_addr[cnt2] = (char*) mem_get_addr_by_handle(hnd[cnt2]);
    allocated_addr[cnt3] = (char*) mem_get_addr_by_handle(hnd[cnt3]);


    mem_page_release(hnd[cnt1]);
    test_mem_free_list_is_freed(hnd[cnt1], allocated_addr[cnt1]);
    test_mem_free_list_check_continuity(hnd[cnt1]);


    mem_page_release(hnd[cnt2]);
    test_mem_free_list_is_freed(hnd[cnt2], allocated_addr[cnt2]);
    test_mem_free_list_check_continuity(hnd[cnt2]);


    mem_page_release(hnd[cnt3]);
    test_mem_free_list_is_freed(hnd[cnt3], allocated_addr[cnt3]);
    test_mem_free_list_check_continuity(hnd[cnt3]);
}


/***************************************************************************************************
 * allocate many then release many, not in order
 * alloc(1), alloc(2), alloc(3), ... then release(2), release(1), release(3)
 */
void
test_alloc_release_mixed_all()
{
    print("%s -- TEST --\n"
          "  alloc:   (1),(2),(3),(4)\n"
          "  release: (1),(2),(3),(4) %s\n", GREEN, CLEAR);
    const memid_t memid = memman_allocate_block(8, 1024, "test_alloc_release_mixed_all");
    {
        struct memhnd_t hnd1 = mem_page_request(1, memid);
        struct memhnd_t hnd2 = mem_page_request(3, memid);
        struct memhnd_t hnd3 = mem_page_request(2, memid);
        struct memhnd_t hnd4 = mem_page_request(2, memid);

        mem_page_release(hnd1);
        mem_page_release(hnd2);
        mem_page_release(hnd3);
        mem_page_release(hnd4);

        char* alloc_addr1 = (char*) mem_get_addr_by_handle(hnd1);
        char* alloc_addr2 = (char*) mem_get_addr_by_handle(hnd2);
        char* alloc_addr3 = (char*) mem_get_addr_by_handle(hnd3);
        char* alloc_addr4 = (char*) mem_get_addr_by_handle(hnd4);


        test_mem_free_list_is_freed(hnd1, alloc_addr1);
        test_mem_free_list_is_freed(hnd2, alloc_addr2);
        test_mem_free_list_is_freed(hnd3, alloc_addr3);
        test_mem_free_list_is_freed(hnd4, alloc_addr4);

        test_mem_free_list_check_continuity(hnd1);
    }


    print("%s -- TEST --\n"
          "  alloc:   (1),(2),(3),(4)\n"
          "  release: (4),(1),(2),(3) %s\n", GREEN, CLEAR);
    {
        struct memhnd_t hnd1 = mem_page_request(1, memid);
        struct memhnd_t hnd2 = mem_page_request(3, memid);
        struct memhnd_t hnd3 = mem_page_request(2, memid);
        struct memhnd_t hnd4 = mem_page_request(2, memid);

        print("%s release mem_block: %u %s\n", ry, hnd4.mem_id, CLEAR);
        mem_page_release(hnd4);
        char* alloc_addr4 = (char*) mem_get_addr_by_handle(hnd4);
        test_mem_free_list_is_freed(hnd4, alloc_addr4);

        mem_page_release(hnd1);
        char* alloc_addr1 = (char*) mem_get_addr_by_handle(hnd1);
        test_mem_free_list_is_freed(hnd1, alloc_addr1);

        mem_page_release(hnd2);
        char* alloc_addr2 = (char*) mem_get_addr_by_handle(hnd2);
        test_mem_free_list_is_freed(hnd2, alloc_addr2);

        mem_page_release(hnd3);
        char* alloc_addr3 = (char*) mem_get_addr_by_handle(hnd3);
        test_mem_free_list_is_freed(hnd3, alloc_addr3);

        test_mem_free_list_check_continuity(hnd1);
    }


    print("%s -- TEST --\n"
          "  alloc:   (1),(2),(3),(4)\n"
          "  release: (3),(1),(4),(2) %s\n", GREEN, CLEAR);
    {
        struct memhnd_t hnd1 = mem_page_request(1, memid);
        struct memhnd_t hnd2 = mem_page_request(3, memid);
        struct memhnd_t hnd3 = mem_page_request(2, memid);
        struct memhnd_t hnd4 = mem_page_request(2, memid);

        print("%s release mem_block: %u %s\n", ry, hnd4.mem_id, CLEAR);
        mem_page_release(hnd3);
        char* alloc_addr3 = (char*) mem_get_addr_by_handle(hnd3);
        test_mem_free_list_is_freed(hnd3, alloc_addr3);

        mem_page_release(hnd1);
        char* alloc_addr1 = (char*) mem_get_addr_by_handle(hnd1);
        test_mem_free_list_is_freed(hnd1, alloc_addr1);

        mem_page_release(hnd4);
        char* alloc_addr4 = (char*) mem_get_addr_by_handle(hnd4);
        test_mem_free_list_is_freed(hnd4, alloc_addr4);

        mem_page_release(hnd2);
        char* alloc_addr2 = (char*) mem_get_addr_by_handle(hnd2);
        test_mem_free_list_is_freed(hnd2, alloc_addr2);


        test_mem_free_list_check_continuity(hnd1);
    }

    print("%s -- FINISH -- %s\n", YELLOW, CLEAR);
}

void
test_alloc_release_fragment_mem()
{
    const memid_t memid = memman_allocate_block(5, 1024, "test_alloc_release_fragment_mem");
    uint32 num_pages = 1;

    print("%s -- TEST --\n"
          "  TODO\n"
          "  TODO %s\n", GREEN, CLEAR);
    {
        // struct memhnd_t hnd1 = mem_page_request(num_pages, MEM_ID_ALLOCATOR_SLAB);
        // struct memhnd_t hnd2 = mem_page_request(num_pages, MEM_ID_ALLOCATOR_SLAB);
        // struct memhnd_t hnd3 = mem_page_request(num_pages, MEM_ID_ALLOCATOR_SLAB);
        // struct memhnd_t hnd4 = mem_page_request(num_pages, MEM_ID_ALLOCATOR_SLAB);
        //
        //
        // mem_page_release(hnd1);
        // // mem_page_release(hnd4);
        // print("%s#### FRAGMENTED HERE%s\n", YELLOW, CLEAR);
        //
        // // hnd1 = mem_page_request(num_pages, MEM_ID_ALLOCATOR_SLAB);
        // // char* allocated_addr1 = (char*)mem_get_addr_by_handle(hnd1);
        // // mem_page_release(hnd1);
        // // test_mem_free_list_is_freed(hnd1, allocated_addr1);
        //
        //
        // hnd1 = mem_page_request(3, MEM_ID_ALLOCATOR_SLAB);
        // // hnd3 = mem_page_request(3, MEM_ID_ALLOCATOR_SLAB);
        // // char* allocated_addr3 = (char*)mem_get_addr_by_handle(hnd3);
        // // hnd3 = mem_page_request(3, MEM_ID_ALLOCATOR_SLAB);
        //
        // print("%s#### ALLOCATED NEW MEM HERE%s\n", YELLOW, CLEAR);
        //
        // char* allocated_addr1 = (char*)mem_get_addr_by_handle(hnd1);
        // mem_page_release(hnd1);
        // test_mem_free_list_is_freed(hnd1, allocated_addr1);
        //
        // char* allocated_addr4 = (char*)mem_get_addr_by_handle(hnd4);
        // mem_page_release(hnd4);
        // test_mem_free_list_is_freed(hnd4, allocated_addr4);
        //
        // char* allocated_addr3 = (char*)mem_get_addr_by_handle(hnd3);
        // mem_page_release(hnd3);
        // test_mem_free_list_is_freed(hnd3, allocated_addr3);
        //
        // char* allocated_addr2 = (char*)mem_get_addr_by_handle(hnd2);
        // mem_page_release(hnd2);
        // test_mem_free_list_is_freed(hnd2, allocated_addr2);
        //
        // test_mem_free_list_check_continuity(hnd1);
        //
        //
        // print("%s#### RELEASE ALL HERE%s\n", YELLOW, CLEAR);
    }
    {
        struct memhnd_t hnd1 = mem_page_request(num_pages, memid);
        struct memhnd_t hnd2 = mem_page_request(num_pages, memid);
        struct memhnd_t hnd3 = mem_page_request(num_pages, memid);
        struct memhnd_t hnd4 = mem_page_request(num_pages, memid);


        print("%s#### FRAGMENTED HERE%s\n", YELLOW, CLEAR);


        print("%s#### ALLOCATED NEW MEM HERE%s\n", YELLOW, CLEAR);

        char* allocated_addr1 = (char*) mem_get_addr_by_handle(hnd1);
        memset(allocated_addr1, 0xa, hnd1.num_pages*PAGE_SIZE); mem_page_release(hnd1);
        test_mem_free_list_is_freed(hnd1, allocated_addr1);

        char* allocated_addr4 = (char*) mem_get_addr_by_handle(hnd4);
        memset(allocated_addr4, 0xb, hnd4.num_pages*PAGE_SIZE);mem_page_release(hnd4);
        test_mem_free_list_is_freed(hnd4, allocated_addr4);

        char* allocated_addr3 = (char*) mem_get_addr_by_handle(hnd3);
        memset(allocated_addr3, 0xc, hnd3.num_pages*PAGE_SIZE);mem_page_release(hnd3);
        test_mem_free_list_is_freed(hnd3, allocated_addr3);

        char* allocated_addr2 = (char*) mem_get_addr_by_handle(hnd2);
        memset(allocated_addr2, 0xd, hnd2.num_pages*PAGE_SIZE);mem_page_release(hnd2);
        test_mem_free_list_is_freed(hnd2, allocated_addr2);

        /******************************************************************************************
         * repeat
         */
        hnd3 = mem_page_request(3, memid);
        allocated_addr3 = (char*) mem_get_addr_by_handle(hnd3);
        memset(allocated_addr3, 0xc, hnd3.num_pages*PAGE_SIZE);mem_page_release(hnd3);
        test_mem_free_list_is_freed(hnd3, allocated_addr3);

        hnd3 = mem_page_request(3, memid);
        allocated_addr3 = (char*) mem_get_addr_by_handle(hnd3);
        memset(allocated_addr3, 0xc, hnd3.num_pages*PAGE_SIZE);mem_page_release(hnd3);
        test_mem_free_list_is_freed(hnd3, allocated_addr3);

        hnd3 = mem_page_request(3, memid);
        allocated_addr3 = (char*) mem_get_addr_by_handle(hnd3);
        memset(allocated_addr3, 0xc, hnd3.num_pages*PAGE_SIZE);mem_page_release(hnd3);
        test_mem_free_list_is_freed(hnd3, allocated_addr3);

        test_mem_free_list_check_continuity(hnd1);

        mem_log_print_free_list(hnd1.mem_id);

        print("%s#### RELEASE ALL HERE%s\n", YELLOW, CLEAR);
    }
    print("%s -- FINISH -- %s\n", YELLOW, CLEAR);
}
