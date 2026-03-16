/*
 * Created by ckalo on 1/22/26.
 *
 * todo:
 */


#include "slab_allocator_internals.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include "file_sys_interface.h"
#include "my_macros.h"
#include "mem_man_internals.h"
#include "slab_allocator_log.h"
#include "slab_allocator_struct.h"


/***************************************************************************************************
 * FUNCTION MACROS */
#define SLABS_FREE_LIST_GET_PAGE_COLOR_START(fnode) ((char*)fnode + sizeof(struct flist))

#define CHECK_NULLPTR(table) \
if(table == NULL) { slab_allocator_print_error(SLAB_ALLOCATOR_MEMCACHE_NO_INIT); }

/* enable file print debugging */
// #define SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
#ifdef SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
#   include "slab_alloc_fprint_debug.h"
#endif


/***************************************************************************************************
 * STATIC SCOPE VARIABLES */
static salltable_t instance_count = 0;
static struct slab_allocator_instance* slab_allocator_table = NULL;
static struct slab_allocator_pool* pool_table = NULL;
static struct slab_allocator_mcache* mcache_table = NULL;
HERE: slould implement mcaches per instance, same with pools


void
slab_allocator_init(void)
{
    if(!slab_allocator_table)
        slab_allocator_table = aligned_alloc(1024, sizeof(struct slab_allocator_instance) * SLAB_ALLOCATOR_INSTANCE_INITIAL_COUNT);

#   ifdef SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
    sslall_fprint_init();
#   endif

}

const char* pool_names[SLAB_ALLOCATOR_POOL_SIZES_COUNT] = {
    "size small", "size medium", "size large",
};


memid_t
slab_allocator_create_new_instance(const pageid_t num_pages, const char* name)
{
    /* case first time stack allocator instance creation */
    if(!slab_allocator_table) {
        slab_allocator_init();
    }

    print("%s###################################################\n"
          "SLAB CREATE NEW INSTANCE: %s\n", RED, name, CLEAR);

    struct slab_allocator_instance instance = {
        .instance_id = -1, // MAX_UINT for ERROR
        .size = 0,
        .count = 0,
    };

    const salltable_t idx = instance_count;
    instance_count++;
    slab_allocator_table[idx] = instance;

    if(!pool_table)
        pool_table = aligned_alloc(1024, sizeof(struct slab_allocator_pool) * SLAB_ALLOCATOR_POOLS_COUNT);

    /* create all sized pools for allocator */
    uint32 i = 0;
    for(; i < SLAB_ALLOCATOR_POOL_SIZES_COUNT; i++) {
        char* start = (char*) pool_table + sizeof(struct slab_allocator_pool) * i;
        struct slab_allocator_pool pool = {
            .name = i, // indexes the pool_names
            .start = start,
            .num_pages = SLAB_ALLOCATOR_POOL_NUM_PAGES,
            .size = SLAB_ALLOCATOR_POOL_NUM_PAGES * PAGE_SIZE,
            .mem_id = memman_alloc_create(num_pages, PAGE_SIZE*16),
        };

        pool_table[i] = pool;
        slab_allocator_table[idx].count++;

        slab_allocator_table[idx].instance_id = pool.mem_id;
        pool_table[i] = pool;


        const uint32 min_num_caches = sizeof(struct slab_allocator_mcache) * SLAB_ALLOCATOR_MCACHE_SIZES_COUNT;
        const uint32 table_size = ROUND_UP(min_num_caches, PAGE_SIZE);
        mcache_table = aligned_alloc(8, table_size);
        // TODO:HERE: mcache_table should be unique for each instance

        /* allocate mcache's table element memory storage */
        uint32 j = 0, type_sizes = sizeof(char*);
        for(; j < SLAB_ALLOCATOR_MCACHE_SIZES_COUNT; j++) {
            const struct slab_allocator_mcache mem_cache = (struct slab_allocator_mcache){
                .is_active = FALSE,
            };
            mcache_table[j] = mem_cache;

            type_sizes *= 2;
        }

    }

    return idx;

}

void
slab_allocator_create_new_pool(const pageid_t num_pages)
{ // TODO: UNUSED, IMPLEMENT
    if(!pool_table)
        pool_table = aligned_alloc(1024, sizeof(struct slab_allocator_pool) * SLAB_ALLOCATOR_POOLS_COUNT);

    const salltable_t idx = instance_count;
    instance_count++;

    struct slab_allocator_pool pool = {
        .name = -1,
        .start = NULL,
        .num_pages = num_pages,
        .size = num_pages * PAGE_SIZE,
        .mem_id = 0,
    };
}

