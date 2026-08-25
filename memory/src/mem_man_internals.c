/*
 * Created by ckalo on 1/4/26.
 * todo: create implementation explanation
 */

#include "mem_man_internals.h"
#include <assert.h>
#include "mem_log.h"
#include "sys_utils.h"
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "my_macros.h"
#include "mem_man_macros.h"
#include "smath.h"

#include <sanitizer/asan_interface.h>



/*  */
memid_t mem_create_new_instance(const uint16 num_pages, const uint32 alignment, const char* name);


/***************************************************************************************************
 * MEMORY POOL
 * local buffer to store all allocated memory requests
 **************************************************************************************************/
static struct mem_block* mem_pool = NULL;
static uint32 memblock_pool_count = 0;
struct mem_block* mem_pool_get() { return mem_pool; }

void
mem_pool_init(void)
{
    const uint32 capacity = MEMORY_INTERNAL_POOL_INITIAL_COUNT;
    const uint32 size = sizeof(struct mem_block)*capacity;
    const uint32 alloc_size = ALIGN_SIZE(size, 64);
    mem_pool = aligned_alloc(64, alloc_size);
    memset(mem_pool, 0x0, alloc_size);

    // mem_fprint_append_mem_pool(mem_pool, MEMORY_INTERNAL_POOL_INITIAL_COUNT, size, alloc_size);

#   ifdef ENABLE_SANITIZER
        /* enable santizers to the stderr out */
        __sanitizer_set_report_path("stderr");
#   endif

    print("%s# INITIALIZE MEMORY #%s\n\n", YELLOW, CLEAR);
    print("%s# Allocate mem pool: %p %s\n", YELLOW, mem_pool, CLEAR);
    print("--%s memory pool buffer %s--\n", ry, CLEAR);
    print(
        " capacity:        %s%u  %s\n"
        " size bytes:      %s%lu %s\n"
        " aloc size:       %s%lu %s\n"
        " data start:      %s%x%s\n",
        GREEN, capacity,    CLEAR,
        YELLOW, size,       CLEAR,
        YELLOW, alloc_size, CLEAR,
        CYAN, REDUCE_ADDR((char*)mem_pool, F6), CLEAR
    );

}
void mem_pool_realloc(){} /// TODO: implement






/***************************************************************************************************
 * FUNCTIONS FROM INTERFACE
 **************************************************************************************************/
enum MEMORY_DEBUG
mem_pool_destroy(void)
{
    if(!mem_pool) return NO_MEMORY_MANAGER_INIT;

    for(uint32 i = 0; i < memblock_pool_count; i++) {

        void* ptr = mem_pool[i].start;
        if(ptr == NULL)
            assert(ptr != NULL);

        print("free mem_block[%u]: %p\n", i, ptr);
        free(ptr);
        ptr = NULL;
    }

    print("Freeing memory pool: %p ... ", mem_pool);
    free(mem_pool);
    print("%sOK%s\n", GREEN, CLEAR);
    return MEMORY_TERMINATE_SUCCESS;
}

memid_t
mem_create_inst(const uint16 num_pages, const uint32 alignment, const char* name)
{
    /// initiaslize new mem block instance
    memid_t inst_id = mem_create_new_instance(num_pages, alignment, name);
    assert(inst_id < max_value_from_type(inst_id)); /// id values < type-MAX


    // struct mem_block* block = mem_pool_get_block(inst_id);
    // mem_log_print_mem(block);
    // mem_log_print_free_list(inst_id);

    return inst_id;
}

memid_t
mem_destroy_inst(const memid_t memid)
{
    return 0;
}

struct memhnd_t
mem_inst_request_pages(const uint16 num_pages, const memid_t mem_id)
{
    // HERE: double calling mem_pool_get_block(mem_id)
    // flist_request_free_page() is used only by this function, so we can padd the mem to it
    struct mem_block* mem = mem_pool_get_block(mem_id);

    CHECK_MEM_NULL(mem, NO_MEMORY_MANAGER_INIT, "mem_inst_request_pages");
    const struct memhnd_t handle = flist_request_free_page(num_pages, mem_id);

    mem->num_pages += handle.num_pages;

    return handle;
}

// struct memhnd_t
// mem_realloc(const pageid_t num_pages, const memid_t mem_id)
// {
//     print_implement("mem_realloc()", __FILE__, __LINE__);
// }

