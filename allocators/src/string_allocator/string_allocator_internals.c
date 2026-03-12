/*
* Created by ckalo on 8/3/26.
 */

#include "string_allocator_internals.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include "file_sys_interface.h"
#include "string_allocator_log.h"
#include "my_macros.h"
#include "mem_man_internals.h"
#include "str_utils.h"


/***************************************************************************************************
 * FUNCTION MACROS */
#define SLABS_FREE_LIST_GET_PAGE_COLOR_START(fnode) ((char*)fnode + sizeof(struct string_slab_flist))

#define STRING_ALLOC_CHECK_MCACHE_TABLE(table) \
if(table == NULL) { string_allocator_print_error(STRING_ALLOC_MEMCACHE_NO_INIT); }


/***************************************************************************************************
 * STATIC SCOPE VARIABLES */
static struct string_slab_allocator_instance* string_slab_allocator_table = NULL;
static uint16 instance_count = 0;
static struct string_mem_cache* string_mcache_table = NULL;


/***************************************************************************************************
 * FOR DEBUG */
static struct fs_t* fs_flist = NULL;
/* output a debug file per mcache  */
static struct fs_t* fs_mcaches[STRING_SLAB_SIZES_LEN] = {NULL};
/* output one debug file and register all created mcaches  */
static struct fs_t* fs_register_created_mcaches = NULL;
/* FOR DEBUG
 **************************************************************************************************/


bool
is_string_slab_allocator_init()
{
    if(string_slab_allocator_table == NULL) return FALSE;
    if(string_mcache_table == NULL) return FALSE;
    return TRUE;
}

const char* names[STRING_SLAB_ALLOCATOR_INSTANCE_SIZES] = {
    "size small", "size medium", "size large",
};
void
string_mem_cache_table_init(void)
{
    string_slab_allocator_table =
            aligned_alloc(1024, sizeof(struct string_slab_allocator_instance) * STRING_SLAB_ALLOCATOR_INSTANCE_SIZES);

    const uint16 num_pages[STRING_SLAB_ALLOCATOR_INSTANCE_SIZES] = {32, 32, 32};
    for(uint32 i = 0; i < STRING_SLAB_ALLOCATOR_INSTANCE_SIZES; i++) {
        const memid_t mem_id = mem_create(num_pages[i]);
        const struct string_slab_allocator_instance instance = {
            // .name = names[i],
            // .name = {0},
            .start = NULL,
            .num_pages = num_pages[i],
            .size = num_pages[i] * PAGE_SIZE,
            .mem_id = mem_id,
        };

        // char_buf16_copy_from_str(names[i], instance.name);

        string_slab_allocator_table[i] = instance;
        instance_count++;
    }

    const uint32 min_num_caches = sizeof(struct string_mem_cache) * STRING_SLAB_SIZES_LEN;
    const uint32 table_size = ROUND_UP(min_num_caches, PAGE_SIZE);
    string_mcache_table = aligned_alloc(1024, table_size);

    /* pre-create mcaches, starting from 8 byte slabs
     * continuing with power of 2's */
    uint32 i = 0, type_sizes = sizeof(char*);
    for(; i < STRING_SLAB_SIZES_LEN; i++) {

        const struct string_mem_cache mem_cache = (struct string_mem_cache){
            .is_active = FALSE,
        };
        string_mcache_table[i] = mem_cache;

        type_sizes *= 2;
    }


    /* DEBUG-FPRINT */
    fs_flist = fs_create("allocators/string/string_flist.txt");
    fs_register_created_mcaches = fs_create("allocators/string/register_created_string_mcaches.txt");
    fs_append(fs_register_created_mcaches, "OPERATIONS: "
              "Create-Allocate-Release operations "
              "of STRING slab allocator mcaches\n"
              "________________________________________________________"
              "________________________________________________________"
              "________________________________________________________\n"
    );
}

memid_t
string_slab_allocator_create_new_instance(const pageid_t num_pages)
{
    struct string_slab_allocator_instance instance = {
        .name = NULL,
        .start = NULL,
        .num_pages = num_pages,
        .size = num_pages * PAGE_SIZE,
        .mem_id = 0,
    };
    char_buf16_copy_from_str("rename me", instance.name);

    string_slab_allocator_table[instance_count++] = instance;

    instance.mem_id = mem_create(instance.num_pages);

    return instance.mem_id;
}

