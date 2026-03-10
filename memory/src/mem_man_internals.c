/*
 * Created by ckalo on 1/4/26.
 *
 * todo: handle mem_alloc exceeds chunks num pages
 * todo: create implementation explanation
 */

#include "mem_man_internals.h"
#include <assert.h>
#include "mem_structs.h"
#include "mem_log.h"
#include "sys_utils.h"
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "file_sys_interface.h"
#include "logger.h"
#include "my_macros.h"
#include "stringc.h"
#include "string_format.h"


/*
 * TODO:
 *  an allocator initializes itself by calling
 */

#define CHUNK_FLAGS_FREE     BIT_MASK_FROM_UINT(0) // chunk is not in use
#define CHUNK_FLAGS_OCCUPIED BIT_MASK_FROM_UINT(1) // chunk is in use but has free pages


#define CHECK_MEM_NULL(p, error_code, msg) if(p==NULL){\
    mem_print_error(error_code);\
    my_exit(msg, __FILE__, __LINE__);\
}

#define CHECK_CONTIGUOUS_FREE_MEM(p1, offset, p2, error_code, msg)\
if((uint64*)((uint8*)p1+offset) < (uint64*)p2){\
    mem_print_error(error_code);\
    print("free list: %x\nnext flist:%x\n", REDUCE_ADDR(p1, F6), REDUCE_ADDR(p2, F6));\
    my_exit(msg, __FILE__, __LINE__);\
}

/* FOR DEBUG */
#define MEMORY_DEBUG_FPRINT_USED_MEM(fs, mem_id)\
fs_fprint_all_used_mem(fs, mem_id);\
/* FOR DEBUG */


/***************************************************************************************************
 * memory manager -> mms
 * is a single allocation at the start of a program, no realloc is implemented,
 * so is plug&play to find the appropriate total mem_chunk size a program needs.
 **************************************************************************************************/
#define MEM_TABLE_CAPACITY 16
static struct mem* mem_table = NULL;
static uint32 mem_table_count = 0;


struct mem*
mem_get_byid(const memid_t mem_id)
{
    assert(mem_id >= 0 && mem_id <= mem_table_count);
    return &mem_table[mem_id];
}

bool
mem_table_init(void)
{
    if(mem_table) return FALSE;

    mem_log_print_init_mem();

    const uint32 size = MEM_TABLE_CAPACITY * sizeof(struct mem);
    const uint32 alignment = PAGE_SIZE;
    const uint32 total_size = ROUND_UP(size, PAGE_SIZE);


    void* addr = aligned_alloc(alignment, total_size);
    CHECK_MEM_NULL(addr, MALLOC_FAILED_TO_ALLOC_MEMORY, "MALLOC_FAILED_TO_ALLOC_MEMORY\nin mem_table_init()");

    mem_table = addr;

    const struct mem memory = {
        .start = addr,
        .data_size = total_size,
        .size = total_size,
        .bound_check_size = 0,
        .name = "-memory table-"
    };


    mem_table[mem_table_count] = memory;
    mem_table_count++;

    return TRUE;
}

