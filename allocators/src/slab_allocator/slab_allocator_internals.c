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
#include "slab_alloc_fprint_debug.h"


#define ALIGNMENT (PAGE_SIZE * 8)

/***************************************************************************************************
 * FUNCTION MACROS */
#define SLABS_FREE_LIST_GET_PAGE_COLOR_START(fnode) ((char*)fnode + sizeof(struct flist))

#define CHECK_NULLPTR(table) \
if(table == NULL) { slab_allocator_print_error(SLAB_ALLOCATOR_MEMCACHE_NO_INIT); }

/* enable file print debugging */
#define SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
#ifdef SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
#   include "slab_alloc_fprint_debug.h"
#endif


/***************************************************************************************************
 * STATIC SCOPE VARIABLES */
static salltable_t instance_count = 0;
static struct slab_allocator_instance* slab_allocator_table = NULL;

struct slab_allocator_instance*
slab_allocator_get_instance(const salltable_t inst_id)
{
    assert(slab_allocator_table != NULL);
    assert(inst_id < instance_count);
    assert(&slab_allocator_table[inst_id] != NULL);

    struct slab_allocator_instance* inst =  &slab_allocator_table[inst_id];

    return inst;
}


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
slab_allocator_create_new_instance(const pageid_t reserve_num_pages, const char* name)
{
    /* case first time stack allocator instance creation */
    if(!slab_allocator_table) {
        slab_allocator_init();
    }

    // print("%s###################################################\n"
    //       "SLAB CREATE NEW INSTANCE: %s %s\n", br1, name, CLEAR);

    /* initialize pool table */
    struct slab_allocator_pool* pool_table =
        aligned_alloc(ALIGNMENT, sizeof(struct slab_allocator_pool) * SLAB_ALLOCATOR_POOL_TABLE_COUNT);

    /* initialize mcache table */
    struct slab_allocator_mcache* mcache_table =
        aligned_alloc(ALIGNMENT, sizeof(struct slab_allocator_mcache) * SLAB_ALLOCATOR_MCACHE_TABLE_COUNT);


    const salltable_t inst_id = instance_count;
    instance_count++;

    struct slab_allocator_instance inst = {
        .instance_id = inst_id,
        .pool_table = pool_table,
        .mcache_table = mcache_table,
        .pool_count = 0,
        .mcache_count = 0,
        .reserve_num_pages = reserve_num_pages,
    };

    slab_allocator_table[inst_id] = inst;


    return inst_id;

}

uint16
slab_allocator_instance_create_new_pool(const salltable_t inst_id, const pageid_t num_pages,
    const uint16 min_cache_size, const uint16 max_cache_size, const char* name)
{
    // TODO: check null
    struct slab_allocator_instance* inst = slab_allocator_get_instance(inst_id);

    struct slab_allocator_pool pool = {
        .mem_id = memman_allocate_block(num_pages, ALIGNMENT, name),
        .mcache_count = 0,
        .min_mcache_size = min_cache_size,
        .max_mcache_size = max_cache_size,
    };

    const uint16 idx = inst->pool_count;
    inst->pool_table[idx] = pool;
    inst->pool_count++;

    return idx;
}