void
string_mem_cache_table_destroy(void)
{
    uint32 i = 0;
    for(; i < STRING_SLAB_SIZES_LEN; i++) {
        if(string_mcache_table[i].is_active) {
            memman_release(string_mcache_table[i].mem_handle);
        }
    }

    if(string_mcache_table) {
        print("FREEING string_mem_cache_table: %x ... ", ry, string_mcache_table, CLEAR);
        free(string_mcache_table);
        print("OK", GREEN, CLEAR);
    }

}

struct string_mem_cache*
string_mem_cache_create(const string_size_t string_size)
{
    STRING_ALLOC_CHECK_MCACHE_TABLE(string_mcache_table);

    const memid_t memid = string_slab_alloc_instance_find_size(string_size);

    const uint32 slab_size = string_size;
    assert(slab_size >= string_size);

    const uint32 requested_num_pages = ROUND_UP(slab_size, PAGE_SIZE) / PAGE_SIZE;
    /* calculate requested size in pages */
    const uint32 slabs_capacity = (requested_num_pages * PAGE_SIZE) / slab_size;

    /* allocate memory */
    struct string_mem_cache* mcache = string_mem_cache_get_next_free_cache();

    const struct memhnd_t hnd = memman_alloc_pages(requested_num_pages, memid);
    void* ptr = memman_get_alloc_mem_by_handle(hnd);
    memset(ptr, 0x0, requested_num_pages * PAGE_SIZE);

    /** create mcache name */
    const char str_slab_size[INT32_TO_STR_BUFFER_SIZE] = {0};
    // TODO: add name to mcache

    /* updated mcache */
    mcache->mem_handle = hnd;
    mcache->slabs_start = ptr;
    mcache->slab_size = slab_size;
    mcache->slabs_capacity = slabs_capacity;
    mcache->slabs_num_pages = requested_num_pages;
    mcache->is_active = TRUE;
    mcache->slabs_free_list = (struct string_slab_flist*) ptr;
    mcache->last_slab_end = ptr + (slabs_capacity * slab_size);

    /* DEBUG-FPRINT */
    {
#ifdef DEBUG_FPRINT_MCACHE
        char filename[64] = "allocators/string/mcache_";
        str_concat_memcpy(filename, str_slab_size, 64);
        str_concat_memcpy(filename, "byte.txt", 64);
        fs_mcaches[mcache->idx] = fs_create(filename);
        fs_register_mcache_create(fs_mcaches[mcache->idx], mcache);
        fs_register_mcache_create(fs_register_created_mcaches, mcache);
#endif
    }


    /* build slabs free list */
    struct string_slab_flist* flist = mcache->slabs_free_list;
    uint32 i = 0;
    for(; i < mcache->slabs_capacity - 1; i++) {
        ptr += slab_size;
        flist->next = (struct string_slab_flist*) ptr;
        flist = flist->next;
    }

    return mcache;
}

struct string_mem_cache*
string_mem_cache_extend_allocation(struct string_mem_cache* const mcache, const uint32 left_over_mem)
{
    STRING_ALLOC_CHECK_MCACHE_TABLE(string_mcache_table);

    const uint32 slab_size = mcache->slab_size;
    const uint32 extra_needed_mem = slab_size - left_over_mem;
    const uint32 requested_num_pages = ROUND_UP(extra_needed_mem, PAGE_SIZE) / PAGE_SIZE;
    const uint32 total_alloc_size = requested_num_pages * PAGE_SIZE;
    const uint32 slabs_capacity = (requested_num_pages * PAGE_SIZE) / slab_size;

    /* allocate memory */
    const struct memhnd_t hnd = memman_alloc_pages(requested_num_pages, mcache->mem_handle.mem_id);
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
    mcache->slabs_free_list = (struct string_slab_flist*) ptr;
    struct string_slab_flist* flist = mcache->slabs_free_list;
    while((uint64*) (ptr + slab_size) < (uint64*) (address + total_alloc_size)) {
        ptr += slab_size;
        flist->next = (struct string_slab_flist*) ptr;
        flist = flist->next;
    }

    /* avoid list getting an address pointer from the memory manager's free list */
    /* MAYBE BUG: If we null the memman freelist then we have a bug */
    flist->next = NULL;

    /* DEBUG-FPRINT */
#ifdef DEBUG_FPRINT_MCACHE
    fs_register_mcache_extend_alloc(fs_mcaches[mcache->idx], mcache, address);
#endif

    return mcache;
}

