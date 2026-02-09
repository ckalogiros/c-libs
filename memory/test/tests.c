/*
 * Created by ckalo on 2/6/26.
 */

#include "tests.h"

#include "buffer_internals.h"
#include "logger.h"
#include "logger_color.h"
#include "mem_log.h"
#include "mem_man.h"
#include "precision_timer.h"
#include "precision_timer_logger.h"


// todo:compare malloc with custom
void
name_me_compare_perf_of_malloc_and_mem_manager() {}

void
perf_mem_big_chunk_allocation()
{
    memory_manager_init(32);
    buffer_init_table();
    timer_init_table();

    struct timer* t = timer_create_perf_timer();

    perf_timer_start(t);

    // memory_manager_init(512);

    perf_timer_stop(t);
    perf_timer_print(timer_print_config((timer_print_params){
        t, 1, 0, PERF_T_ALL
    }));
    perf_timer_reset(t);

    memory_manager_terminate();
}


/***************************************************************************************************
 * allocate then release in sequence
 * alloc(1)-release(), alloc(2)-release, ...
 */
void
test_alloc_release_insequence_at_start_by_1()
{
    uint32 num_pages = 1;
    uint32 repeat = 10;

    memory_manager_init(4);

    // mem_log_print_mem(mem_get_mem());
    mem_log_print_free_list(free_list_get_head());
    // mem_log_print_all_used_mem();

    for(uint32 i = 0; i < repeat; i++) {
        struct memhnd_t hnd = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        mms_release(hnd);
    }


    memory_manager_terminate();
}

void
test_alloc_release_insequence_at_start_by_2()
{
    uint32 num_pages = 2;
    uint32 repeat = 10;

    memory_manager_init(4);

    // mem_log_print_mem(mem_get_mem());
    mem_log_print_free_list(free_list_get_head());
    // mem_log_print_all_used_mem();

    for(uint32 i = 0; i < repeat; i++) {
        struct memhnd_t hnd = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        mms_release(hnd);
    }


    memory_manager_terminate();
}

void
test_alloc_release_insequence_at_start_by_3()
{
    uint32 num_pages = 3;
    uint32 repeat = 10;

    memory_manager_init(4);

    // mem_log_print_mem(mem_get_mem());
    mem_log_print_free_list(free_list_get_head());
    // mem_log_print_all_used_mem();

    for(uint32 i = 0; i < repeat; i++) {
        struct memhnd_t hnd = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        mms_release(hnd);
    }

    memory_manager_terminate();
}

void
test_alloc_release_insequence_at_start_by_4()
{
    uint32 num_pages = 4;
    uint32 repeat = 10;

    memory_manager_init(4);

    // mem_log_print_mem(mem_get_mem());
    mem_log_print_free_list(free_list_get_head());
    // mem_log_print_all_used_mem();

    for(uint32 i = 0; i < repeat; i++) {
        struct memhnd_t hnd = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        mms_release(hnd);
    }


    memory_manager_terminate();
}

void
test_alloc_release_insequence_at_start_mix()
{
    uint32 num_pages = 4;
    uint32 repeat = 10;

    memory_manager_init(4);

    // mem_log_print_mem(mem_get_mem());
    mem_log_print_free_list(free_list_get_head());
    // mem_log_print_all_used_mem();

    {
        struct memhnd_t hnd = mms_alloc(1, MEM_CHUNK_ALLOCATOR_SLAB);
        mms_release(hnd);
    }
    {
        struct memhnd_t hnd = mms_alloc(2, MEM_CHUNK_ALLOCATOR_SLAB);
        mms_release(hnd);
    }

    {
        struct memhnd_t hnd = mms_alloc(3, MEM_CHUNK_ALLOCATOR_SLAB);
        mms_release(hnd);
    }

    {
        struct memhnd_t hnd = mms_alloc(4, MEM_CHUNK_ALLOCATOR_SLAB);
        mms_release(hnd);
    }


    memory_manager_terminate();
}


/***************************************************************************************************
 * allocate many then release many, not in order
 * alloc(1), alloc(2), alloc(3), ... then release(2), release(1), release(3)
 */
void
test_alloc_release_unordered_at_start_by_1()
{
    uint32 num_pages = 1;

    memory_manager_init(4);

    print("%s -- TEST --\n"
          "  alloc(1) x 3times\n"
          "  release in order, %s\n", GREEN, CLEAR);
    {
        mem_log_print_free_list(free_list_get_head());
        struct memhnd_t id1 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id2 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id3 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);

        mms_release(id1);
        mms_release(id2);
        mms_release(id3);
    }
    print("%s -- FINISH -- %s\n", YELLOW, CLEAR);


    print("%s -- TEST --\n"
          "  alloc(1) x 3times\n"
          "  release NOT in order, %s\n", GREEN, CLEAR);
    {
        mem_log_print_free_list(free_list_get_head());
        struct memhnd_t id1 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id2 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id3 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);

        mms_release(id2);
        mms_release(id3);
        mms_release(id1);
    }


    memory_manager_terminate();
}

