/*
 * Created by ckalo on 2/16/26.
 *
 *
 */


#include "test_slab_allocator.h"
#include "slab_allocator_internals.h"
#include "slab_allocator_log.h"
#include <string.h>
#include "logger.h"


void print_buffer(void* buffer, const uint32 buffer_size)
{
    uint32 i = 0;
    char* buf = (char*)buffer;
    for(; i<buffer_size; i++) {
        print("%c", buf[i]);
    }
    print("\n");
}
void check_buffer(void* buffer, const uint32 buffer_size, const char c)
{
    bool failed = FALSE;

    uint32 i = 0;
    char* buf = (char*)buffer;
    for(; i<buffer_size; i++) {
        print("%c", buf[i]);
        if((uint8)buf[i]!=(uint8)c) {
            failed = TRUE;
            // print("-%c|", c);
        }
    }
    if(failed) print(" %sFAIL%s\n", RED, CLEAR);
    else print(" %sPASS%s\n", GREEN, CLEAR);
}


/*
 * these are a copies of the function(s) of slab_allocator, for testing purposes
 * MUST BE KEPT UPDATED TO THE ACTUAL SLAB ALLOCATOR'S FUNCTIONS
 */

bool
mem_cache_free_slab_test_debug(struct mem_cache* mcache, void* slab_address, uint64* free_buffer, const uint32 free_idx)
{
    // TODO: write test
    struct slab_flist* released =  (struct slab_flist*)slab_address;
    struct slab_flist* node =  mcache->slabs_free_list;

    const uint32 color = 0x0;
    memset(slab_address, color, mcache->slab_size);

    // insert at the begining of the free list
    if(node > released) {
        mcache->slabs_free_list = released;
        mcache->slabs_free_list->next = node;
        free_buffer[free_idx] = (uint64)mcache->slabs_free_list;
        return TRUE;
    }

    // else search for the last free with smaller address value
    while(node->next < released) {
        node = node->next;
    }

    // if smaller address found, insert in between
    if(node->next != NULL) {
        struct slab_flist* temp = node->next;
        node->next = released;
        node->next->next = temp;
        free_buffer[free_idx] = (uint64)node->next;
        return TRUE;
    }
    // else insert at the end of the free list
    node->next = (struct slab_flist*)slab_address;
    free_buffer[free_idx] = (uint64)node->next;
    return TRUE;
}

void
mem_cache_release_buffer_test_debug(void* ptr, const buffer_size_t buffer_size,
                            const uint8 type_size, uint64* free_buffer, const uint32 free_idx)
{
    const slab_size_t slab_size = buffer_size * type_size;

    struct mem_cache* mcache = mem_cache_find_cache(slab_size);

    if(!mcache) // handle error: cache not found
        slab_allocator_print_error(SLAB_ALLOC_MEMCACHE_NOT_FOUND);

    mem_cache_free_slab_test_debug(mcache, ptr, free_buffer, free_idx);
}