struct slab_allocator_mcache*
slab_allocator_create_mcache(const slab_elems_t num_elems, const uint8 type_size)
{
    CHECK_NULLPTR(mcache_table);

    const slab_size_t slab_size = num_elems * type_size;
    const memid_t memid = pool_find_size(slab_size);

    const uint32 bytesize = slab_size * type_size;
    assert(bytesize >= slab_size);

    const uint32 requested_num_pages = ROUND_UP(slab_size, PAGE_SIZE) / PAGE_SIZE;
    /* calculate requested size in pages */
    const uint32 slabs_capacity = (requested_num_pages * PAGE_SIZE) / slab_size;

    /* allocate memory */
    struct slab_allocator_mcache* mcache = mcache_get_next_free();

    const struct memhnd_t hnd = memman_alloc_get(requested_num_pages, memid);
    void* ptr = memman_get_alloc_mem_by_handle(hnd);
    memset(ptr, 0x0, requested_num_pages * PAGE_SIZE);

    /* updated mcache */
    mcache->mem_handle = hnd;
    mcache->slabs_start = ptr;
    mcache->slab_size = slab_size;
    mcache->slabs_capacity = slabs_capacity;
    mcache->slabs_num_pages = requested_num_pages;
    mcache->is_active = TRUE;
    mcache->slabs_free_list = (struct flist*) ptr;
    mcache->last_slab_end = ptr + (slabs_capacity * slab_size);



    /* build slabs free list */
    struct flist* flist = mcache->slabs_free_list;
    uint32 i = 0;
    for(; i < mcache->slabs_capacity - 1; i++) {
        ptr += slab_size;
        flist->next = (struct flist*) ptr;
        flist = flist->next;
    }

#   ifdef SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
        slall_fprint_mcaches(mcache, "CREATE");
#   endif


    return mcache;
}

struct slab_allocator_mcache*
mcache_extend_allocation(struct slab_allocator_mcache* const mcache, const uint32 left_over_mem)
{
    CHECK_NULLPTR(mcache_table);

    const uint32 slab_size = mcache->slab_size;
    const uint32 extra_needed_mem = slab_size - left_over_mem;
    const uint32 requested_num_pages = ROUND_UP(extra_needed_mem, PAGE_SIZE) / PAGE_SIZE;
    const uint32 total_alloc_size = requested_num_pages * PAGE_SIZE;
    const uint32 slabs_capacity = (requested_num_pages * PAGE_SIZE) / slab_size;

    /* allocate memory */
    const struct memhnd_t hnd = memman_alloc_get(requested_num_pages, mcache->mem_handle.mem_id);
    void* address = memman_get_alloc_mem_by_handle(hnd);
    memset(address, 0x0, requested_num_pages);
    /* update only num_pages field, mem_is and page_id
     * should be constant for the life duration of the mcache */
    mcache->mem_handle.num_pages = hnd.num_pages;

    /* update mcache */
    mcache->slabs_capacity += slabs_capacity;
    mcache->slabs_num_pages += requested_num_pages;
    mcache->is_active = TRUE;

    /* continue from the end of the last allocated slab address */
    char* ptr = mcache->last_slab_end;


    /* check if the start of the new allocated address is in contiguity with last_slab_end */
    const char* next_paged_aligned_address = (char*) ROUND_UP((uint64)mcache->last_slab_end, PAGE_SIZE);
    const bool mem_is_contiguous = (next_paged_aligned_address == address) ? TRUE : FALSE;
    if(!mem_is_contiguous) ptr = address;

    /* build the slabs free list for the newlly allocatd memory */
    mcache->slabs_free_list = (struct flist*) ptr;
    struct flist* flist = mcache->slabs_free_list;
    while((uint64*) (ptr + slab_size) < (uint64*) (address + total_alloc_size)) {
        ptr += slab_size;
        flist->next = (struct flist*) ptr;
        flist = flist->next;
    }

    /* avoid list getting an address pointer from the memory manager's free list */
    /* MAYBE BUG: If we null the memman freelist then we have a bug */
    flist->next = NULL;


#   ifdef SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
    slall_fprint_mcaches(mcache, "EXTEND");
#   endif

    return mcache;
}

struct flist*
mcache_alloc_slab(struct slab_allocator_mcache* mcache)
{
    /*
     * TODO: I can check with "mcache->slabs_capacity"
     */
    const uint64 current_used_size = mcache->slabs_used * mcache->slab_size;
    const uint64 total_allocated = mcache->slabs_num_pages * PAGE_SIZE;
    const uint64 left_over_mem = total_allocated - current_used_size;
    const bool not_enough_mem = left_over_mem < mcache->slab_size ? TRUE : FALSE;

    /* if not enough memory, extend with new slabs from memory manager */
    if(mcache->slabs_free_list == NULL || not_enough_mem) {
        mcache = mcache_extend_allocation(mcache, left_over_mem);
    }


    /* get next free slab */
    struct flist* free = mcache->slabs_free_list;
    mcache->slabs_free_list = mcache->slabs_free_list->next;

    mcache->last_slab_end = (char*) free + mcache->slab_size;


    /* update mcache */
    mcache->slabs_used++;
    mcache->slabs_count++;


#   ifdef SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
    slall_fprint_mcaches(mcache, "ALLOCATE");
#   endif

    return free;
}

