/*
 * Created by ckalo on 2/6/26.
 */

/*
 * example of a testing unit
 *
// Example: Testing the free list functions from your previous question

// Mock implementation (replace with your actual code)
typedef struct FreePage {
    void* address;
    struct FreePage* next;
} FreePage;

FreePage* free_list_head = NULL;

void* flist_peek_next_free_page() {
    return free_list_head ? free_list_head->address : NULL;
}

void* flist_get_next_free_page() {
    if (!free_list_head) return NULL;
    void* result = free_list_head->address;
    // In real implementation, update free_list_head to next
    return result;
}

// Test utilities
#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            TestResult res = {test_name, false, message, __FILE__, __LINE__}; \
            print_test_result_advanced(res); \
            return false; \
        } \
    } while(0)

#define TEST(name) \
    bool test_##name(const char* test_name)

#define RUN_TEST(test_func) \
    do { \
        total_tests++; \
        if (test_func(#test_func)) { \
            passed_tests++; \
            print_test_result(#test_func, true, __FILE__, __LINE__, ""); \
        } \
    } while(0)

// Individual test functions
TEST(flist_peek_returns_null_when_empty) {
    // Setup
    free_list_head = NULL;

    // Execute & Assert
    void* result = flist_peek_next_free_page();
    ASSERT(result == NULL, "Should return NULL for empty list");
    return true;
}

TEST(flist_peek_does_not_modify_list) {
    // Setup
    FreePage page1 = {(void*)0x1000, NULL};
    free_list_head = &page1;

    // Execute
    void* result1 = flist_peek_next_free_page();
    void* result2 = flist_peek_next_free_page();

    // Assert
    ASSERT(result1 == (void*)0x1000, "First peek should return correct address");
    ASSERT(result2 == (void*)0x1000, "Second peek should return same address");
    ASSERT(free_list_head == &page1, "List should not be modified by peek");
    return true;
}

TEST(flist_get_consumes_page) {
    // Setup
    FreePage page2 = {(void*)0x2000, NULL};
    FreePage page1 = {(void*)0x1000, &page2};
    free_list_head = &page1;

    // Execute
    void* result = flist_get_next_free_page();

    // Assert
    ASSERT(result == (void*)0x1000, "Should return first page address");
    // In real implementation, check that free_list_head now points to page2
    return true;
}

TEST(flist_peek_vs_get_difference) {
    // Setup
    FreePage page = {(void*)0x3000, NULL};
    free_list_head = &page;

    // Execute
    void* peek_result = flist_peek_next_free_page();
    void* get_result = flist_get_next_free_page();
    void* second_peek = flist_peek_next_free_page();  // Should be NULL now

    // Assert
    ASSERT(peek_result == (void*)0x3000, "Peek should return address");
    ASSERT(get_result == (void*)0x3000, "Get should return same address");
    // Note: This assertion depends on your actual implementation
    // ASSERT(second_peek == NULL, "List should be empty after get");
    return true;
}

// Main test runner
int main() {
    int total_tests = 0;
    int passed_tests = 0;

    printf("%sRunning Free List Tests...%s\n\n", COLOR_YELLOW, COLOR_RESET);

    // Run all tests
    RUN_TEST(flist_peek_returns_null_when_empty);
    RUN_TEST(flist_peek_does_not_modify_list);
    RUN_TEST(flist_get_consumes_page);
    RUN_TEST(flist_peek_vs_get_difference);

    // Print summary
    print_test_summary(passed_tests, total_tests);

    return (passed_tests == total_tests) ? 0 : 1;
}
 */

#include "tests.h"

#include "buffer_internals.h"
#include "logger.h"
#include "logger_color.h"
#include "mem_log.h"
#include "mem_man_interface.h"
#include "precision_timer.h"
#include "precision_timer_logger.h"


// todo:compare malloc with custom
void
name_me_compare_perf_of_malloc_and_mem_manager() {}

