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
#include "slab_allocator_log.h"
#include "my_macros.h"
#include "mem_man_internals.h"
#include "str_utils.h"

#define MIN_SLABS_TABLE_LEN 128

/***************************************************************************************************
 * FUNCTION MACROS */
#define SLABS_FREE_LIST_GET_PAGE_COLOR_START(fnode) ((char*)fnode + sizeof(struct slab_flist))

#define SLABALLOC_CHECK_MCACHE_TABLE(table) \
if(table == NULL) { slab_allocator_print_error(SLAB_ALLOC_MEMCACHE_NO_INIT); }


/***************************************************************************************************
 * STATIC SCOPE VARIABLES */
static struct mem_cache* mem_cache_table = NULL;


/***************************************************************************************************
 * FOR DEBUG */
static struct fs_t* fs_flist = NULL;
static struct fs_t* fs_mcaches[SLAB_SIZES_LEN] = {NULL};

#define DEBUG_FPRINT_MCACHE 1

#ifdef DEBUG_FPRINT_MCACHE
#define fs_register_mcache_create(fs, mcache)\
    fs_append(fs,\
    "CREATE  : | "          \
    " name: %s | "          \
    " pages: %03u | "       \
    "capac: %05u | "        \
    "count: %03u | "        \
    "used: %03u | "         \
    "flist: %08x %08x\n",   \
    mcache->name.buf,\
    mcache->slabs_num_pages,\
    mcache->slabs_capacity, \
    mcache->slabs_count,    \
    mcache->slabs_used,     \
    (uint64)mcache->slabs_free_list >> 32,(uint64)mcache->slabs_free_list & 0xFFFFFFFF\
    )

#define fs_register_mcache_alloc_slab(fs, mcache)\
    fs_append(fs,\
    "ALLOCATE  : | "        \
    " pages: %03u | "       \
    "capac: %05u | "        \
    "count: %03u | "        \
    "used: %03u | "         \
    "flist: %08x %08x | "   \
    "free: %08x %08x | "    \
    "dif: %04llu bytes\n",  \
    mcache->slabs_num_pages,\
    mcache->slabs_capacity, \
    mcache->slabs_count,    \
    mcache->slabs_used,     \
    (uint64)mcache->slabs_free_list >> 32,(uint64)mcache->slabs_free_list & 0xFFFFFFFF,\
    (uint64)free >> 32,(uint64)free & 0xFFFFFFFF,\
    (uint64*)mcache->slabs_free_list - (uint64*)free\
    )

#define fs_register_mcache_extend_alloc(fs, mcache, address)\
    fs_append(fs,\
    "EXTEND    : | "        \
    " pages: %03u | "       \
    "capac: %05u | "        \
    "count: %03u | "        \
    "used: %03u | "         \
    "flist: %08x %08x | "   \
    "addr: %08x %08x | "    \
    "dif: %04llu bytes\n",  \
    mcache->slabs_num_pages,\
    mcache->slabs_capacity, \
    mcache->slabs_count,    \
    mcache->slabs_used,     \
    (uint64)mcache->slabs_free_list >> 32,(uint64)mcache->slabs_free_list & 0xFFFFFFFF,\
    (uint64)address >> 32,(uint64)address & 0xFFFFFFFF,\
    (uint64*)address - (uint64*)mcache->slabs_free_list\
    )

#define fs_register_mcache_release_slab(fs, mcache)\
    fs_append(fs,\
    "RELEASE   : | "        \
    " pages: %03u | "       \
    "capac: %05u | "        \
    "count: %03u | "        \
    "used: %03u | "         \
    "flist: %08x %08x | "   \
    "free: %08x %08x | "    \
    "dif: %04llu bytes\n",  \
    mcache->slabs_num_pages,\
    mcache->slabs_capacity, \
    mcache->slabs_count,    \
    mcache->slabs_used,     \
    (uint64)mcache->slabs_free_list >> 32,(uint64)mcache->slabs_free_list & 0xFFFFFFFF,\
    (uint64)free >> 32,(uint64)free & 0xFFFFFFFF,\
    (uint64*)mcache->slabs_free_list - (uint64*)free\
    )