bool
mem_inst_release_pages(const struct memhnd_t hnd)
{
    /* DEBUG-ERROR */
    assert(hnd.num_pages > 0);
    bool already_freed = debug_mem_freeing_already_freed_pages(hnd);
    if(already_freed) {
        struct flist* released = calc_address_from_page_id(hnd.page_id, hnd.mem_id);
        print("%sTRYING TO RELEASE AN ALREADY RELEASED MEM PAGE%s\n"
              "memid: %u\n"
              "released pageid: %u\n"
              "released  addr:  %x\n",
              RED, CLEAR, hnd.mem_id, hnd.page_id, released);
        mem_print_error(FREE_PAGE_ALREADY_FREED);
    }

    /*
     * we have 2 cases
     * 1. free.id < head.id (first)
     * 2. free.id > head.id
     */

    struct flist* head = flist_get_head(hnd.mem_id);
    const memid_t headpageid = calc_page_id_from_address((void*) head, hnd.mem_id);

    struct flist* cur = head;
    struct flist* prev = cur;

    /* 1. check if free id is < flist head id */
    if(hnd.page_id < headpageid) {
        pageid_t pageid = hnd.page_id;
        prev = calc_address_from_page_id(/**/pageid, hnd.mem_id);
        /* set free list head */
        flist_set_head(hnd.mem_id, prev);

        for(uint32 i = 1; i < hnd.num_pages; i++) {
            pageid++;
            cur = calc_address_from_page_id(pageid, hnd.mem_id);
            prev->next = cur;
            prev = cur;
        }

        prev->next = head;
    } else {
        if(hnd.page_id > headpageid) {
            struct flist* released = calc_address_from_page_id(hnd.page_id, hnd.mem_id);
            const struct flist* flist = head;
            cur = head;

            /* find previous node from the released node, in the free list */
            while(cur && (char*) cur < (char*) released) {
                prev = cur;
                cur = cur->next;
            }

            /* at this point cur points at the first freed object in the free list (after released address)*/
            struct flist* temp = cur;
            assert(flist);

            /* connect prev of released free node, to the released */
            prev->next = released;


            pageid_t pageid = hnd.page_id;
            for(pageid_t i = 1; i < hnd.num_pages; i++) {
                cur = calc_address_from_page_id(pageid + i, hnd.mem_id);
                released->next = cur;
                released = released->next;
            }

            /* connect the released to the next in the free list */
            released->next = temp;
        } else {}
    }

    mem_print_mem_release(calc_address_from_page_id(hnd.page_id, hnd.mem_id), hnd);

    // mem_log_print_free_list(hnd.mem_id);

    return TRUE;
}

void*
mem_inst_get_address(const struct memhnd_t hnd)
{
    void* addr = calc_address_from_page_id(hnd.page_id, hnd.mem_id);
    return addr;
}




/***************************************************************************************************
 * PRIVATE
 **************************************************************************************************/
memid_t
mem_create_new_instance(const uint16 num_pages, const uint32 alignment, const char* name)
{
    assert(MEMORY_POISON_SIZE <= MAX_UINT64);

    /* case mem_pool is uninitialized*/
    if(!mem_pool)
        mem_pool_init();

    const uint32 size = num_pages * PAGE_SIZE;
    const uint32 poison_data_size = BLOCK_POISON_DATA_SIZE;
    const uint32 total_size = size + poison_data_size;

    assert(num_pages > 0);
    assert(is_power_of_two(alignment));  /* alignment must be power of 2 */
    assert(alignment<total_size);
    assert((total_size%alignment) == 0); /* size must be multiple of alignment */

    // assert(IS_POWER_OF_2(alignment));
    // assert(IS_POWER_OF_2(total_size));

    /* initialize pool table */
    struct mem_block* addr = aligned_alloc(alignment, total_size);
    memset(addr, 0x0, total_size);
    CHECK_MEM_NULL(addr, MALLOC_FAILED_TO_ALLOC_MEMORY, "MALLOC_FAILED_TO_ALLOC_MEMORY\nin mem_table_init()");
    // print("%s# Allocate mem block: %p %s\n", YELLOW, addr, CLEAR);


    const memid_t inst_id = memblock_pool_count;
    memblock_pool_count++;

    mem_pool[inst_id].start            = addr;
    mem_pool[inst_id].data_size        = size;
    mem_pool[inst_id].size             = total_size;
    mem_pool[inst_id].num_pages        = num_pages;
    mem_pool[inst_id].poison_data_size = poison_data_size;
    strncpy(mem_pool[inst_id].name, name, strlen(name));

    /* set poison data area, for corruption detection */
    char* off_bound_mem_start = (char*) mem_pool[inst_id].start + mem_pool[inst_id].data_size;
    uint64* ptr = (uint64*) off_bound_mem_start;
    const uint32 len = poison_data_size / sizeof(uint64);
    uint32 i = 0;
    for(; i < len; i++)
        ptr[i] = MEM_POISON_PATTERN;

    // if poison_data is treated as string data, should end with NULL
    // Set the last byte of the last uint64 to 0
    char* last_byte = (char*)&ptr[len-1] + 7;
    *last_byte = 0;


    // test memory corruption
    const bool memory_corrupted = check_mem_off_boundaries_writes(off_bound_mem_start, poison_data_size);
    if(memory_corrupted) {
        my_exit("ERROR: MEMORY CORRUPTION DETECTED", __FILE__, __LINE__);
    }

    mem_init_free_list_table(&mem_pool[inst_id]);

    return inst_id;
}

