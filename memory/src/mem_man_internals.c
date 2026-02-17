/*
 * Created by ckalo on 1/4/26.
 *
 * todo: handle mem_alloc exceeds chunks num pages
 */

#include "mem_man_internals.h"
#include "mem_structs.h"
#include "mem_log.h"
#include "sys_utils.h"
#include <stdlib.h>
#include "logger.h"


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


/***************************************************************************************************
 * memory manager -> mms
 * is a single allocation at the start of a program, no realloc is implemented,
 * so is plug&play to find the appropriate total mem_chunk size a program needs.
 **************************************************************************************************/

static struct mem* mem = NULL;
static struct mem_chunk* chunks_table = NULL; // mem is devided into chunks
static struct free_list** flist_heads_table = NULL; // table stores:flist heads per chunk


bool
mem_init(void)
{
    if(mem) return FALSE;

    mem_log_print_init_mem();

    const uint32 page_size = PAGE_SIZE;
    const uint32 num_pages = 512;
    const uint32 alignment = NUM_CHUNKS * PAGE_SIZE;
    const uint32 self_storage_size = PAGE_SIZE;
    const uint32 memory_bound_check = PAGE_SIZE;

    const uint32 total_size = self_storage_size + (num_pages * page_size) + memory_bound_check;

    void* addr = aligned_alloc(alignment, total_size);

    if(!addr) {
        mem_print_error(MALLOC_FAILED_TO_ALLOC_MEMORY);
        my_exit("MALLOC_FAILED_TO_ALLOC_MEMORY", __FILE__, __LINE__);
        return FALSE; // for IDE complaining about null ptr addr
    }


    mem = addr;
    mem->start = addr;
    mem->size = total_size;
    mem->data_size = num_pages * page_size;
    mem->self_size = self_storage_size;
    mem->bound_check_size = memory_bound_check;
    mem->chunks_count = 0;

    mem_init_chunks_table();

    mem_init_free_list_table();


    /* *************************************************
     * set off boundary area, for corruption detection */
    void* off_bound_mem_start = addr + (self_storage_size + (num_pages * page_size));
    uint64* ptr = (uint64*) off_bound_mem_start;
    const uint32 len = PAGE_SIZE / sizeof(uint64);
    for(uint32 i = 0; i < len; i++) {
        ptr[i] = MEM_OFF_BOUNDARIES_CHECK;
    }
    // test it
    const bool memory_corrupted = check_mem_off_boundaries_writes(off_bound_mem_start, PAGE_SIZE);
    if(memory_corrupted) {
        my_exit("ERROR: MEMORY CORRUPTION DETECTED", __FILE__, __LINE__);
    }

    mem_log_print_mem(mem);

    return TRUE;
}

bool
mem_free()
{
    if(!mem) return FALSE;
    free(mem);
    return TRUE;
}

struct memhnd_t
mem_alloc(const pageid_t num_pages, const enum MEM_CHUNK_SELECTOR chunk_selector)
{
    CHECK_MEM_NULL(mem, NO_MEMORY_MANAGER_INIT, "mem_alloc");

    const memid_t chunk_id = chunk_selector_get_chunk(chunk_selector);
    const void* addr = flist_get_next_free_page(num_pages, chunk_id);
    const memid_t page_id = calc_page_id_from_address(addr);

    const struct memhnd_t handle = {
        .chunk_id = chunk_id,
        .page_id = page_id,
        .num_pages = num_pages,
    };

    return handle;
}

bool
mem_release(const struct memhnd_t hnd)
{
    if(hnd.num_pages == 0) {
        mem_print_error(MEMORY_IS_IN_USE);
        my_exit("mem_release", __FILE__, __LINE__);
    }

    /*
     * we have 3 cases
     * 1. free.id < head.id (first)
     * 2. free.id > head.id && free.id < 31 (in-between)
     * 3. free.id + num_pages == 31 (last)
     */

    struct free_list* head = flist_table_get_head(hnd.chunk_id);
    const memid_t headpageid = calc_page_id_from_address((void*) head);

    const memid_t fpageid = hnd.page_id;

    struct free_list* prev = NULL;
    struct free_list* next = NULL;

    for(uint32 i = 0; i < hnd.num_pages; i++) {}


    /* 1. check if free id is < flist head id */
    if(fpageid < headpageid) {
        memid_t pageid = hnd.page_id;
        prev = calc_address_from_page_id(pageid);
        flist_table_set_head(hnd.chunk_id, prev);

        for(uint32 i = 1; i < hnd.num_pages; i++) {
            pageid++;
            next = calc_address_from_page_id(pageid);
            prev->next = next;
            prev = next;
        }

        prev->next = head;
    } else {
        if(fpageid > headpageid && fpageid < PAGES_PER_CHUNK - 1) {
            memid_t pageid = hnd.page_id - 1;
            prev = calc_address_from_page_id(pageid);
            struct free_list* temp = prev->next;

            for(uint32 i = 0; i < hnd.num_pages; i++) {
                pageid++;
                next = calc_address_from_page_id(pageid);
                prev->next = next;
                prev = next;
            }

            prev->next = temp;
        } else {}
    }

    // mem_print_mem_release(released_addr, hnd);

    return TRUE;
}


void*
mem_get_addr_by_handle(const struct memhnd_t hnd)
{
    void* addr = calc_address_from_page_id(hnd.page_id);
    return addr;
}