struct string_slab_flist*
string_mem_cache_alloc_slab(struct string_mem_cache* mcache)
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
        mcache = string_mem_cache_extend_allocation(mcache, left_over_mem);
    }


    /* get next free slab */
    struct string_slab_flist* free = mcache->slabs_free_list;
    mcache->slabs_free_list = mcache->slabs_free_list->next;

    mcache->last_slab_end = (char*) free + mcache->slab_size;


    /* update mcache */
    mcache->slabs_used++;
    mcache->slabs_count++;

    /* DEBUG-FPRINT */
#ifdef DEBUG_FPRINT_MCACHE
    fs_register_mcache_alloc_slab(fs_mcaches[mcache->idx], mcache);
#endif

    return free;
}

bool
string_mem_cache_free_slab(struct string_mem_cache* mcache, void* slab_address)
{
    // TODO: write test
    struct string_slab_flist* released = (struct string_slab_flist*) slab_address;
    struct string_slab_flist* node = mcache->slabs_free_list;

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
        struct string_slab_flist* temp = node->next;
        node->next = released;
        node->next->next = temp;
    } else {
        /* insert at the end */
        node->next = (struct string_slab_flist*) slab_address;
        node->next->next = NULL;
    }

    /* update mcache */
    mcache->slabs_used--;
    mcache->slabs_count--;

    /* DEBUG-FPRINT */
#ifdef DEBUG_FPRINT_MCACHE
    fs_register_mcache_release_slab(fs_mcaches[mcache->idx], mcache);
#endif

    return TRUE;
}


/***************************************************************************************************
 * HELPER FUNCTIONS */
memid_t
string_slab_alloc_instance_find_size(const slab_size_t slab_size)
{
    memid_t memid = -1; // uint guarantees UINT_MAX
    for(uint32 i = 0; i < STRING_SLAB_ALLOCATOR_INSTANCE_SIZES; i++) {

        if(slab_size < STRING_SLAB_ALLOCATOR_INSTANCE_SIZE_SMALL) {
            memid = string_slab_allocator_table[i].mem_id;
        } else if(slab_size < STRING_SLAB_ALLOCATOR_INSTANCE_SIZE_MEDIUM) {
            memid = string_slab_allocator_table[i].mem_id;
        } else if(slab_size < STRING_SLAB_ALLOCATOR_INSTANCE_SIZE_LARGE){
            memid = string_slab_allocator_table[i].mem_id;
        }
        else assert(1);
    }

    return memid;
}

struct string_mem_cache*
string_mem_cache_find_cache(const slab_size_t slab_size)
{
    STRING_ALLOC_CHECK_MCACHE_TABLE(string_mcache_table);
    for(uint32 i = 0; i < STRING_SLAB_SIZES_LEN; i++) {
        if(string_mcache_table[i].slab_size == slab_size) {
            return &string_mcache_table[i];
        }
    }

    return NULL;
}

struct string_mem_cache*
string_mem_cache_get_next_free_cache(void)
{
    for(uint32 i = 0; i < STRING_SLAB_SIZES_LEN; i++) {
        if(string_mcache_table[i].is_active == FALSE) {
            string_mcache_table[i].idx = i;
            return &string_mcache_table[i];
        }
    }

    return NULL;
}


/* DEBUG */
void*
string_mem_cache_flist_peek_next_free_slab(const string_size_t string_size)
{
    const slab_size_t slab_size = string_size;

    const struct string_mem_cache* mcache = string_mem_cache_find_cache(slab_size);
    struct string_slab_flist* flist_head = mcache->slabs_free_list;
    return flist_head;
}
