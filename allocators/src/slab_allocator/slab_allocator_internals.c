/*
 * Created by ckalo on 1/22/26.
 *
 * todo:
 */

#include "slab_allocator_internals.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>

#include "slab_allocator_log.h"
#include "my_macros.h"
#include "mem_man_internals.h"

#define MIN_SLABS_TABLE_LEN 128

#define SLABS_FREE_LIST_GET_PAGE_COLOR_START(fnode) ((char*)fnode + sizeof(struct slab_flist))

#define SLABALLOC_CHECK_MCACHE_TABLE(table) \
if(table == NULL) {\
    slab_allocator_print_error(SLAB_ALLOC_MEMCACHE_NO_INIT);\
}\

// hardcoded byte sizes of each cache's slab (buffer) capacity (in bytes). todo: automate
// static uint16 cache_size_bytes[] = {8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};

static struct mem_cache* mem_cache_table = NULL;
static char* slabs_table_start = NULL;
static struct slab* slabs_table = NULL;

void
mem_cache_table_init(void)
{

    {
        const uint32 min_size = sizeof(struct mem_cache) * SLAB_SIZES_LEN;
        const uint32 table_size = ROUND_UP(min_size, PAGE_SIZE);
        mem_cache_table = aligned_alloc(1024, table_size);

        uint32 i = 0, type_sizes = sizeof(char*);
        for(; i < SLAB_SIZES_LEN; i++) {
            const struct mem_cache mem_cache = (struct mem_cache){
                .type_size = type_sizes,
                .is_active = FALSE,
            };
            mem_cache_table[i] = mem_cache;

            type_sizes *= 2;
        }
    }

    // {
    //     const uint32 min_size = sizeof(struct slab) * MIN_SLABS_TABLE_LEN;
    //     const uint32 table_size = ROUND_UP(min_size, PAGE_SIZE);
    //     mem_cache_table = aligned_alloc(1024, table_size);
    //
    //     uint32 i = 0, type_sizes = sizeof(char*);
    //     for(; i < SLAB_SIZES_LEN; i++) {
    //         const struct mem_cache mem_cache = (struct mem_cache){
    //             .type_size = type_sizes,
    //             .is_active = FALSE,
    //         };
    //         mem_cache_table[i] = mem_cache;
    //
    //         type_sizes *= 2;
    //     }
    // }




    // todo: slab allocator logger print table init
}

