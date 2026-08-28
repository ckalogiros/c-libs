/*
 * Created by ckalo on 2/16/26.
 *
 *
 */


#include "tests_other.h"
#include "slab_allocator_internals.h"
#include "slab_allocator_log.h"
#include <string.h>
#include "bits.h"
#include "file_sys_interface.h"
#include "logger.h"
#include "slab_allocator_fprint.h"
#include "smath.h"
#include "string_internals.h"
#include "unit_test.h"



enum CHAR_ARRAY {
    CHAR_A=65,
    CHAR_END = 90,
};


void test_slab_allocator(const uint32 num_pages)
{
    memid_t inst_id = slab_allocator_create_new_instance(num_pages, "test_slab_allocator()");
    struct slab_allocator_mcache* mcache = NULL;

    { //
        typedef uint8 T;
        uint32 num_elems = 32;
        uint32 type_size   = sizeof(T);
        T* buffer = (T*) slab_allocator_alloc(inst_id, num_elems, type_size, "test create buffer uint8");
        for(uint32 i = 0; i < num_elems; i++) {
            buffer[i] = i;
        }
        // mcache = slab_allocator_get_cache(inst_id, num_elems, type_size);
        fprint_append_a_slab(buffer, num_elems*type_size, "buffer[0] uint8");

        // slab_allocator_release(inst_id, buffer, num_elems, type_size);
    }
    { //
        typedef uint16 T;
        uint32 num_elems = 32;
        uint32 type_size   = sizeof(T);
        T* buffer = (T*) slab_allocator_alloc(inst_id, num_elems, type_size, "test create buffer uint16");
        for(uint32 i = 0; i < num_elems; i++) {
            buffer[i] = i;
        }
        fprint_append_a_slab(buffer, num_elems*type_size, "buffer[0] uint16");

        // slab_allocator_release(inst_id, buffer, num_elems, type_size);
    }
    { //
        typedef uint32 T;
        uint32 num_elems = 32;
        uint32 type_size   = sizeof(T);
        T* buffer = (T*) slab_allocator_alloc(inst_id, num_elems, type_size, "test create buffer uint32");
        for(uint32 i = 0; i < num_elems; i++) {
            buffer[i] = i;
        }
        fprint_append_a_slab(buffer, num_elems*type_size, "buffer[0] uint32");

        slab_allocator_release(inst_id, buffer, num_elems, type_size);
    }
    { //
        typedef uint64 T;
        uint32 num_elems = 32;
        uint32 type_size   = sizeof(T);
        T* buffer = (T*) slab_allocator_alloc(inst_id, num_elems, type_size, "test create buffer uint64");
        for(uint32 i = 0; i < num_elems; i++) {
            buffer[i] = i;
        }
        fprint_append_a_slab(buffer, num_elems*type_size, "buffer[0] uint64");

        slab_allocator_release(inst_id, buffer, num_elems, type_size);
    }
    { //
        typedef struct v_128 {
            uint64 a; uint64 b;
        } T;
        // typedef struct v_128 T;
        uint32 num_elems = 16;
        uint32 type_size   = sizeof(T);
        T* buffer = (T*) slab_allocator_alloc(inst_id, num_elems, type_size, "test create buffer 128 typesize");
        for(uint32 i = 0; i < num_elems; i++) {
            buffer[i].a = i+0x30;
            // buffer[i].b = i+0x40;
        }
        fprint_append_a_slab(buffer, num_elems*type_size, "buffer[0] 128 typesize");

        slab_allocator_release(inst_id, buffer, num_elems, type_size);
    }

    // fprint_append_all_slabs_of_mcache();

}


/***************************************************************************************************
 * TEST  FUNCTIONS
 **************************************************************************************************/