void
test_alloc_release_unordered_at_start_by_2()
{
    uint32 num_pages = 2;

    memory_manager_init(6);


    print("%s -- TEST --\n"
          "  alloc(2) x 3times\n"
          "  release in order, %s\n", GREEN, CLEAR);
    {
        mem_log_print_free_list(free_list_get_head());
        struct memhnd_t id1 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id2 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id3 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);

        mms_release(id1);
        mms_release(id2);
        mms_release(id3);
    }
    print("%s -- FINISH -- %s\n", YELLOW, CLEAR);


    print("%s -- TEST --\n"
          "  alloc(2) x 3times\n"
          "  release NOT in order, %s\n", GREEN, CLEAR);
    {
        mem_log_print_free_list(free_list_get_head());
        struct memhnd_t id1 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id2 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id3 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);

        mms_release(id2);
        mms_release(id3);
        mms_release(id1);
    }


    memory_manager_terminate();
}

void
test_alloc_release_unordered_at_start_by_3()
{
    uint32 num_pages = 3;

    memory_manager_init(9);


    print("%s -- TEST --\n"
          "  alloc(3) x 3times\n"
          "  release in order, %s\n", GREEN, CLEAR);
    {
        mem_log_print_free_list(free_list_get_head());
        struct memhnd_t id1 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id2 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id3 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);

        mms_release(id1);
        mms_release(id2);
        mms_release(id3);
    }
    print("%s -- FINISH -- %s\n", YELLOW, CLEAR);


    print("%s -- TEST --\n"
          "  alloc(3) x 3times\n"
          "  release NOT in order, %s\n", GREEN, CLEAR);
    {
        mem_log_print_free_list(free_list_get_head());
        struct memhnd_t id1 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id2 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id3 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);

        mms_release(id2);
        mms_release(id3);
        mms_release(id1);
    }


    memory_manager_terminate();
}

void
test_alloc_release_unordered_at_start_by_4()
{
    uint32 num_pages = 4;

    memory_manager_init(12);


    print("%s -- TEST --\n"
          "  alloc(3) x 3times\n"
          "  release in order, %s\n", GREEN, CLEAR);
    {
        mem_log_print_free_list(free_list_get_head());
        struct memhnd_t id1 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id2 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id3 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);

        mms_release(id1);
        mms_release(id2);
        mms_release(id3);
    }
    print("%s -- FINISH -- %s\n", YELLOW, CLEAR);


    print("%s -- TEST --\n"
          "  alloc(3) x 3times\n"
          "  release NOT in order, %s\n", GREEN, CLEAR);
    {
        mem_log_print_free_list(free_list_get_head());
        struct memhnd_t id1 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id2 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id3 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);

        mms_release(id2);
        mms_release(id3);
        mms_release(id1);
    }


    memory_manager_terminate();
}


/***************************************************************************************************
 * allocate many then release many, not in order
 * alloc(1), alloc(2), alloc(3), ... then release(2), release(1), release(3)
 */