bool
mem_create(const uint16 num_pages, enum MEM_ID_SELECTOR mem_id)
{
    const uint32 page_size = PAGE_SIZE;
    // const uint32 num_pages = 264;
    // const uint32 num_pages = 32;
    const uint32 alignment = PAGE_SIZE * 16;
    const uint32 memory_bound_check = PAGE_SIZE;

    const uint32 total_size = (num_pages * page_size) + memory_bound_check;

    void* addr = aligned_alloc(alignment, total_size);
    CHECK_MEM_NULL(addr, MALLOC_FAILED_TO_ALLOC_MEMORY, "MALLOC_FAILED_TO_ALLOC_MEMORY\nin mem_create()");


    const struct mem memory = {
        .start = addr,
        .size = total_size,
        .data_size = num_pages * page_size,
        .bound_check_size = memory_bound_check,
        .name = "-pass name param-"
    };

    const memid_t idx = (memid_t)mem_id;
    mem_table[idx] = memory;
    mem_table_count++;

    struct mem* mem = mem_get_byid(idx);

    mem_init_free_list_table(mem);

    /* set off boundary area, for corruption detection */
    char* off_bound_mem_start = (char*) mem->start + mem->data_size;
    uint64* ptr = (uint64*) off_bound_mem_start;
    const uint32 len = PAGE_SIZE / sizeof(uint64);
    for(uint32 i = 0; i < len; i++)
        ptr[i] = MEM_OFF_BOUNDARIES_CHECK;

    // test it
    const bool memory_corrupted = check_mem_off_boundaries_writes(off_bound_mem_start, PAGE_SIZE);
    if(memory_corrupted) {
        my_exit("ERROR: MEMORY CORRUPTION DETECTED", __FILE__, __LINE__);
    }

    mem_log_print_mem(mem);
    // mem_log_print_free_list(idx);

    return TRUE;
}

bool
mem_table_destroy(void)
{
    for(uint32 i = 0; i < MEM_ID_MAX_ELEMS; i++) {

        string_t* str = string_create_empty(32);
        const char* fname = string_format(str->data, "/all_used_mem_chunks_%u.txt", i);

        if(mem_table[i].start) {
            MEMORY_DEBUG_FPRINT_USED_MEM(fname, i);
        }
    }

    if(!mem_table) return FALSE;
    print("FREEING MEMORY MANAGER: %x ... ", ry, mem_table[0].start, CLEAR);
    for(uint32 i = 0; i < mem_table_count; i++) {
        free(mem_table[i].start);
    }
    print("OK", GREEN, CLEAR);
    return TRUE;
}

void
mem_init_free_list_table(struct mem* mem)
{
    /* */
    mem->flist = mem->start; // start of struct flist storage
    struct flist* flist = mem->flist;
    char* color_page = FREE_LIST_GET_PAGE_COLOR_OFFSET(flist);
    char* page_ptr = (char*) mem->flist;

    /* initialize all memory as free pages */
    const char* data_mem_end = (char*) mem->start + (mem->data_size - mem->bound_check_size);
    while(page_ptr < data_mem_end) {

        color_page = FREE_LIST_GET_PAGE_COLOR_OFFSET(flist);
        color_page[0] = 'F'; color_page[1] = 'R'; color_page[2] = 'E'; color_page[3] = 'E';

        page_ptr = page_ptr + PAGE_SIZE;
        flist->next = (struct flist*) page_ptr;
        flist = flist->next;
    }

    // Add the last color to the NULL list element
    color_page = FREE_LIST_GET_PAGE_COLOR_OFFSET(flist);
    color_page[0] = 'F'; color_page[1] = 'R'; color_page[2] = 'E'; color_page[3] = 'E';
}

struct memhnd_t
mem_alloc(const uint16 num_pages, const memid_t mem_id)
{
    struct mem* mem = mem_get_byid(mem_id);

    CHECK_MEM_NULL(mem, NO_MEMORY_MANAGER_INIT, "mem_alloc");
    const struct memhnd_t handle = flist_get_next_free_page(num_pages, mem_id);

    return handle;
}