// some basic initialization of slab allocator
void test_check_alloc_extend_pages(const uint32 num_pages)
{
    memid_t slab_inst_id = slab_allocator_create_new_instance(num_pages, "test_check_alloc_extend_pages()");

    const uint32 size = ((num_pages*PAGE_SIZE)/num_pages)/16;
    const uint32 buffer_size = (size < 16) ? size : 16;
    const uint32 type_size = 16;


    const uint32 num_allocations = (num_pages*PAGE_SIZE)/(buffer_size*type_size);
    uint32* buffer[num_allocations] = {};


    print_test("Slab Allocator",
        " Allocate - deallocate slab buffers\n"
        " Allocations are greater than a page\n"
        " Check if deallocated slabs are sorted\n"
        " and none is missing\n"
        " size:            %s%u bytes%s\n"
        " type_size:       %s%u bytes%s\n"
        " buffer_size:     %s%u%s elems\n"
        " num_allocations: %s%u%s\n",
        ry, size, CLEAR,
        ry, type_size, CLEAR,
        ry, buffer_size, CLEAR,
        ry, num_allocations, CLEAR
    );

    uint32 i = 0;
    for(; i < num_allocations; i++) {
        buffer[i] = (uint32*) slab_allocator_alloc(slab_inst_id, buffer_size, type_size, "test_check_alloc_extend_pages(#1)");
        print("%sAllocating Buffer. index:%u%s | ", ry, i, CLEAR);
        print("address:%s%x%s\n", CYAN, REDUCE_ADDR(buffer[i], F6), CLEAR);
        memset(buffer[i], 'A'+i, buffer_size*type_size); // memset whole buffer slab
        fprint_append_a_slab(buffer[i], buffer_size*type_size, "test_check_alloc_extend_pages: INITIALIZED");
    }

    for(uint32 i = 0; i < num_allocations; i++) {
        print("%sRelease Buffer. index:%u%s | ", ry, i, CLEAR);
        print("address:%s%x%s\n", br3, REDUCE_ADDR(buffer[i], F6), CLEAR);
        slab_allocator_release(slab_inst_id, buffer[i], buffer_size, type_size);
        fprint_append_a_slab(buffer[i], buffer_size*type_size, "test_check_alloc_extend_pages: RELEASED");
    }


    /*
     * repeat
     */
    print("%s---------------------------------------%s\n", GREEN, CLEAR);
    print("%srepeat allocations, with random indexing%s\n", GREEN, CLEAR);
    print("%s---------------------------------------%s\n\n", GREEN, CLEAR);
    // uint32 rand_indexing[8] = {3,7, 1,2, 6,0, 5,4};
    uint32 rand_indexing[num_allocations] = {};
    randomize_range(*(&rand_indexing), 0, num_allocations);

    i = 0;
    for(; i < num_allocations; i++) {
        print("%sAllocating Buffer. index:%u%s | ", ry, rand_indexing[i], CLEAR);
        print("address:%s%x%s\n", CYAN, REDUCE_ADDR(buffer[rand_indexing[i]], F6), CLEAR);
        buffer[rand_indexing[i]] = (uint32*) slab_allocator_alloc(slab_inst_id, buffer_size, type_size, "test_check_alloc_extend_pages(#2)");
        memset(buffer[rand_indexing[i]], 'A'+i, buffer_size*type_size); // memset whole buffer slab
    }

    for(uint32 i = 0; i < num_allocations; i++) {
        print("%sRelease Buffer. index:%u%s | ", ry, i, CLEAR);
        print("address:%s%x%s\n", br3, REDUCE_ADDR(buffer[i], F6), CLEAR);
        slab_allocator_release(slab_inst_id, buffer[i], buffer_size, type_size);
    }

    print("%sTEST free slabs list:%s\n", yg, CLEAR);
    uint32 failed = 0;
    struct flist* free = mcache_flist_peek_next_free_slab(slab_inst_id, buffer_size, type_size);
    i=0;
    while(free) {
        print("  %s[%u]%s addr:%s%x%s\n", YELLOW, i,CLEAR, CYAN, REDUCE_ADDR(free, F6), CLEAR);
        free = free->next;
        uint32 ptrdif = (uint64)free - (uint64)free;
        if(ptrdif != buffer_size*type_size)
            failed++;
        i++;
    }
    print("%sTEST: if free slabs are in order\n"
          "      and in slab sized stride%s\n", ry, CLEAR);
    if(failed) print("%sPASS%s\n", GREEN, CLEAR);
    else print("%sFAIL%s\n", RED, CLEAR);



}