/* *************************************************************************************************
 * private
 * ************************************************************************************************/

/* DEBUG */
struct mem*
debug_mem_get() { return mem; }

uint16
debug_mem_get_chunks_count() { return mem->chunks_count; }

struct mem_chunk*
debug_chunks_table_get() { return chunks_table; }

struct free_list**
debug_free_list_table_get() { return flist_heads_table; }

void
mem_init_chunks_table()
{
    void* ptr = mem;
    // advance pointer to free memory, where we can store chunks_table
    ptr = (char*) ptr + sizeof(struct mem);

    chunks_table = (struct mem_chunk*) ptr; // start of struct chunks_table storage

    /*************************************************************
     * chunks_table storage layout
     *************************************************************/
    for(uint32 i = 0; i < NUM_CHUNKS; i++) {
        chunks_table[i].id = i;
        chunks_table[i].flags.is_init = 0;

        // properly count total number of chunks
        mem->chunks_count++;
    }
}

void
mem_init_free_list_table()
{
    /*************************************************************
     * flist_heads_table storage layout
     *************************************************************/
#define COLOR_PAGE_FREE "FREE" // color a page  to denote that belongs to free list

    void* ptr = mem;
    // advance pointer to free memory, where we can store mem_used_table
    ptr = (char*) ptr + sizeof(struct mem); // advance through struct mem
    // advance through struct mem_chunk * number of chunks
    ptr = (char*) ptr + (sizeof(struct mem_chunk) * mem->chunks_count);

    flist_heads_table = ptr; // start of struct free_list storage


    void* page_ptr = MEM_GET_DATA_START;
    // initialize all free list heads and free lists for each mem_chunk
    for(uint32 chunk_id = 0; chunk_id < mem->chunks_count; chunk_id++) {
        // create storage for head pointer
        flist_heads_table[chunk_id] = (struct free_list*) page_ptr;

        struct free_list* cur = flist_heads_table[chunk_id];
        char* color_page = FREE_LIST_GET_PAGE_COLOR_START(cur);
        color_page[0] = 'F';
        color_page[1] = 'R';
        color_page[2] = 'E';
        color_page[3] = 'E';


        for(uint16 i = 0; i < PAGES_PER_CHUNK - 1; i++) {
            page_ptr = (char*) page_ptr + PAGE_SIZE;

            struct free_list* new = (struct free_list*) page_ptr;
            new->next = NULL;
            cur->next = new;
            cur = new;

            color_page = FREE_LIST_GET_PAGE_COLOR_START(new);
            color_page[0] = 'F';
            color_page[1] = 'R';
            color_page[2] = 'E';
            color_page[3] = 'E';
        }

        // cur->next = NULL;
        page_ptr = (char*) page_ptr + PAGE_SIZE;
    }
}


void*
flist_get_next_free_page(pageid_t num_pages, const memid_t chunk_id)
{
    struct free_list* flist_head = flist_table_get_head(chunk_id);
    struct free_list* fnode = flist_head;
    void* free = flist_head;

    if(!flist_head) { // todo: deal with :: no mem left in this chunk, advance chunk and allocate from that
        my_exit("NO FREE PAGES FOR CHUNK\nFREE_LIST OUT OF MEMORY", __FILE__, __LINE__);
    }

    while(num_pages) {
        CHECK_CONTIGUOUS_FREE_MEM(fnode, PAGE_SIZE, flist_head, ALLOC_NOT_CONTIGUOUS_MEM, "flist_get_next_free_page");
        CHECK_MEM_NULL(free, NO_FREE_MEMORY, "flist_get_next_free_page");

        // remove page free coloring
        char* color_page = FREE_LIST_GET_PAGE_COLOR_START(flist_head);
        color_page[0] = '\0'; color_page[1] = '\0'; color_page[2] = '\0'; color_page[3] = '\0';

        flist_head = fnode->next;
        fnode = fnode->next;
        num_pages--;
    }

    flist_table_set_head(chunk_id, flist_head);

    return free;
}

void*
flist_peek_next_free_page(const memid_t chunk_id)
{
    struct free_list* flist_head = flist_table_get_head(chunk_id);
    return flist_head;
}

struct free_list*
flist_table_get_head(const memid_t chunk_id)
{
    struct free_list* head = flist_heads_table[chunk_id];
    return head;
}

void
flist_table_set_head(const memid_t chunk_id, struct free_list* head)
{
    flist_heads_table[chunk_id] = head;
}



memid_t
chunk_selector_get_chunk(const enum MEM_CHUNK_SELECTOR chunk_selector)
{
    if(chunk_selector == MEM_CHUNK_ALLOCATOR_SLAB) return MEM_CHUNK_ALLOCATOR_SLAB;
    return MEM_CHUNK_ANY;
}

memid_t
calc_page_id_from_address(const void* addr)
{
    const char* mem_start = (char*) mem + mem->self_size;
    const char* cur_addr = addr;

    const uint64 ptr_dif = cur_addr - mem_start;
    if(ptr_dif == 0)
        return 0;

    const memid_t page_id = ptr_dif / PAGE_SIZE;
    return page_id;
}

void*
calc_address_from_page_id(const memid_t page_id)
{
    uint64* mem_start = (uint64*) mem;
    void* addr = (char*) mem_start + (PAGE_SIZE * (page_id + 1));
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