#define fs_register_mcache_append(fs, fmt, ...)\
    fs_append(fs, fmt, ##__VA_ARGS__)

#else

#endif
/* FOR DEBUG
 **************************************************************************************************/


void
mem_cache_table_init(void)
{
    const uint32 min_num_caches = sizeof(struct mem_cache) * SLAB_SIZES_LEN;
    const uint32 table_size = ROUND_UP(min_num_caches, PAGE_SIZE);
    mem_cache_table = aligned_alloc(1024, table_size);

    /* pre-create mcaches, starting from 8 byte slabs
     * continuing with power of 2's */
    uint32 i = 0, type_sizes = sizeof(char*);
    for(; i < SLAB_SIZES_LEN; i++) {
        const struct mem_cache mem_cache = (struct mem_cache){
            .type_size = type_sizes,
            .is_active = FALSE,
        };
        mem_cache_table[i] = mem_cache;

        type_sizes *= 2;
    }

    /* DEBUG-FPRINT */
    fs_flist = fs_create("allocators/slab_allocator_flist.txt");
}

bool
mem_cache_does_address_belong_to_mcache(void* address)
{
    uint32 i = 0;
    for(; i < SLAB_SIZES_LEN; i++) {
        void* start = mem_cache_table[i].slabs_start;
        void* end = mem_cache_table[i].slabs_start +
            (mem_cache_table[i].slabs_capacity*mem_cache_table[i].slab_size);
        if(address == start || (address > start && address <= end)) {
            return TRUE;
        }
    }
    return FALSE;
}

struct mem_cache*
mem_cache_create(const buffer_size_t buffer_size, const uint16 type_size, const char* name)
{
    SLABALLOC_CHECK_MCACHE_TABLE(mem_cache_table);

    const uint32 slab_size = buffer_size * type_size;
    assert(slab_size >= buffer_size);

    /* calculate requested size in pages */
    const uint32 requested_num_pages = (slab_size <= PAGE_SIZE) ? 1 : slab_size / PAGE_SIZE;
    const uint32 slabs_capacity = (requested_num_pages * PAGE_SIZE) / slab_size;

    /* allocate memory */
    struct mem_cache* mcache = mem_cache_get_next_free_cache();
    const struct memhnd_t hnd = memman_alloc_pages(requested_num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
    char* ptr = (char*) memman_get_alloc_mem_by_handle(hnd);
    memset(ptr, 0x0, requested_num_pages * PAGE_SIZE);

    /** create mcache name */
    char str_slab_size[INT32_TO_STR_BUFFER_SIZE] = {0};
    str_conv_int32_to_str(str_slab_size, slab_size);
    char_buf16_copy_from_str("mcache_", &mcache->name);
    str_concat_memcpy(mcache->name.buf, str_slab_size, 16);

    /* updated mcache */
    mcache->mem_handle = hnd;
    mcache->slabs_start = ptr;
    mcache->type_size = type_size;
    mcache->slab_size = slab_size;
    mcache->slabs_capacity = slabs_capacity;
    mcache->slabs_num_pages = requested_num_pages;
    mcache->is_active = TRUE;
    mcache->slabs_free_list = (struct slab_flist*) ptr;

    /* DEBUG-FPRINT */
    {
        char filename[64] = "allocators/mcache_";
        str_concat_memcpy(filename, str_slab_size, 64);
        str_concat_memcpy(filename, "byte.txt", 64);
        fs_mcaches[mcache->idx] = fs_create(filename);
        fs_register_mcache_create(fs_mcaches[mcache->idx], mcache);
    }


    /* build slabs free list */
    struct slab_flist* flist = mcache->slabs_free_list;
    uint32 i = 0;
    for(; i < mcache->slabs_capacity - 1; i++) {
        ptr += slab_size;
        flist->next = (struct slab_flist*) ptr;
        flist = flist->next;
    }

    print_mem_cache_create(mcache);

    return mcache;
}

struct mem_cache*
mem_cache_extend_allocation(struct mem_cache* const mcache, const uint32 left_over_mem)
{
    SLABALLOC_CHECK_MCACHE_TABLE(mem_cache_table);

    const uint32 slab_size = mcache->slab_size;
    const uint32 extra_needed_mem = slab_size - left_over_mem;
    const uint32 requested_num_pages = ROUND_UP(extra_needed_mem, PAGE_SIZE) / PAGE_SIZE;
    const uint32 total_alloc_size = requested_num_pages * PAGE_SIZE;
    const uint32 slabs_capacity = (requested_num_pages * PAGE_SIZE) / slab_size;

    /* allocate memory */
    const struct memhnd_t hnd = memman_alloc_pages(requested_num_pages, MEM_CHUNK_ALLOCATOR_SLAB);
    void* address = memman_get_alloc_mem_by_handle(hnd);
    memset(address, 0x0, requested_num_pages*PAGE_SIZE);

    /* update mcache */
    mcache->slabs_capacity += slabs_capacity;
    mcache->slabs_num_pages += requested_num_pages;
    mcache->is_active = TRUE;

    /* continue from the end of the last allocated slab address */
    char* ptr = mcache->slabs_start + (slab_size * mcache->slabs_count);

    bool is_occupied = mem_cache_does_address_belong_to_mcache(ptr);
    if(is_occupied) ptr = address;

    /* build the slabs free list for the newlly allocatd memory */
    if(mcache->slabs_free_list == NULL) mcache->slabs_free_list = (struct slab_flist*) ptr;
    struct slab_flist* flist = mcache->slabs_free_list;
    while((uint64*) (ptr + slab_size) < (uint64*) (address + total_alloc_size)) {
        ptr += slab_size;
        flist->next = (struct slab_flist*) ptr;
        flist = flist->next;
    }

    /* avoid list getting an address pointer from the memory manager free list*/
    flist->next = NULL;

    /* DEBUG-FPRINT */
    fs_register_mcache_extend_alloc(fs_mcaches[mcache->idx], mcache, address);

    return mcache;
}

struct slab_flist*
mem_cache_alloc_slab(struct mem_cache* mcache)
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
        mcache = mem_cache_extend_allocation(mcache, left_over_mem);
    }

    /* get next free slab */
    struct slab_flist* free = mcache->slabs_free_list;
    mcache->slabs_free_list = mcache->slabs_free_list->next;

    /* update mcache */
    mcache->slabs_used++;
    mcache->slabs_count++;

    /* DEBUG-FPRINT */
    fs_register_mcache_alloc_slab(fs_mcaches[mcache->idx], mcache);

    return free;
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

    if(mem_cache_table) {
        print("FREEING mem_cache_table: %x ... ", ry, mem_cache_table, CLEAR);
        free(mem_cache_table);
        print("OK", GREEN, CLEAR);
    }

    // todo: slab allocator logger print table destroy
}

bool
mem_cache_free_slab(struct mem_cache* mcache, void* slab_address)
{
    // TODO: write test
    struct slab_flist* released = (struct slab_flist*) slab_address;
    struct slab_flist* node = mcache->slabs_free_list;

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
        struct slab_flist* temp = node->next;
        node->next = released;
        node->next->next = temp;
    } else {
        /* insert at the end */
        node->next = (struct slab_flist*) slab_address;
        node->next->next = NULL;
    }

    /* update mcache */
    mcache->slabs_used--;
    mcache->slabs_count--;

    /* DEBUG-FPRINT */
    fs_register_mcache_release_slab(fs_mcaches[mcache->idx], mcache);

    return TRUE;
}

/***************************************************************************************************
 * HELPER FUNCTIONS */
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

struct mem_cache*
mem_cache_get_next_free_cache(void)
{
    uint32 i = 0;
    for(; i < SLAB_SIZES_LEN; i++) {
        if(mem_cache_table[i].is_active == FALSE) {
            mem_cache_table[i].idx = i;
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
    struct slab_flist* flist_head = mcache->slabs_free_list;
    return flist_head;
}