struct mem_cache*
mem_cache_create(const buffer_size_t buffer_size, const uint16 type_size, const char* name)
{
    SLABALLOC_CHECK_MCACHE_TABLE(mem_cache_table);

    const uint32 slab_size = buffer_size * type_size;
    assert(slab_size >= buffer_size);

    // if requested allocation total size is greater or less/equal than a PAGE..
    const uint32 requested_num_pages = (slab_size <= PAGE_SIZE) ? 1 : slab_size / PAGE_SIZE;
    const uint32 slabs_capacity = (requested_num_pages * PAGE_SIZE) / slab_size;

    // allocate memory
    struct mem_cache* mcache = mem_cache_get_free_cache_QUICK_IMPL();
    const struct memhnd_t hnd = memman_alloc_pages(requested_num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
    char* ptr = (char*) memman_get_alloc_mem_by_handle(hnd);
    memset(ptr, 0x0, slab_size);

    char_buf16_copy_from_str(name, &mcache->name);
    mcache->mem_handle = hnd;
    mcache->type_size = type_size;
    mcache->slab_size = slab_size;
    mcache->slabs_capacity = slabs_capacity;
    mcache->slabs_num_pages = requested_num_pages;
    mcache->is_active = TRUE;
    mcache->slabs_free_list = (struct slab_flist*) ptr;

    /* initialize slabs free list */
    struct slab_flist* flist = (struct slab_flist*) ptr;
    uint32 i = 1;
    for(; i < mcache->slabs_capacity; i++) {
        // char* color_page = SLABS_FREE_LIST_GET_PAGE_COLOR_START(flist);
        // color_page[0] = 'S'; color_page[1] = 'L'; color_page[2] = 'A'; color_page[3] = 'B';
        ptr += slab_size;
        flist->next = (struct slab_flist*) ptr;
        flist = flist->next;
    }

    // flist->next = NULL;

    print_mem_cache_create(mcache);
    // print_slabs_free_list(mcache->slabs_free_list);

    return mcache;
}

struct mem_cache*
mem_cache_extend_allocation(struct mem_cache* const  mcache)
{
    SLABALLOC_CHECK_MCACHE_TABLE(mem_cache_table);

    const uint32 slab_size = mcache->slab_size;
    const uint32 requested_num_pages = (slab_size <= PAGE_SIZE) ? 1 : slab_size / PAGE_SIZE;
    const uint32 total_alloc_size = requested_num_pages * PAGE_SIZE;
    const uint32 slabs_capacity = total_alloc_size / slab_size;

    // allocate memory
    const struct memhnd_t hnd = memman_alloc_pages(requested_num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
    char* ptr = (char*) memman_get_alloc_mem_by_handle(hnd);
    memset(ptr, 0x0, slab_size);

    mcache->slabs_capacity += slabs_capacity;
    mcache->slabs_num_pages += requested_num_pages;
    mcache->is_active = TRUE;

    /* initialize slabs free list
     * Suposedly the mcache->slabs_free_list is NULL,
     * since this function is called for extending
     * mcache's allocation size
     */
    mcache->slabs_free_list = (struct slab_flist*) ptr;
    struct slab_flist* flist = (struct slab_flist*) ptr;
    uint32 i = 1;
    for(; i < slabs_capacity; i++) {
        ptr += slab_size;
        flist->next = (struct slab_flist*) ptr;
        flist = flist->next;
    }

    // flist->next = NULL;

    // print_mem_cache_create(mcache);
    // print_slabs_free_list(mcache->slabs_free_list);

    return mcache;
}

void
mem_cache_table_destroy(void)
{
    uint32 i = 0;
    for(; i < SLAB_SIZES_LEN; i++) {
        if(mem_cache_table[i].is_active) {
            memman_release(mem_cache_table[i].mem_handle);
        }
    }

    if(mem_cache_table)
        free(mem_cache_table);

    // todo: slab allocator logger print table destroy
}


struct slab_flist*
mem_cache_alloc_slab(struct mem_cache* mcache)
{
    // TODO: if there are no more free slabs, allocate new page and setup slabs
    if(mcache->slabs_free_list == NULL) {
        mcache = mem_cache_extend_allocation(mcache);
    }

    struct slab_flist* free = mcache->slabs_free_list;
    const struct slab_flist* flist = mcache->slabs_free_list;
    mcache->slabs_free_list = flist->next;
    mcache->slabs_used++;

    return free;
}


bool
mem_cache_free_slab(struct mem_cache* mcache, void* slab_address)
{
    // TODO: write test
    struct slab_flist* released = (struct slab_flist*) slab_address;
    struct slab_flist* node = mcache->slabs_free_list;

    // insert at the begining of the free list
    if(node == NULL) {
        memset(slab_address, 0x0, mcache->slab_size);
        mcache->slabs_free_list = released;
        mcache->slabs_free_list->next = NULL;
        // print_slabs_free_list(mcache->slabs_free_list);
        return TRUE;
    }

    // insert at the begining of the free list
    if(node > released) {
        memset(slab_address, 0x0, mcache->slab_size);
        mcache->slabs_free_list = released;
        mcache->slabs_free_list->next = node;
        // print_slabs_free_list(mcache->slabs_free_list);
        return TRUE;
    }


    // if smaller address found, insert in between
    if(node->next != NULL) {
        // search for the last free with smaller address value
        while(node->next  != NULL && node->next < released) {
            node = node->next;
        }
        memset(slab_address, 0x0, mcache->slab_size);
        struct slab_flist* temp = node->next;
        node->next = released;
        node->next->next = temp;
        // print_slabs_free_list(mcache->slabs_free_list);
        return TRUE;
    }

    // else insert at the end of the free list
    memset(slab_address, 0x0, mcache->slab_size);
    node->next = (struct slab_flist*) slab_address;
    // print_slabs_free_list(mcache->slabs_free_list);

    return TRUE;
}

struct mem_cache*
mem_cache_find_cache(const slab_size_t slab_size)
{
    SLABALLOC_CHECK_MCACHE_TABLE(mem_cache_table);
    uint32 i = 0;
    for(; i < SLAB_SIZES_LEN; i++) {
        if(mem_cache_table[i].slab_size == slab_size) {
            return &mem_cache_table[i];
        }
    }

    return NULL;
}

struct mem_cache* mem_cache_get_free_cache_QUICK_IMPL(void)
{
    uint32 i = 0;
    for(; i < SLAB_SIZES_LEN; i++) {
        if(mem_cache_table[i].is_active == FALSE) {
            return &mem_cache_table[i];
        }
    }

    return NULL;
}


// TODO: implement mem cache release cache(for reuse)
// TODO: also this is the function that will release the mem_cache's mem chunk from memory manager


/* DEBUG */
void*
mem_cache_flist_peek_next_free_slab(const buffer_size_t buffer_size, const uint16 type_size)
{
    const slab_size_t slab_size = buffer_size * type_size;

    struct mem_cache* mcache = mem_cache_find_cache(slab_size);
    const struct slab_flist* flist_head = mcache->slabs_free_list;
    return flist_head;
}

// TEMPORARY
char_buf16_t
number_to_string(const char* str, const uint32 integer)
{
    char_buf16_t buf = {
        .buf = {'\0'},
        .len = 0,
    };

    char_buf16_copy_from_str(str, &buf);

    if(integer <= 8) {
        char_buf16_copy_from_str_append("8", &buf);
    } else if(integer <= 16) {
        char_buf16_copy_from_str_append("16", &buf);
    } else if(integer <= 32) {
        char_buf16_copy_from_str_append("32", &buf);
    } else if(integer <= 64) {
        char_buf16_copy_from_str_append("64", &buf);
    } else if(integer <= 128) {
        char_buf16_copy_from_str_append("128", &buf);
    } else if(integer <= 256) {
        char_buf16_copy_from_str_append("256", &buf);
    } else if(integer <= 512) {
        char_buf16_copy_from_str_append("512", &buf);
    } else if(integer <= 1024) {
        char_buf16_copy_from_str_append("1024", &buf);
    } else if(integer <= 2048) {
        char_buf16_copy_from_str_append("2048", &buf);
    } else {
        char_buf16_copy_from_str_append("error", &buf);
    }

    return buf;
}