struct memhnd_t
mem_realloc_extend(const uint16 num_pages, const memid_t mem_id)
{
    my_exit("mem_realloc_extend() is deprecated\nImplement mem_realloc() instead", __FILE__, __LINE__);

    /*
     * here we extend an already full mem chunk with a free one
     * (maybe in contiguous memory, maybe not).
     * We pass the free chunk's id, and for the page we use the
     * first one(page id = 0).
     * Finally we build the page's free list and return the mem handle
     */
    const memid_t page_id = mem_id * PAGES_PER_CHUNK;
    const void* page_ptr = calc_address_from_page_id(page_id, mem_id);

    /* get the free lists head(should be NULL,
     * because the extended chunk is full) */
    // flist_heads_table[mem_id] = (struct flist*) page_ptr;
    struct flist* cur = mem_table_get_head(mem_id);
    char* color_page = FREE_LIST_GET_PAGE_COLOR_OFFSET(cur);
    color_page[0] = 'F';
    color_page[1] = 'R';
    color_page[2] = 'E';
    color_page[3] = 'E';

    /* insert free pages to the extended chunk.
     * note that these pages already exist as free,
     * in the chunk that the extension applies */
    for(uint16 i = 0; i < PAGES_PER_CHUNK - 1; i++) {
        page_ptr = (char*) page_ptr + PAGE_SIZE;

        struct flist* new = (struct flist*) page_ptr;
        new->next = NULL;
        cur->next = new;
        cur = new;

        color_page = FREE_LIST_GET_PAGE_COLOR_OFFSET(new);
        color_page[0] = 'F';
        color_page[1] = 'R';
        color_page[2] = 'E';
        color_page[3] = 'E';
    }


    // fs_write_list(fs_flist, flist_heads_table[mem_id],
    //     "\n--------------------\n  new memory free list\n  head:%x\n", flist_heads_table[mem_id]
    // );
    return (struct memhnd_t){
        .mem_id = mem_id,
        .page_id = page_id,
        .num_pages = num_pages,
    };
}

bool
debug_mem_freeing_already_freed_pages(const struct memhnd_t hnd)
{
    struct flist* flist = mem_table_get_head(hnd.mem_id);
    struct flist* released = calc_address_from_page_id(hnd.page_id, hnd.mem_id);
    while(flist) {
        if(flist == released)
            return TRUE;
        flist = flist->next;
    }

    return FALSE;
}

bool
mem_release(const struct memhnd_t hnd)
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

    struct flist* head = mem_table_get_head(hnd.mem_id);
    const memid_t headpageid = calc_page_id_from_address((void*) head, hnd.mem_id);

    struct flist* cur = head;
    struct flist* prev = head;

    /* 1. check if free id is < flist head id */
    if(hnd.page_id < headpageid) {
        memid_t pageid = hnd.page_id;
        prev = calc_address_from_page_id(/**/pageid, hnd.mem_id);
        /* set free list head */
        mem_table_set_head(hnd.mem_id, prev);

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
            struct flist* flist = head;
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
mem_get_addr_by_handle(const struct memhnd_t hnd)
{
    void* addr = calc_address_from_page_id(hnd.page_id, hnd.mem_id);
    return addr;
}

void*
mem_get_pageid_by_handle(const struct memhnd_t hnd)
{
    void* addr = calc_address_from_page_id(hnd.page_id, hnd.mem_id);
    return addr;
}

void* // temporary function: allocate memory seperately from mem man alloc system
mem_temp_alloc_sys_call(const uint32 alignment, const uint32 total_size)
{
    void* addr = aligned_alloc(alignment, total_size);
    if(!addr) {
        mem_print_error(MALLOC_FAILED_TO_ALLOC_MEMORY);
        my_exit("FAILED TO ALLOCATE MEMORY\nat:mem_temp_alloc_sys_call()\n", __FILE__, __LINE__);
    }
    return addr;
}


/* *************************************************************************************************
 * private
 * ************************************************************************************************/
struct memhnd_t
flist_get_next_free_page(const uint16 num_pages, const memid_t mem_id)
{
    struct mem* mem = mem_get_byid(mem_id);
    if(!mem->flist) mem_print_error(NO_FREE_MEMORY);


    struct flist* head = mem->flist;
    struct flist* flist = head;
    struct flist* found = flist;
    struct flist* prev = flist;
    struct flist* temp = head;
    pageid_t numpages = num_pages;

    /* find contiguous mem pages
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

        pageid_t nextpageid = calc_page_id_from_address(flist, mem_id);
        pageid_t nextpageoffset = num_pages - (numpages);

        if(pageid + nextpageoffset != nextpageid) {
            numpages = num_pages;
            found = flist;
            pageid = calc_page_id_from_address(flist, mem_id);
            temp = prev; /* temp stores the last free node before the allocation */
        }
        prev = flist;
        flist = flist->next;
        numpages--;
    }

    /* at this point, flist should point to the first
     * free node after the allocated memory */
    struct flist* next_from_allocated = flist;

    // print(
    //     "head: %x\n"
    //     "prev: %x\n"
    //     "flist: %x\n"
    //     "found: %x\n"
    //     "temp: %x\n"
    //     "next: %x\n",
    //     head, prev, flist, found, temp, next_from_allocated
    // );

    assert(numpages == 0); // all requested num pages should be allocated
    assert(found);

    /* connect last free page (before allocated memory)
     * to the next free page (after allocated memory) */
    if(head != found)
        temp->next = next_from_allocated;
    else /* if allocated starts from head, advance head accordingly */
        mem_table_set_head(mem_id, flist);

    /* zero out allocated memory */
    void* alloc_start = (char*) mem->start + (pageid * PAGE_SIZE);
    memset(alloc_start, 0, num_pages * PAGE_SIZE);

    // mem_log_print_free_list(mem_id);

    struct memhnd_t handle = {
        .mem_id = mem_id,
        .page_id = pageid,
        .num_pages = num_pages,
    };

    return handle;
}