void
perf_mem_big_chunk_allocation()
{
    memman_init();
    buffer_init_table();
    timer_init_table();

    struct timer* t = timer_create_perf_timer();

    perf_timer_start(t);

    // memman_init(512);

    perf_timer_stop(t);
    perf_timer_print(timer_print_config((timer_print_params){
        t, 1, 0, PERF_T_ALL
    }));
    perf_timer_reset(t);

    memman_terminate();
}


/***************************************************************************************************
 * allocate then release in sequence
 * alloc(1)-release(), alloc(2)-release, ...
 */
void
test_alloc_release_insequence(const uint32 num_pages, const bool print_list)
{
    struct memhnd_t hnd;
    char* allocated_addr = NULL;
    char* next_free_addr = NULL;
    char* released_addr = NULL;


    bool fails = FALSE;

    print("%sTest:test_alloc_release-insequence-pages:%u %s\n", GREEN, num_pages, CLEAR);

    // mem_log_print_free_list_of_chunk(0);

    hnd = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
    if(print_list) {
        print("%s mem alloc %s\n", ry, CLEAR);
        mem_log_print_free_list_of_chunk(0);
    }

    // for test
    allocated_addr = (char*)mem_get_addr_by_handle(hnd);
    next_free_addr = (char*)flist_peek_next_free_page(hnd.chunk_id);

    mem_release(hnd);
    if(print_list) {
        print("%s mem release %s\n", ry, CLEAR);
        mem_log_print_free_list_of_chunk(0);
    }

    // for test
    released_addr = (char*)flist_peek_next_free_page(hnd.chunk_id);

    // test 1: allocated memory and released memory match
    if(allocated_addr == released_addr) {
        print_test_result("mem alloc-release", TRUE, __FILE__, __LINE__, "match allocated-released memory");
    } else {
        print_test_result("mem alloc-release", FALSE, __FILE__, __LINE__, "match allocated-released memory");
        fails = TRUE;
    }

    // test 2: match: next free list be at an offset of allocated pages
    uint32 dif = (uint32 )(next_free_addr - allocated_addr);
    dif /= PAGE_SIZE;
    if(dif == hnd.num_pages) {
        print_test_result("mem alloc-release", TRUE, __FILE__, __LINE__, "{match}: next free list be at an offset of allocated pages");
    } else {
        print_test_result("mem alloc-release", FALSE, __FILE__, __LINE__, "{match}: next free list be at an offset of allocated pages");
        fails = TRUE;
    }

    if(fails) {
        print(
            "allocated_addr: %x\n"
            "next_free_addr: %x\n"
            "released_addr:  %x\n"
            "ptr dif: %u\n",
            allocated_addr,
            next_free_addr,
            released_addr,
            dif
        );
    }
}