struct slab_allocator_mcache*
slab_allocator_create_mcache(const salltable_t inst_id, const slab_elems_t num_elems,
    const uint8 type_size, const char* name)
{
    // TODO: check null
    const slab_size_t slab_size = num_elems * type_size;

    struct slab_allocator_instance* inst = slab_allocator_get_instance(inst_id);

    int16 pool_id = pool_find_size(inst_id, slab_size);
    if(pool_id == INT_NULL) {
        pool_id = slab_allocator_instance_create_new_pool(inst_id, inst->reserve_num_pages,
            SLAB_ALLOCATOR_MCACHE_SIZE_8, SLAB_ALLOCATOR_MCACHE_SIZE_64, name);
    }

    struct slab_allocator_pool* pool = &inst->pool_table[pool_id];

    const memid_t memid = pool->mem_id;

    const uint32 bytesize = slab_size * type_size;
    assert(bytesize >= slab_size);

    const uint32 requested_num_pages = ROUND_UP(slab_size, PAGE_SIZE) / PAGE_SIZE;
    /* calculate requested size in pages */
    const uint32 slabs_capacity = (requested_num_pages * PAGE_SIZE) / slab_size;

    /* allocate memory */
    const struct memhnd_t hnd = memman_page_request(requested_num_pages, memid);
    void* ptr = memman_get_alloc_mem_by_handle(hnd);
    memset(ptr, 0x0, requested_num_pages * PAGE_SIZE);

    const uint16 mcache_id = inst->mcache_count;

    struct slab_allocator_mcache mcache = {
        .mem_handle = hnd,
        .slabs_start = ptr,
        .slab_size = slab_size,
        .slabs_capacity = slabs_capacity,
        .slabs_count = 0,
        .slabs_num_pages = requested_num_pages,
        .is_active = TRUE,
        .slabs_free_list = (struct flist*) ptr,
        .last_slab_end = ptr + (slabs_capacity * slab_size),
        .idx = mcache_id,
        .name = name,
    };

    /* build slabs free list */
    struct flist* flist = mcache.slabs_free_list;
    uint32 i = 0;
    for(; i < mcache.slabs_capacity - 1; i++) {
        ptr += slab_size;
        flist->next = (struct flist*) ptr;
        flist = flist->next;
    }


    inst->mcache_table[mcache_id] = mcache;
    inst->mcache_count++;

#   ifdef SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
        slall_fprint_mcaches(&inst->mcache_table[mcache_id], "CREATE");
#   endif


    return &inst->mcache_table[mcache_id];
}

struct slab_allocator_mcache*
mcache_extend_allocation(struct slab_allocator_mcache* const mcache, const uint32 left_over_mem)
{
    // TODO: check null
    const uint32 slab_size = mcache->slab_size;
    // assert(slab_size > left_over_mem);
    const uint32 extra_needed_mem = slab_size - left_over_mem;
    const uint32 requested_num_pages = ROUND_UP(extra_needed_mem, PAGE_SIZE) / PAGE_SIZE;
    const uint32 total_alloc_size = requested_num_pages * PAGE_SIZE;
    const uint32 slabs_capacity = ((requested_num_pages * PAGE_SIZE) + left_over_mem )/ slab_size;

    /* allocate memory */
    const struct memhnd_t hnd = memman_page_request(requested_num_pages, mcache->mem_handle.mem_id);
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

// TODO: HERE: IMPORTANT!: check alignment here OR at caller, wher the type is known
    /* check if the start of the new allocated address is in contiguity with last_slab_end */
    const char* next_paged_aligned_address = (char*) ROUND_UP((uint64)mcache->last_slab_end, PAGE_SIZE);
    const bool mem_is_contiguous = (next_paged_aligned_address == address) ? TRUE : FALSE;
    if(!mem_is_contiguous) ptr = address;

    /* build the slabs free list for the newlly allocatd memory */
    mcache->slabs_free_list = (struct flist*) ptr;
    struct flist* flist = mcache->slabs_free_list;
    while((char*) (ptr + slab_size) < (char*) (address + total_alloc_size)) {
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
    // TODO: check null
    /*
     * TODO: I can check with "mcache->slabs_capacity"
     */
    const uint64 current_used_size = mcache->slabs_count * mcache->slab_size;
    const uint64 total_allocated = mcache->slabs_num_pages * PAGE_SIZE;
    const uint64 left_over_mem = total_allocated - current_used_size;
    const bool not_enough_mem = left_over_mem < mcache->slab_size ? TRUE : FALSE;

    // if(mcache->slabs_count > mcache->slabs_capacity)
    //     assert(mcache->slabs_count > mcache->slabs_capacity);

    /* if not enough memory, extend with new slabs from memory manager */
    if(mcache->slabs_count >= mcache->slabs_capacity) {
        mcache = mcache_extend_allocation(mcache, left_over_mem);
    }


    /* get next free slab */
    struct flist* free = mcache->slabs_free_list;
    mcache->slabs_free_list = mcache->slabs_free_list->next;

    mcache->last_slab_end = (char*) free + mcache->slab_size;


    /* update mcache */
    mcache->slabs_count++;
    // if(mcache->slabs_count > mcache->slabs_capacity)
    // assert(mcache->slabs_count > mcache->slabs_capacity);


#   ifdef SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
    slall_fprint_mcaches(mcache, "ALLOCATE");
#   endif

    return free;
}

/*SAVE*/
// struct flist*
// mcache_alloc_slab(struct slab_allocator_mcache* mcache)
// {
//     // TODO: check null
//     /*
//      * TODO: I can check with "mcache->slabs_capacity"
//      */
//     const uint64 current_used_size = mcache->slabs_count * mcache->slab_size;
//     const uint64 total_allocated = mcache->slabs_num_pages * PAGE_SIZE;
//     const uint64 left_over_mem = total_allocated - current_used_size;
//     const bool not_enough_mem = left_over_mem < mcache->slab_size ? TRUE : FALSE;
//
//     if(mcache->slabs_count > mcache->slabs_capacity)
//         assert(mcache->slabs_count > mcache->slabs_capacity);
//
//     /* if not enough memory, extend with new slabs from memory manager */
//     if(mcache->slabs_free_list == NULL || not_enough_mem) {
//         mcache = mcache_extend_allocation(mcache, left_over_mem);
//     }
//
//
//     /* get next free slab */
//     struct flist* free = mcache->slabs_free_list;
//     mcache->slabs_free_list = mcache->slabs_free_list->next;
//
//     mcache->last_slab_end = (char*) free + mcache->slab_size;
//
//
//     /* update mcache */
//     mcache->slabs_count++;
//     if(mcache->slabs_count > mcache->slabs_capacity)
//     assert(mcache->slabs_count > mcache->slabs_capacity);
//
//
// #   ifdef SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
//     slall_fprint_mcaches(mcache, "ALLOCATE");
// #   endif
//
//     return free;
// }

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
    mcache->slabs_count--;

#   ifdef SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
    slall_fprint_mcaches(mcache, "FREE SLAB");
#   endif

    return TRUE;
}