void
test_mem_cache_alloc_release_slabs(void)
{
    uint16 slab_inst_id = slab_allocator_create_new_instance(4, "test_mem_cache_alloc_release_slabs");
    // string_container_init();

    const uint8 type_size = sizeof(uint64);
    const slab_elems_t buffer_size = 8;
    const slab_elems_t slab_size = buffer_size * type_size;

    // print(" %sTEST-%s\n", ry, CLEAR);
    const uint32 page_size = PAGE_SIZE;
    const uint32 total_slabs = page_size / slab_size;
    uint8* buffer[page_size / slab_size];

    print_test("Slab Allocator",
        " . Allocate slab buffers\n"
        " . \"Color\" them with a character\n"
        " . Check slab buffer insert() function\n"
        "   that inserts elements (coloring)\n"
        "   in the boundaries of each slab\n"
        " slab_size:   %s%u bytes%s\n"
        " type_size:   %s%u bytes%s\n"
        " buffer_size: %s%u%s elems\n"
        " total_slabs: %s%u%s\n",
        ry, slab_size, CLEAR,
        ry, type_size, CLEAR,
        ry, buffer_size, CLEAR,
        ry, total_slabs, CLEAR
    );


    print("%s-- ALLOCATION + WRITE --%s\n", ry, CLEAR);
    uint32 c = CHAR_A;
    for(uint32 i = 0; i < total_slabs; i++) {
        buffer[i] = (uint8*) slab_allocator_alloc(slab_inst_id, buffer_size, type_size, "test_mem_cache_alloc_release_slabs()");
        memset(buffer[i], (char) c, slab_size);
        check_buffer(buffer[i], slab_size, c, i);

        fprint_append_a_slab(buffer[i], buffer_size*type_size, "test_mem_cache_alloc_release_slabs");

        c++;
        if(c > CHAR_END) c = CHAR_A;
    }
    // fprint_append_a_slab(buffer, total_slabs*buffer_size*type_size, "test_mem_cache_alloc_release_slabs");


    const char* buffer_start = (*buffer);
    const char* buffer_end = ((*buffer) + (total_slabs * slab_size));
    print(" alloc start: %s%x%s\n", CYAN, REDUCE_ADDR(buffer_start, F5), CLEAR);
    print(" alloc end:   %s%x%s\n", CYAN, REDUCE_ADDR(buffer_end, F5), CLEAR);
    print(" alloc size:  %s%llu%s bytes\n", CYAN, buffer_end - buffer_start, CLEAR);
    if((uint64) buffer_end - (uint64) buffer_start == page_size)
        print(" Allocation size :: %sPASS%s\n", GREEN, CLEAR);
    else print(" Allocation size ::  %sFAIL%s\n", RED, CLEAR);

    print("\n\n");


    /*
     * . release all slabs of the allocated memory cache
     * . use a buffer to store all free slabs addresses for testing
     * . the release slab functions are copies of the implementation(for debugging)
     *   so make sure they are updated on any changes
     */
    char* free_buffer[total_slabs] = {};
    print("%s-- DEALLOCATION + ERASE --%s\n", ry, CLEAR);
    for(uint32 i = 0; i < total_slabs; i++) {
        struct slab_release_ret ret = mem_cache_release_buffer_test_debug(slab_inst_id, buffer[i], buffer_size, type_size);
        free_buffer[i] = ret.freed;
        check_buffer(buffer[i], buffer_size * type_size, '\0', i);
    }

    /*
     * . print all free slabs addresses
     * . all slabs ought to be free at this point
     * . if the difference between 2 consecutive slabs
     *   is the size of the slab in bytes: TEST PASSES, else TEST: FAILS
     */
    uint64 dif = (uint64)free_buffer[0];
    print(" slab_size:%u\n", slab_size);
    for(uint32 i = 1; i < total_slabs; i++) {
        dif = (uint64) free_buffer[i] - dif;

        print("[%s%u%s]"
              " addr:%s%x%s"
              "",
              CYAN, i, CLEAR,
              CYAN, REDUCE_ADDR(free_buffer[i], F5), CLEAR
        );
        if(dif == slab_size) print(" %sPASS%s\n", GREEN, CLEAR);
        else print(" %sFAIL%s\n", RED, CLEAR);

        dif = (uint64)free_buffer[i];
    }



}