void
test_alloc_first_release_after(const uint32 num_pages, const bool print_list)
{
    struct memhnd_t hnd1;
    struct memhnd_t hnd2;
    char* allocated_addr1 = NULL;
    char* next_free_addr1 = NULL;
    char* released_addr1 = NULL;
    char* allocated_addr2 = NULL;
    char* next_free_addr2 = NULL;
    char* released_addr2 = NULL;


    bool fails = FALSE;

    print("%sTest:alloc first - release after. 2 allocations. pages:%u %s\n", GREEN, num_pages, CLEAR);

    hnd1 = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
    if(print_list) {
        print("%s first alloc %s\n", ry, CLEAR);
        mem_log_print_free_list_of_chunk(0);
    }

    hnd2 = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
    if(print_list) {
        print("%s second alloc %s\n", ry, CLEAR);
        mem_log_print_free_list_of_chunk(0);
    }

    // for test
    allocated_addr1 = (char*)mem_get_addr_by_handle(hnd1);
    next_free_addr1 = (char*)flist_peek_next_free_page(hnd1.chunk_id);
    allocated_addr2 = (char*)mem_get_addr_by_handle(hnd2);
    next_free_addr2 = (char*)flist_peek_next_free_page(hnd2.chunk_id);

    mem_release(hnd2);
    if(print_list) {
        print("%s release second %s\n", ry, CLEAR);
        mem_log_print_free_list_of_chunk(0);
    }
    released_addr2 = (char*)flist_peek_next_free_page(hnd2.chunk_id);



    mem_release(hnd1);
    if(print_list) {
        print("%s release first %s\n", ry, CLEAR);
        mem_log_print_free_list_of_chunk(0);
    }
    released_addr1 = (char*)flist_peek_next_free_page(hnd1.chunk_id);


    // test for first alloc
    {
        // test 1: allocated memory and released memory match
        if(allocated_addr1 == released_addr1) {
            print_test_result("mem alloc-release", TRUE, __FILE__, __LINE__, "match allocated-released memory");
        } else {
            print_test_result("mem alloc-release", FALSE, __FILE__, __LINE__, "match allocated-released memory");
            fails = TRUE;
        }


        // test 2: match: next free list be at an offset of allocated pages
        uint32 dif = (uint32 )(next_free_addr1 - allocated_addr1);
        dif /= PAGE_SIZE;
        if(dif == hnd1.num_pages * 2) {
            print_test_result("mem alloc-release", TRUE, __FILE__, __LINE__, "{match}: next free list be at an offset of allocated pages");
        } else {
            print_test_result("mem alloc-release", FALSE, __FILE__, __LINE__, "{match}: next free list be at an offset of allocated pages");
            fails = TRUE;
        }

        if(fails) {
            print(
                "allocated_addr: %x\n"
                "next_free_addr: %x\n"
                "released_addr:  %x\n"
                "ptr dif: %u\n",
                allocated_addr1,
                next_free_addr1,
                released_addr1,
                dif
            );
        }

    }

    // test for second alloc
    {
        // test 1: allocated memory and released memory match
        if(allocated_addr2 == released_addr2) {
            print_test_result("mem alloc-release", TRUE, __FILE__, __LINE__, "match allocated-released memory");
        } else {
            print_test_result("mem alloc-release", FALSE, __FILE__, __LINE__, "match allocated-released memory");
            fails = TRUE;
        }

        // test 2: match: next free list be at an offset of allocated pages
        uint32 dif = (uint32 )(next_free_addr2 - allocated_addr2);
        dif /= PAGE_SIZE;
        if(dif == hnd1.num_pages) {
            print_test_result("mem alloc-release", TRUE, __FILE__, __LINE__, "{match}: next free list be at an offset of allocated pages");
        } else {
            print_test_result("mem alloc-release", FALSE, __FILE__, __LINE__, "{match}: next free list be at an offset of allocated pages");
            fails = TRUE;
        }

        if(fails) {
            print(
                "allocated_addr: %x\n"
                "next_free_addr: %x\n"
                "released_addr:  %x\n"
                "ptr dif: %u\n",
                allocated_addr2,
                next_free_addr2,
                released_addr2,
                dif
            );
        }

    }

}



/***************************************************************************************************
 * allocate many then release many, not in order
 * alloc(1), alloc(2), alloc(3), ... then release(2), release(1), release(3)
 */