void*
flist_peek_next_free_page(const memid_t mem_id)
{
    struct flist* flist_head = mem_table_get_head(mem_id);
    return flist_head;
}

struct flist*
mem_table_get_head(const memid_t mem_id)
{
    struct mem* mem = mem_get_byid(mem_id);
    return mem->flist;
}

void
mem_table_set_head(const memid_t mem_id, struct flist* head)
{
    struct mem* mem = mem_get_byid(mem_id);
    mem->flist = head;
}

pageid_t
calc_page_id_from_address(const void* addr, const memid_t mem_id)
{
    struct mem* mem = mem_get_byid(mem_id);
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
    struct mem* mem = mem_get_byid(mem_id);
    void* addr = (char*) mem->start + (PAGE_SIZE * page_id);
    return addr;
}

bool
check_mem_off_boundaries_writes(void* mem_start, const uint32 byte_size)
{
    const uint64* ptr = (uint64*) mem_start;
    const uint32 len = byte_size / sizeof(uint64);
    for(uint32 i = 0; i < len; i++) {
        if(ptr[i] != MEM_OFF_BOUNDARIES_CHECK) {
            return TRUE;
        }
    }
    return FALSE;
}


/* DEBUG */

struct mem*
debug_mem_get_table() { return mem_table; }

uint32
debug_mem_get_table_count() { return mem_table_count; }

void*
debug_mem_get_start_address(const memid_t memid) { return mem_table[memid].start; }

uint64
debug_mem_get_size(const memid_t memid) { return mem_table[memid].size; }

//
// struct flist*
// debug_mem_get_free_list(const memid_t memid) { return mem_table[memid].flist; }

void
debug_mem_corrupted(const memid_t memid)
{
    void* off_bound_mem_start = mem_table[memid].start + mem_table[memid].data_size;
    const bool memory_corrupted = check_mem_off_boundaries_writes(off_bound_mem_start, PAGE_SIZE);
    if(memory_corrupted) {
        my_exit("ERROR: MEMORY CORRUPTION DETECTED", __FILE__, __LINE__);
    } else {
        print("%s MEMORY CORRUPTION: PASS %s\n", GREEN, CLEAR);
    }
}

bool
debug_flist_check_continuity(const memid_t memid)
{
    struct flist* flist = mem_table_get_head(memid);
    bool is_contiguous = TRUE;

    while(flist != NULL) {
        if(flist->next && (char*) flist + PAGE_SIZE != (char*) flist->next)
            is_contiguous = FALSE;
        flist = flist->next;
    }

    return is_contiguous;
}