void
mem_init_free_list_table(struct mem_block* mem)
{
    /* */
    mem->flist = mem->start; // start of struct flist storage
    struct flist* flist = mem->flist;
    char* color_page = FREE_LIST_GET_PAGE_COLOR_OFFSET(flist);
    char* page_ptr = (char*) mem->flist;

    /* initialize all memory as free pages */
    /* INFO: remove one page, despite the memory poison corruption size,
     * so last free in list does not point to poison(its content be NULL) */
    const char* data_mem_end = (char*) mem->start + mem->data_size;
    while(page_ptr < data_mem_end) {
        color_page = FREE_LIST_GET_PAGE_COLOR_OFFSET(flist);
        color_page[0] = 'F'; color_page[1] = 'R'; color_page[2] = 'E'; color_page[3] = 'E';

        page_ptr = page_ptr + PAGE_SIZE;
        flist->next = (struct flist*) page_ptr;
        flist = flist->next;
    }

    flist->next = NULL; /// BUG! the write to the flist->next does not belong to the memory allocation
}

struct memhnd_t
flist_request_free_page(const uint16 num_pages, const memid_t mem_id)
{
    const struct mem_block* mem = mem_pool_get_block(mem_id);
    if(!mem->flist){
        mem_print_error(NO_FREE_MEMORY);
        if(!mem->flist) print("free list = NULL\n");
        else print("free list -> next = NULL  free list:%p\n", mem->flist);
        my_exit("NO FREE MEMORY", __FILE__, __LINE__);
    }


    struct flist* head = mem->flist;
    struct flist* flist = head;
    const struct flist* found = flist;
    struct flist* prev = flist;
    struct flist* temp = head;
    pageid_t numpages = num_pages;

    /* find contiguous mem_block pages
     * set found = to flist head
     * run the free list
     * if next page is not contiguous
     *      numpages = original num_pages
     *      Reset: found = current flist
     *
     * SEE: memory_manager-free_list-find_contig_pages_1.png
     */
    pageid_t pageid = calc_page_id_from_address(flist, mem_id);


    /* we already got 1 free page */
    flist = flist->next;
    numpages--;

    /* find enough num_pages for the allocation.
     * keep track of the last free page (before allocated memory)
     * keep track of the next free page (after allocated memory)
     */
    while(numpages > 0) {
        if(flist == NULL) break;

        const pageid_t nextpageid = calc_page_id_from_address(flist, mem_id);
        const pageid_t nextpageoffset = num_pages - (numpages);

        if(pageid + nextpageoffset != nextpageid) {
            numpages = num_pages;
            found = flist;
            // pageid = calc_page_id_from_address(flist, mem_id);
            pageid = nextpageid; /// copy the already calculated pageid from a previous call to calc_page_id_from_address(flist, mem_id)
            temp = prev; /* temp stores the last free node before the allocation */
        }
        prev = flist;
        flist = flist->next;
        numpages--;
    }


    if(num_pages <= 0 || numpages != 0) {
        mem_print_error(NO_FREE_MEMORY);
        print("num pages left:%u \n", num_pages);
        my_exit("NO FREE MEMORY", __FILE__, __LINE__);
    }
    assert(found);

    /* at this point, flist should point to the first
     * free node after the allocated memory */
    struct flist* next_from_allocated = flist;


    /* connect last free page (before allocated memory)
     * to the next free page (after allocated memory) */
    if(head != found)
        temp->next = next_from_allocated;
    else /* if allocated starts from head, advance head accordingly */
        flist_set_head(mem_id, flist);

    /* zero out allocated memory */
    void* alloc_start = (char*) mem->start + (pageid * PAGE_SIZE);
    memset(alloc_start, 0x0, num_pages * PAGE_SIZE); /// reset requested memory before giving it to the caller

    // mem_log_print_free_list(mem_id);
    debug_mem_corrupted(mem_id, TRUE);


    const struct memhnd_t handle = {
        .mem_id = mem_id,
        .page_id = pageid,
        .num_pages = num_pages,
    };


    return handle;
}