void
test_alloc_first_release_after_backwards(const uint32 num_pages, const bool print_list)
{
    uint16 cnt1 = 0;
    uint16 cnt2 = 1;
    uint16 cnt3 = 2;
    struct memhnd_t hnd[3];
    char* allocated_addr[3];
    char* next_free_addr[3];
    char* released_addr[3];


    bool fails = FALSE;

    print("%sTest:alloc first - release after backwards.\n 3 allocations. pages:%u %s\n", GREEN, num_pages, CLEAR);
    {
        hnd[cnt1] = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        if(print_list) {
            print("%s first alloc %s\n", ry, CLEAR);
            mem_log_print_free_list_of_chunk(0);
        }
        hnd[cnt2] = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        if(print_list) {
            print("%s second alloc %s\n", ry, CLEAR);
            mem_log_print_free_list_of_chunk(0);
        }
        hnd[cnt3] = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        if(print_list) {
            print("%s third alloc %s\n", ry, CLEAR);
            mem_log_print_free_list_of_chunk(0);
        }

        // for test
        allocated_addr[cnt1] = (char*)mem_get_addr_by_handle(hnd[cnt1]);
        next_free_addr[cnt1] = (char*)flist_peek_next_free_page(hnd[cnt1].chunk_id);
        allocated_addr[cnt2] = (char*)mem_get_addr_by_handle(hnd[cnt2]);
        next_free_addr[cnt2] = (char*)flist_peek_next_free_page(hnd[cnt2].chunk_id);
        allocated_addr[cnt3] = (char*)mem_get_addr_by_handle(hnd[cnt3]);
        next_free_addr[cnt3] = (char*)flist_peek_next_free_page(hnd[cnt3].chunk_id);


        mem_release(hnd[cnt3]);
        released_addr[cnt3] = (char*)flist_peek_next_free_page(hnd[cnt3].chunk_id);
        if(print_list) {
            print("%s release third %s\n", ry, CLEAR);
            mem_log_print_free_list_of_chunk(0);
        }
        mem_release(hnd[cnt2]);
        released_addr[cnt2] = (char*)flist_peek_next_free_page(hnd[cnt2].chunk_id);
        if(print_list) {
            print("%s release second %s\n", ry, CLEAR);
            mem_log_print_free_list_of_chunk(0);
        }
        mem_release(hnd[cnt1]);
        released_addr[cnt1] = (char*)flist_peek_next_free_page(hnd[cnt1].chunk_id);
        if(print_list) {
            print("%s release first %s\n", ry, CLEAR);
            mem_log_print_free_list_of_chunk(0);
        }



        const uint32 ptrdif_to_page_num = PAGE_SIZE;

        // test first
        {
            // test 1: allocated memory and released memory match
            if(allocated_addr[cnt1] == released_addr[cnt1]) {
                print_test_result("mem alloc-release", TRUE, __FILE__, __LINE__, "match allocated-released memory");
            } else {
                print_test_result("mem alloc-release", FALSE, __FILE__, __LINE__, "match allocated-released memory");
                fails = TRUE;
            }
            // test 2: match: next free list be at an offset of allocated pages
            uint32 dif1 = (uint32 )(next_free_addr[cnt1] - allocated_addr[cnt1]);
            dif1 /= ptrdif_to_page_num;
            if(dif1 == hnd[cnt1].num_pages*3) {
                print_test_result("mem alloc-release", TRUE, __FILE__, __LINE__, "{match}: next free list be at an offset of allocated pages");
            } else {
                print_test_result("mem alloc-release", FALSE, __FILE__, __LINE__, "{match}: next free list be at an offset of allocated pages");
                fails = TRUE;
            }
            if(fails) {
                print("1st:\n"
                    "allocated_addr: %x\n"
                    "next_free_addr: %x\n"
                    "released_addr:  %x\n"
                    "ptr dif: %u\n",
                    allocated_addr[cnt1],
                    next_free_addr[cnt1],
                    released_addr[cnt1],
                    dif1
                );
            }

        }
        // test second
        {
            // test 1: allocated memory and released memory match
            if(allocated_addr[cnt2] == released_addr[cnt2]) {
                print_test_result("mem alloc-release", TRUE, __FILE__, __LINE__, "match allocated-released memory");
            } else {
                print_test_result("mem alloc-release", FALSE, __FILE__, __LINE__, "match allocated-released memory");
                fails = TRUE;
            }
            // test 2: match: next free list be at an offset of allocated pages
            uint32 dif2 = (uint32 )(next_free_addr[cnt2] - allocated_addr[cnt2]);
            dif2 /= ptrdif_to_page_num;
            if(dif2 == hnd[cnt2].num_pages*2) {
                print_test_result("mem alloc-release", TRUE, __FILE__, __LINE__, "{match}: next free list be at an offset of allocated pages");
            } else {
                print_test_result("mem alloc-release", FALSE, __FILE__, __LINE__, "{match}: next free list be at an offset of allocated pages");
                fails = TRUE;
            }
            if(fails) {
                print("2nd:\n"
                    "allocated_addr: %x\n"
                    "next_free_addr: %x\n"
                    "released_addr:  %x\n"
                    "ptr dif: %u\n",
                    allocated_addr[cnt2],
                    next_free_addr[cnt2],
                    released_addr[cnt2],
                    dif2
                );
            }

        }
        // test third
        {
            // test 1: allocated memory and released memory match
            if(allocated_addr[cnt3] == released_addr[cnt3]) {
                print_test_result("mem alloc-release", TRUE, __FILE__, __LINE__, "match allocated-released memory");
            } else {
                print_test_result("mem alloc-release", FALSE, __FILE__, __LINE__, "match allocated-released memory");
                fails = TRUE;
            }
            // test 2: match: next free list be at an offset of allocated pages
            uint32 dif3 = (uint32 )(next_free_addr[cnt3] - allocated_addr[cnt3]);
            dif3 /= ptrdif_to_page_num;
            if(dif3 == hnd[cnt3].num_pages*1) {
                print_test_result("mem alloc-release", TRUE, __FILE__, __LINE__, "{match}: next free list be at an offset of allocated pages");
            } else {
                print_test_result("mem alloc-release", FALSE, __FILE__, __LINE__, "{match}: next free list be at an offset of allocated pages");
                fails = TRUE;
            }
            if(fails) {
                print("3rd:\n"
                    "allocated_addr: %x\n"
                    "next_free_addr: %x\n"
                    "released_addr:  %x\n"
                    "ptr dif: %u\n",
                    allocated_addr[cnt3],
                    next_free_addr[cnt3],
                    released_addr[cnt3],
                    dif3
                );
            }
        }

    }

}