bool
mcache_free_slab(struct slab_allocator_mcache* mcache, void* slab_address)
{
    struct flist* released = (struct flist*) slab_address;
    struct flist* node = mcache->slabs_free_list;

    memset(slab_address, 0x0, mcache->slab_size);

    if(node == NULL) {
        /* insert at head if free list is NULL */
        mcache->slabs_free_list = released;
        mcache->slabs_free_list->next = NULL;
    } else if(node > released) {
        /* insert at head and link to the next free */
        mcache->slabs_free_list = released;
        mcache->slabs_free_list->next = node;
    } else if(node->next != NULL) {
        /* insert in between and link next and previous free */
        while(node->next != NULL && node->next < released)
            node = node->next;
        struct flist* temp = node->next;
        node->next = released;
        node->next->next = temp;
    } else {
        /* insert at the end */
        node->next = (struct flist*) slab_address;
        node->next->next = NULL;
    }

    /* update mcache */
    mcache->slabs_used--;
    mcache->slabs_count--;

#   ifdef SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
    slall_fprint_mcaches(mcache, "FREE SLAB");
#   endif


    return TRUE;
}

void
mcache_table_destroy(void)
{
    uint32 i = 0;
    for(; i < SLAB_ALLOCATOR_MCACHE_SIZES_COUNT; i++) {
        if(mcache_table[i].is_active) {
            memman_alloc_release(mcache_table[i].mem_handle);
        }
    }

    if(mcache_table) {
        print("FREEING string_mem_cache_table: %x ... ", ry, mcache_table, CLEAR);
        free(mcache_table);
        print("OK", GREEN, CLEAR);
    }
}

/***************************************************************************************************
 * HELPER FUNCTIONS */
memid_t
pool_find_size(const slab_size_t slab_size)
{
    memid_t memid = -1; // MAX_UINT for ERROR
    for(uint32 i = 0; i < SLAB_ALLOCATOR_POOL_SIZES_COUNT; i++) {
        if(slab_size < SLAB_ALLOCATOR_POOL_SIZE_SMALL) {
            memid = pool_table[i].mem_id;
            break;
        } else if(slab_size < SLAB_ALLOCATOR_POOL_SIZE_MEDIUM) {
            memid = pool_table[i].mem_id;
            break;
        } else if(slab_size < SLAB_ALLOCATOR_POOL_SIZE_LARGE) {
            memid = pool_table[i].mem_id;
            break;
        } else
            assert(1);
    }

    return memid;
}

struct slab_allocator_mcache*
mcache_find_size(const slab_size_t slab_size)
{
    CHECK_NULLPTR(mcache_table);
    for(uint32 i = 0; i < SLAB_ALLOCATOR_MCACHE_SIZES_COUNT; i++) {
        if(mcache_table[i].slab_size == slab_size) {
            return &mcache_table[i];
        }
    }

    return NULL;
}

struct slab_allocator_mcache*
mcache_get_next_free(void)
{
    for(uint32 i = 0; i < SLAB_ALLOCATOR_MCACHE_SIZES_COUNT; i++) {
        if(mcache_table[i].is_active == FALSE) {
            mcache_table[i].idx = i;
            return &mcache_table[i];
        }
    }

    return NULL;
}


/* DEBUG */
void*
mcache_flist_peek_next_free_slab(const slab_elems_t num_elems, const uint8 type_size)
{
    const slab_size_t slab_size = num_elems * type_size;

    const struct slab_allocator_mcache* mcache = mcache_find_size(slab_size);
    struct flist* flist_head = mcache->slabs_free_list;
    return flist_head;
}



/*
 * these are a copies of the function(s) of slab_allocator, for testing purposes
 * MUST BE KEPT UPDATED TO THE ACTUAL SLAB ALLOCATOR'S FUNCTIONS
 */
char*
mcache_free_slab_test_debug(struct slab_allocator_mcache* mcache, void* slab_address)
{
    const char* free_buffer = slab_address;

    struct flist* released = (struct flist*) slab_address;
    struct flist* node = mcache->slabs_free_list;

    memset(slab_address, 0x0, mcache->slab_size);

    if(node == NULL) {
        /* insert at head if free list is NULL */
        mcache->slabs_free_list = released;
        mcache->slabs_free_list->next = NULL;
    } else if(node > released) {
        /* insert at head and link to the next free */
        mcache->slabs_free_list = released;
        mcache->slabs_free_list->next = node;
    } else if(node->next != NULL) {
        /* insert in between and link next and previous free */
        while(node->next != NULL && node->next < released)
            node = node->next;
        struct flist* temp = node->next;
        node->next = released;
        node->next->next = temp;
    } else {
        /* insert at the end */
        node->next = (struct flist*) slab_address;
        node->next->next = NULL;
    }

    /* update mcache */
    mcache->slabs_used--;
    mcache->slabs_count--;

    return free_buffer;

}

char*
slab_allocator_release_test_debug(void* ptr, const slab_size_t slab_size)
{
    struct slab_allocator_mcache* mcache = mcache_find_size(slab_size);

    if(!mcache) // handle error: cache not found
        slab_allocator_print_error(SLAB_ALLOCATOR_MEMCACHE_NOT_FOUND);

    return mcache_free_slab_test_debug(mcache, ptr);
}