struct flist*
flist_get_head(const memid_t mem_id)
{
    const struct mem_block* mem = mem_pool_get_block(mem_id);
    return mem->flist;
}

void
flist_set_head(const memid_t mem_id, struct flist* head)
{
    struct mem_block* mem = mem_pool_get_block(mem_id);
    mem->flist = head;
}

pageid_t
calc_page_id_from_address(const void* addr, const memid_t mem_id)
{
    const struct mem_block* mem = mem_pool_get_block(mem_id);
    const char* cur_addr = addr;

    const uint64 ptr_dif = cur_addr - (char*) mem->start;
    if(ptr_dif == 0)
        return 0;

    const pageid_t page_id = ptr_dif / PAGE_SIZE;
    return page_id;
}

void*
calc_address_from_page_id(const pageid_t page_id, const memid_t mem_id)
{
    const struct mem_block* mem = mem_pool_get_block(mem_id);
    void* addr = (char*) mem->start + (PAGE_SIZE * page_id);
    return addr;
}



/***************************************************************************************************
 * DEBUG
 **************************************************************************************************/
struct mem_block*
debug_mem_get_table() { return mem_pool; }

struct mem_block*
mem_pool_get_block(const memid_t inst_id)
{
    struct mem_block* block = &mem_pool[inst_id];
    CHECK_MEM_NULL(block, MALLOC_FAILED_TO_ALLOC_MEMORY, "MALLOC_FAILED_TO_ALLOC_MEMORY\nin mem_table_init()");
    return block;
}

uint32
debug_mem_get_table_count() { return memblock_pool_count; }

void*
debug_mem_get_start_address(const memid_t memid) { return mem_pool[memid].start; }

uint64
debug_mem_get_size(const memid_t memid) { return mem_pool[memid].size; }

void*
flist_peek_next_free_page(const memid_t mem_id)
{
    struct flist* flist_head = flist_get_head(mem_id);
    return flist_head;
}

bool
debug_mem_freeing_already_freed_pages(const struct memhnd_t hnd)
{
    struct flist* flist = flist_get_head(hnd.mem_id);
    const struct flist* released = calc_address_from_page_id(hnd.page_id, hnd.mem_id);
    while(flist) {
        if(flist == released)
            return TRUE;
        flist = flist->next;
    }

    return FALSE;
}

void
debug_mem_corrupted(const memid_t memid, const bool silent)
{
    const uint32 flist_null = sizeof(struct flist);
    void* off_bound_mem_start = mem_pool[memid].start + mem_pool[memid].data_size + flist_null;
    const bool memory_corrupted = check_mem_off_boundaries_writes(off_bound_mem_start, MEMORY_POISON_SIZE-flist_null);
    if(memory_corrupted) {
        my_exit("ERROR: MEMORY CORRUPTION DETECTED", __FILE__, __LINE__);
    } else if(!silent){
        print("%s MEMORY CORRUPTION: PASS %s\n", GREEN, CLEAR);
    }
}

bool
check_mem_off_boundaries_writes(void* mem_start, const uint32 size)
{
    /*
     * Check MEMORY_POISON_SIZE bytes
     * excluding the first 8 bytes that the freelist
     * occupies and have to be NULL,
     * and also excluding the last 8 bytes
     * where we deliberately set the last byte to '\0'
     * for debugging purposes.
     * Example:
     * size:  64 bytes
     *                 |-------------- this is what we check --------------|
     *        byte 0-7  8-15     16-23    24-31    32-39    40-47    48-55    55-62   63
     * data:  00000000 |POISON| |POISON| |POISON| |POISON| |POISON| |POISON| |POISON  '\0'
     */
    const uint64* ptr = (uint64*) mem_start;
    const uint32 len = size / sizeof(uint64);
    for(uint32 i = 0; i < len-1; i++) {
        if(ptr[i] != MEM_POISON_PATTERN) {
            return TRUE;
        }
    }
    return FALSE;
}

bool
debug_flist_check_continuity(const memid_t memid)
{
    struct flist* flist = flist_get_head(memid);
    bool is_contiguous = TRUE;

    while(flist != NULL) {
        if(flist->next && (char*) flist + PAGE_SIZE != (char*) flist->next)
            is_contiguous = FALSE;
        flist = flist->next;
    }

    return is_contiguous;
}