/***************************************************************************************************
 * allocate many then release many, not in order
 * alloc(1), alloc(2), alloc(3), ... then release(2), release(1), release(3)
 */
void
test_alloc_release_mixed_all()
{
    uint32 num_pages = 1;

    memman_init();

    print("%s -- TEST --\n"
          "  alloc:(1),(3),(2),(2) x 3times\n"
          "  release in order, %s\n", GREEN, CLEAR);
    {
        struct memhnd_t hnd1 = mem_alloc(1, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t hnd2 = mem_alloc(3, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t hnd3= mem_alloc(2, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t hnd4 = mem_alloc(2, MEM_CHUNK_ALLOCATOR_SLAB);

        mem_release(hnd1);
        mem_release(hnd2);
        mem_release(hnd3);
        mem_release(hnd4);
    }
    print("%s -- FINISH -- %s\n", YELLOW, CLEAR);


    print("%s -- TEST --\n"
          "  alloc:(1),(3),(2),(2) x 3times\n"
          "  release NOT in order, %s\n", GREEN, CLEAR);
    {
        struct memhnd_t hnd1 = mem_alloc(1, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t hnd2 = mem_alloc(3, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t hnd3= mem_alloc(2, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t hnd4 = mem_alloc(2, MEM_CHUNK_ALLOCATOR_SLAB);

        mem_release(hnd4);
        mem_release(hnd1);
        mem_release(hnd2);
        mem_release(hnd3);
    }


    memman_terminate();
}

void
test_alloc_release_fragment_mem()
{
    uint32 num_pages = 1;

    memman_init();

    print("%s -- TEST --\n"
          "  TODO\n"
          "  TODO %s\n", GREEN, CLEAR);
    {
        struct memhnd_t hnd1 = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t hnd2 = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t hnd3= mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t hnd4 = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id5 = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id6 = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id7 = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id8 = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id9 = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t hnd10 = mem_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);

        mem_release(hnd3);
        mem_release(hnd4);
        mem_release(id5);

        print("%s#### FRAGMENTED HERE%s\n", YELLOW, CLEAR);


        struct memhnd_t hnd11 = mem_alloc(3, MEM_CHUNK_ALLOCATOR_SLAB);

        print("%s#### ALLOCATED NEW MEM HERE%s\n", YELLOW, CLEAR);

        mem_release(hnd1);
        mem_release(hnd2);
        mem_release(hnd3);
        mem_release(hnd4);
        mem_release(id5);
        mem_release(id6);
        mem_release(id7);
        mem_release(id8);
        mem_release(id9);
        mem_release(hnd10);
        mem_release(hnd11);


        print("%s#### RELEASE ALL HERE%s\n", YELLOW, CLEAR);
    }
    print("%s -- FINISH -- %s\n", YELLOW, CLEAR);

    print("%s -- TEST --\n"
          "  TODO\n"
          "  TODO %s\n", GREEN, CLEAR);
    // {
    //     struct memhnd_t hnd1 = mem_alloc(1); memory_manager_memset(hnd1, 1, 0x0f);
    //     struct memhnd_t hnd2 = mem_alloc(1); memory_manager_memset(hnd2, 1, 0x0ff);
    //     struct memhnd_t hnd3= mem_alloc(1); memory_manager_memset(hnd3, 1, 0x0f);
    //     uint32 hnd4 = mem_alloc(1); memory_manager_memset(hnd4, 1, 0x0f);
    //     uint32 id5 = mem_alloc(1); memory_manager_memset(id5, 1, 0x0f);
    //     uint32 id6 = mem_alloc(1); memory_manager_memset(id6, 1, 0x0f);
    //     uint32 id7 = mem_alloc(1); memory_manager_memset(id7, 1, 0x0f);
    //     uint32 id8 = mem_alloc(1); memory_manager_memset(id8, 1, 0x0f);
    //
    //     mem_release(hnd1);
    //     mem_release(hnd3);
    //     mem_release(id5);
    //     mem_release(id7);
    //
    //     print("%s####################\n"
    //           "#### FRAGMENTED HERE\n"
    //           "####################%s\n", YELLOW, CLEAR);
    //
    //
    //     // TODO: HERE: should ERROR, cause there is no 2xPAGE_SIZE free memory. We write over used memory!!!
    //     uint32 id9 = mem_alloc(2); memory_manager_memset(hnd1, 2, 'A');
    //     uint32 hnd10 = mem_alloc(2);memory_manager_memset(hnd1, 2, 'B');
    //     uint32 hnd11 = mem_alloc(2);memory_manager_memset(hnd1, 2, 'C');
    //     uint32 hnd12 = mem_alloc(2);memory_manager_memset(hnd1, 2, 'D');
    //
    //     print("%s#### ALLOCATED NEW MEM HERE%s\n", YELLOW, CLEAR);
    //
    //     mem_release(id8);
    //     mem_release(id6);
    //     mem_release(hnd10);
    //     mem_release(hnd4);
    //     mem_release(id9);
    //     mem_release(hnd12);
    //     mem_release(hnd2);
    //     mem_release(hnd11);
    //
    //
    //     print("%s#### RELEASE ALL HERE%s\n", YELLOW, CLEAR);
    // }


    memman_terminate();
}


/***************************************************************************************************
 * test logger to html
 */
void
test_logger_to_html()
{
    logger_to_html_init();
    memman_init();

    {
        struct memhnd_t hnd = mem_alloc(3, MEM_CHUNK_ALLOCATOR_SLAB);
    }
    {
        struct memhnd_t hnd = mem_alloc(2, MEM_CHUNK_ALLOCATOR_SLAB);
    }

    memman_terminate();
    logger_to_html_destroy();
}
