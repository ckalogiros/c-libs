/*
 * Created by ckalo on 3/8/26.
 */

#include "test_string_allocator.h"

#include <string.h>

#include "logger.h"
#include "mem_man_interface.h"
#include "slab_allocator_internals.h"
#include "slab_allocator_mcache.h"
#include "slab_allocator_internals.h"
#include "slab_allocator_log.h"
#include "string_internals.h"

enum CHAR_ARRAY {
    CHAR_A=65,
    CHAR_END = 90,
};


void test_string_allocator(void)
{

}



void string_check_buffer(void* buffer, const uint32 string_size, const char c, const uint32 index);

char* slab_allocator_mcache_free_slab_test_debug(struct slab_allocator_mcache* mcache, void* slab_address);
char* slab_allocator_mcache_free_slab_test_debug(struct slab_allocator_mcache* mcache, void* slab_address);
char* slab_allocator_mcache_release_buffer_test_debug(void* ptr, const slab_size_t string_size);





void
test_slab_allocator_mcache_alloc_release_slabs(void)
{
    memman_init();

    // slab_allocator_init();
    // slab_allocator_create_new_instance();
    string_container_init();


    const slab_size_t string_size = 8;
    const slab_size_t slab_size = string_size;

    // print(" %sTEST-%s\n", ry, CLEAR);
    const uint32 page_size = PAGE_SIZE;
    const uint32 total_slabs = page_size / slab_size;
    uint8* buffer[page_size / slab_size];

    // print_test("Slab Allocator",
    //     " . Allocate slab buffers\n"
    //     " . \"Color\" them with a character\n"
    //     " . Check slab buffer insert() function\n"
    //     "   that inserts elements (coloring)\n"
    //     "   in the boundaries of each slab\n"
    //     " slab_size:   %s%u bytes%s\n"
    //     " string_size: %s%u%s elems\n"
    //     " total_slabs: %s%u%s\n",
    //     ry, slab_size, CLEAR,
    //     ry, string_size, CLEAR,
    //     ry, total_slabs, CLEAR
    // );


    print("%s-- ALLOCATION + WRITE --%s\n", ry, CLEAR);
    uint32 c = CHAR_A;
    for(uint32 i = 0; i < total_slabs; i++) {
        // buffer[i] = (uint8*) slab_allocator_mcache_alloc_buffer(string_size);
        buffer[i] = (uint8*) slab_allocator_alloc(string_size, sizeof(char));
        memset(buffer[i], (char) c, slab_size);
        string_check_buffer(buffer[i], slab_size, c, i);
        c++;
        if(c > CHAR_END) c = CHAR_A;
    }


    // const void* buffer_start = &(buffer[0][0]);
    const char* buffer_start = (*buffer);
    // 0x7ffff7f1c7f8
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
        free_buffer[i] = slab_allocator_release_test_debug(buffer[i], string_size);
        string_check_buffer(buffer[i], string_size, '\0', i);
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


    mcache_table_destroy();
    memman_terminate();
}




/***************************************************************************************************
 * HELPERS
 **************************************************************************************************/
void
string_print_buffer(void* buffer, const uint32 string_size)
{
    uint32 i = 0;
    char* buf = (char*) buffer;
    for(; i < string_size; i++) {
        print("%c", buf[i]);
    }
    print("\n");
}

void
string_check_buffer(void* buffer, const uint32 string_size, const char c, const uint32 index)
{
    bool failed = FALSE;

    print("[%s%u%s]", CYAN, index, CLEAR);

    uint32 i = 0;
    char* buf = (char*) buffer;

    for(; i < string_size; i++) {
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

char*
slab_allocator_mcache_free_slab_test_debug(struct slab_allocator_mcache* mcache, void* slab_address)
{
    // TODO: write test
    struct flist* released = (struct flist*) slab_address;
    struct flist* node = mcache->slabs_free_list;
    const char* free_buffer = slab_address;

    // insert at the begining of the free list
    if(node == NULL) {
        memset(slab_address, 0x0, mcache->slab_size);
        mcache->slabs_free_list = released;
        mcache->slabs_free_list->next = NULL;
        return free_buffer;
    }

    // insert at the begining of the free list
    if(node > released) {
        memset(slab_address, 0x0, mcache->slab_size);
        mcache->slabs_free_list = released;
        mcache->slabs_free_list->next = node;
        return free_buffer;
    }


    // if smaller address found, insert in between
    if(node->next != NULL) {
        // search for the last free with smaller address value
        while(node->next  != NULL && node->next < released) {
            node = node->next;
        }

        memset(slab_address, 0x0, mcache->slab_size);
        struct flist* temp = node->next;
        node->next = released;
        node->next->next = temp;
        return free_buffer;
    }

    // else insert at the end of the free list
    memset(slab_address, 0x0, mcache->slab_size);
    node->next = (struct flist*) slab_address;
    return free_buffer;

}

char*
slab_allocator_mcache_release_buffer_test_debug(void* ptr, const slab_size_t string_size)
{
    const slab_size_t slab_size = string_size;

    struct slab_allocator_mcache* mcache = mcache_find_size(slab_size);

    if(!mcache) // handle error: cache not found
        slab_allocator_print_error(SLAB_ALLOCATOR_MEMCACHE_NOT_FOUND);

    return slab_allocator_mcache_free_slab_test_debug(mcache, ptr);
}