void
mcache_table_destroy(void)
{
    print_implement("mcache_table_destroy()", __FILE__, __LINE__);
    // uint32 i = 0;
    // for(; i < SLAB_ALLOCATOR_MCACHE_SIZES_COUNT; i++) {
    //     if(mcache_table[i].is_active) {
    //         memman_page_release(mcache_table[i].mem_handle);
    //     }
    // }
    //
    // if(mcache_table) {
    //     print("FREEING string_mem_cache_table: %x ... ", ry, mcache_table, CLEAR);
    //     free(mcache_table);
    //     print("OK", GREEN, CLEAR);
    // }
}

/***************************************************************************************************
 * HELPER FUNCTIONS */
int16
pool_find_size(const salltable_t inst_id, const slab_size_t slab_size)
{
    struct slab_allocator_instance* inst = slab_allocator_get_instance(inst_id);

    for(uint32 i = 0; i < inst->pool_count; i++) {
        const struct slab_allocator_pool* pool = &inst->pool_table[i];
        if(pool[i].max_mcache_size <= slab_size && pool[i].min_mcache_size >= slab_size) {
            return i;
        }
    }

    return INT_NULL;
}

struct slab_allocator_mcache*
mcache_find_size(const salltable_t inst_id, const slab_size_t slab_size)
{
    struct slab_allocator_instance* inst = slab_allocator_get_instance(inst_id);

    for(uint32 i = 0; i < inst->mcache_count; i++) {
        struct slab_allocator_mcache* mcache = &inst->mcache_table[i];
        if(mcache->slab_size == slab_size) {
            return mcache;
        }
    }

    return NULL;
}


/* DEBUG */
void*
mcache_flist_peek_next_free_slab(const salltable_t inst_id, const slab_elems_t num_elems, const uint8 type_size)
{
    const slab_size_t slab_size = num_elems * type_size;

    const struct slab_allocator_mcache* mcache = mcache_find_size(inst_id, slab_size);
    struct flist* flist_head = mcache->slabs_free_list;
    return flist_head;
}






/***************************************************************************************************
 * TEST-DEBUG
 **************************************************************************************************/

struct slab_allocator_instance*
debug_get_slab_allocator_table()
{return slab_allocator_table;}

struct slab_allocator_mcache*
debug_mcache_free_slab(struct slab_allocator_mcache* mcache, void* slab_address)
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
    mcache->slabs_count--;

#   ifdef SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
    slall_fprint_mcaches(mcache, "FREE SLAB");
#   endif

    return slab_address;
}