void
test_alloc_release_mixed_all()
{
    uint32 num_pages = 1;

    memory_manager_init(10);

    print("%s -- TEST --\n"
          "  alloc:(1),(3),(2),(2) x 3times\n"
          "  release in order, %s\n", GREEN, CLEAR);
    {
        mem_log_print_free_list(free_list_get_head());
        struct memhnd_t id1 = mms_alloc(1, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id2 = mms_alloc(3, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id3= mms_alloc(2, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id4 = mms_alloc(2, MEM_CHUNK_ALLOCATOR_SLAB);

        mms_release(id1);
        mms_release(id2);
        mms_release(id3);
        mms_release(id4);
    }
    print("%s -- FINISH -- %s\n", YELLOW, CLEAR);


    print("%s -- TEST --\n"
          "  alloc:(1),(3),(2),(2) x 3times\n"
          "  release NOT in order, %s\n", GREEN, CLEAR);
    {
        mem_log_print_free_list(free_list_get_head());
        struct memhnd_t id1 = mms_alloc(1, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id2 = mms_alloc(3, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id3= mms_alloc(2, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id4 = mms_alloc(2, MEM_CHUNK_ALLOCATOR_SLAB);

        mms_release(id4);
        mms_release(id1);
        mms_release(id2);
        mms_release(id3);
    }


    memory_manager_terminate();
}

void
test_alloc_release_fragment_mem()
{
    uint32 num_pages = 1;

    memory_manager_init(10);

    print("%s -- TEST --\n"
          "  TODO\n"
          "  TODO %s\n", GREEN, CLEAR);
    {
        mem_log_print_free_list(free_list_get_head());
        struct memhnd_t id1 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id2 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id3= mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id4 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id5 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id6 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id7 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id8 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id9 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
        struct memhnd_t id10 = mms_alloc(num_pages, MEM_CHUNK_ALLOCATOR_SLAB);

        mms_release(id3);
        mms_release(id4);
        mms_release(id5);

        print("%s#### FRAGMENTED HERE%s\n", YELLOW, CLEAR);
        mem_log_print_free_list(free_list_get_head());


        struct memhnd_t id11 = mms_alloc(3, MEM_CHUNK_ALLOCATOR_SLAB);

        print("%s#### ALLOCATED NEW MEM HERE%s\n", YELLOW, CLEAR);
        mem_log_print_free_list(free_list_get_head());

        mms_release(id1);
        mms_release(id2);
        mms_release(id3);
        mms_release(id4);
        mms_release(id5);
        mms_release(id6);
        mms_release(id7);
        mms_release(id8);
        mms_release(id9);
        mms_release(id10);
        mms_release(id11);


        print("%s#### RELEASE ALL HERE%s\n", YELLOW, CLEAR);
    }
    print("%s -- FINISH -- %s\n", YELLOW, CLEAR);

    print("%s -- TEST --\n"
          "  TODO\n"
          "  TODO %s\n", GREEN, CLEAR);
    // {
    //     mem_log_print_free_list(free_list_get_head());
    //     struct memhnd_t id1 = mms_alloc(1); memory_manager_memset(id1, 1, 0x0f);
    //     struct memhnd_t id2 = mms_alloc(1); memory_manager_memset(id2, 1, 0x0ff);
    //     struct memhnd_t id3= mms_alloc(1); memory_manager_memset(id3, 1, 0x0f);
    //     uint32 id4 = mms_alloc(1); memory_manager_memset(id4, 1, 0x0f);
    //     uint32 id5 = mms_alloc(1); memory_manager_memset(id5, 1, 0x0f);
    //     uint32 id6 = mms_alloc(1); memory_manager_memset(id6, 1, 0x0f);
    //     uint32 id7 = mms_alloc(1); memory_manager_memset(id7, 1, 0x0f);
    //     uint32 id8 = mms_alloc(1); memory_manager_memset(id8, 1, 0x0f);
    //
    //     mms_release(id1);
    //     mms_release(id3);
    //     mms_release(id5);
    //     mms_release(id7);
    //
    //     print("%s####################\n"
    //           "#### FRAGMENTED HERE\n"
    //           "####################%s\n", YELLOW, CLEAR);
    //     mem_log_print_free_list(free_list_get_head());
    //
    //
    //     // TODO: HERE: should ERROR, cause there is no 2xPAGE_SIZE free memory. We write over used memory!!!
    //     uint32 id9 = mms_alloc(2); memory_manager_memset(id1, 2, 'A');
    //     uint32 id10 = mms_alloc(2);memory_manager_memset(id1, 2, 'B');
    //     uint32 id11 = mms_alloc(2);memory_manager_memset(id1, 2, 'C');
    //     uint32 id12 = mms_alloc(2);memory_manager_memset(id1, 2, 'D');
    //
    //     print("%s#### ALLOCATED NEW MEM HERE%s\n", YELLOW, CLEAR);
    //     mem_log_print_free_list(free_list_get_head());
    //
    //     mms_release(id8);
    //     mms_release(id6);
    //     mms_release(id10);
    //     mms_release(id4);
    //     mms_release(id9);
    //     mms_release(id12);
    //     mms_release(id2);
    //     mms_release(id11);
    //
    //
    //     print("%s#### RELEASE ALL HERE%s\n", YELLOW, CLEAR);
    // }


    memory_manager_terminate();
}


/***************************************************************************************************
 * test logger to html
 */
void
test_logger_to_html()
{
    logger_to_html_init();
    memory_manager_init(10);

    mem_log_print_mem(mem_get_mem());
    mem_log_print_free_list(free_list_get_head());
    mem_log_print_all_used_mem();

    {
        struct memhnd_t hnd = mms_alloc(3, MEM_CHUNK_ALLOCATOR_SLAB);
    }
    {
        struct memhnd_t hnd = mms_alloc(2, MEM_CHUNK_ALLOCATOR_SLAB);
    }

    memory_manager_terminate();
    logger_to_html_destroy();
}
