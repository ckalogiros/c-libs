/*
 * Created by ckalo on 1/4/26.
 *
 * todo: handle mem_alloc exceeds chunks num pages
 *
 *  FORGOT THAT MEM MANAGER DEALS WITH PAGES NOT CHUNKS
 *
 * memory managers is devided in chunks
 *  chunks are a view of memory, not defined by specific size.
 *  their min size is:(PAGE_SIZE * PAGES_PER_CHUNK)
 *  but they can be extended (by PAGE_SIZE, not by min chunk size).
 *  BUT
 *  when a new chunk is requested, memory manager returns
 *  the next free chunk. that means it returns a memory address,
 *  for the new chunk, aligned to the min chunk size and not
 *  folowing the prev chunk's end address
 *
 *  0____1____2____3____4____
 *  |    |    |    |    |    |
 *  |____|____|____|____|____|
 *  ^     ^   ^
 *  used end  requested
 *
 *  if the next chunk is occupied, then memory manager
 *  advances to the next free.
 *  that means a memory chunk can refer to NOT CONTIGUOUS memory
 *  that is feasible via the free list.
 *  every chunk has a free list, that can span in a NOT CONTIGUOUS memory.
 *  the release of a chunk's memory pages is what defines these memory hops,
 *  and is the responsibility of the "allocator" to give these pages back.
 *
 *  SEE /home/ckalo/data/projects/clion/memory/memory_manager_layout-allocations.png
 *
 *  chunks have a table with pointers pointing at the start mem,
 *  and a table of free list heads for each chunk.
 *  todo: if a chunk allocates to more than on chunk memory
 *
 * if an allocator wants memory, an empty chunk is given by mem manager
 *
 * if the same allocator needs to extend its initial memory
 *  mem manager gives next available chunk(preferably contiguous in mem)
 *  mem manager releases the chunk from its ownership
 *  now it is in the allocators "hands" to deal with the memory
 *   that is acomplished with allocators free list,
 *   where it can "span" over NOT contiguous memory
 *
 * if allocator releases its memory, the mem manager adds it
 *  back to its ownership as a free chunk
 *
 *  NOW,
 *  memory manager handles chunks as either FREE or OCCUPIED
 */

#include "mem_man_internals.h"

#include <assert.h>

#include "mem_structs.h"
#include "mem_log.h"
#include "sys_utils.h"
#include <stdlib.h>

#include "file_sys_interface.h"
#include "logger.h"


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
struct fs_t* fs_flist = NULL;
static uint32 free_lists_heads_count = 0;

/***************************************************************************************************
 * memory manager -> mms
 * is a single allocation at the start of a program, no realloc is implemented,
 * so is plug&play to find the appropriate total mem_chunk size a program needs.
 **************************************************************************************************/

static struct mem* mem = NULL;
static struct mem_chunk* chunks_table = NULL; // mem is devided into chunks
static struct flist** flist_heads_table = NULL; // table stores:flist heads per chunk


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
    print("FREEING MEMORY MANAGER: %x ... ", ry, mem, CLEAR);
    free(mem);
    print("OK", GREEN, CLEAR);
    return TRUE;
}

struct memhnd_t
mem_alloc(const pageid_t num_pages, const enum MEM_CHUNK_SELECTOR chunk_selector)
{
    CHECK_MEM_NULL(mem, NO_MEMORY_MANAGER_INIT, "mem_alloc");

    const memid_t chunk_id = chunk_selector_get_chunk(chunk_selector);
    const struct memhnd_t handle = flist_get_next_free_page(num_pages, chunk_id);

    chunks_table[chunk_id].flags |= CHUNK_FLAGS_OCCUPIED;
    chunks_table[chunk_id].flags &= ~CHUNK_FLAGS_FREE;


    // fs_write_list(fs_flist, flist_heads_table[chunk_id],
    //     "\n--------------------\n"
    //     "  new memory free list\n"
    //     "  id:   %u\n"
    //     "  head: %x\n",
    //     free_lists_heads_count,
    //     flist_heads_table[chunk_id]
    // );
    // free_lists_heads_count++;


    return handle;
}


struct memhnd_t
mem_alloc_extend_chunk(const pageid_t num_pages, const memid_t chunk_id)
{
    /*
     * here we extend an already full mem chunk with a free one
     * (maybe in contiguous memory, maybe not).
     * We pass the free chunk's id, and for the page we use the
     * first one(page id = 0).
     * Finally we build the page's free list and return the mem handle
     */
    const memid_t page_id = chunk_id * PAGES_PER_CHUNK;
    const void* page_ptr = calc_address_from_page_id(page_id);

    /* get the free lists head(should be NULL,
     * because the extended chunk is full) */
    flist_heads_table[chunk_id] = (struct flist*) page_ptr;
    struct flist* cur = flist_heads_table[chunk_id];
    char* color_page = FREE_LIST_GET_PAGE_COLOR_START(cur);
    color_page[0] = 'F'; color_page[1] = 'R'; color_page[2] = 'E'; color_page[3] = 'E';

    /* insert free pages to the extended chunk.
     * note that these pages already exist as free,
     * in the chunk that the extension applies */
    for(uint16 i = 0; i < PAGES_PER_CHUNK - 1; i++) {
        page_ptr = (char*) page_ptr + PAGE_SIZE;

        struct flist* new = (struct flist*) page_ptr;
        new->next = NULL;
        cur->next = new;
        cur = new;

        color_page = FREE_LIST_GET_PAGE_COLOR_START(new);
        color_page[0] = 'F'; color_page[1] = 'R'; color_page[2] = 'E'; color_page[3] = 'E';
    }

    chunks_table[chunk_id].flags = CHUNK_FLAGS_OCCUPIED;

    // fs_write_list(fs_flist, flist_heads_table[chunk_id],
    //     "\n--------------------\n  new memory free list\n  head:%x\n", flist_heads_table[chunk_id]
    // );
    return (struct memhnd_t){
        .chunk_id = chunk_id,
        .page_id = page_id,
        .num_pages = num_pages,
    };
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

    struct flist* head = flist_table_get_head(hnd.chunk_id);
    const memid_t headpageid = calc_page_id_from_address((void*) head);

    const memid_t fpageid = hnd.page_id;

    struct flist* prev = NULL;
    struct flist* next = NULL;

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
            struct flist* temp = prev->next;

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
    chunks_table[hnd.chunk_id].flags = CHUNK_FLAGS_FREE;

    return TRUE;
}


void*
mem_get_addr_by_handle(const struct memhnd_t hnd)
{
    void* addr = calc_address_from_page_id(hnd.page_id);
    return addr;
}

void*
mem_get_pageid_by_handle(const struct memhnd_t hnd)
{
    void* addr = calc_address_from_page_id(hnd.page_id);
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
        chunks_table[i].flags |= CHUNK_FLAGS_FREE;

        // properly count total number of chunks
        mem->chunks_count++;
    }


    fs_flist = fs_create("memory_flist.txt");

    // DEBUG-WRITE
    // const char* msg = "mem_init_chunks_table()";
    // fs_mem_blocks_debug(FILENAME_MEM_DEBUG, (char*)chunks_table, (char*)chunks_table+(mem->chunks_count*sizeof(struct mem)), msg);

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

    flist_heads_table = ptr; // start of struct flist storage


    void* page_ptr = MEM_GET_DATA_START;
    // initialize all free list heads and free lists for each mem_chunk
    for(uint32 chunk_id = 0; chunk_id < mem->chunks_count; chunk_id++) {
        // create storage for head pointer
        flist_heads_table[chunk_id] = (struct flist*) page_ptr;

        struct flist* cur = flist_heads_table[chunk_id];
        char* color_page = FREE_LIST_GET_PAGE_COLOR_START(cur);
        color_page[0] = 'F'; color_page[1] = 'R'; color_page[2] = 'E'; color_page[3] = 'E';


        for(uint16 i = 0; i < PAGES_PER_CHUNK - 1; i++) {
            page_ptr = (char*) page_ptr + PAGE_SIZE;

            struct flist* new = (struct flist*) page_ptr;
            new->next = NULL;
            cur->next = new;
            cur = new;

            color_page = FREE_LIST_GET_PAGE_COLOR_START(new);
            color_page[0] = 'F'; color_page[1] = 'R'; color_page[2] = 'E'; color_page[3] = 'E';
        }

        // cur->next = NULL;
        page_ptr = (char*) page_ptr + PAGE_SIZE;
    }

    // // DEBUG-WRITE
    // const char* msg = "mem_init_free_list_table()";
    // fs_mem_blocks_debug(FILENAME_MEM_DEBUG, (char*)flist_heads_table,
    //     (char*)flist_heads_table+(mem->chunks_count*PAGES_PER_CHUNK), msg);

}


struct memhnd_t
flist_get_next_free_page(pageid_t num_pages, const memid_t chunk_id)
{
    struct flist* flist_head = flist_table_get_head(chunk_id);
    void* free = flist_head;

    struct memhnd_t handle = {0};


    assert(free);
    handle.num_pages = num_pages;
    handle.chunk_id = chunk_id;
    handle.page_id = calc_page_id_from_address(free);

    fs_append(fs_flist,
        "free:%llx\n",
        free
    );

    while(num_pages) {
        CHECK_CONTIGUOUS_FREE_MEM(flist_head, PAGE_SIZE, flist_head, ALLOC_NOT_CONTIGUOUS_MEM, "flist_get_next_free_page");
        CHECK_MEM_NULL(free, NO_FREE_MEMORY, "flist_get_next_free_page");
        if(flist_head->next == NULL) {
            const memid_t new_chunk_id = chunk_selector_get_next_free_chunk();
            const struct memhnd_t new_handle =  mem_alloc_extend_chunk(num_pages,  new_chunk_id);
            void* address = mem_get_addr_by_handle(new_handle);
            flist_head->next = (struct flist*)address;
        }
        flist_head = flist_head->next;
        num_pages--;
    }


    flist_table_set_head(chunk_id, flist_head);

    return handle;
}

void*
flist_peek_next_free_page(const memid_t chunk_id)
{
    struct flist* flist_head = flist_table_get_head(chunk_id);
    return flist_head;
}

struct flist*
flist_table_get_head(const memid_t chunk_id)
{
    struct flist* head = flist_heads_table[chunk_id];
    return head;
}

void
flist_table_set_head(const memid_t chunk_id, struct flist* head)
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
chunk_selector_get_next_free_chunk(void)
{
    // TODO: should check if next chunk is available(contiguous mem), if not, search from 0-th chunk
    uint32 i = 0;
    for(; i < NUM_CHUNKS; i++) {
        print("id:%u flags:%llu\n", chunks_table[i].id, chunks_table[i].flags);
        if(chunks_table[i].flags & CHUNK_FLAGS_FREE) {
            return i;
        }
    }

    my_exit("OUT OF MEMORY\nNO CHUNKS AVAILABLE", __FILE__, __LINE__);
    return INT_NULL;
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
    //                start of mem + mem_cache storage + page offset
    void* addr = (char*) mem_start + mem->self_size + (PAGE_SIZE * page_id) ;
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
debug_mem_get() { return mem; }
void*
debug_mem_get_start_address() { return mem->start; }
uint64
debug_mem_get_size() { return mem->size; }

uint16
debug_mem_get_chunks_count() { return mem->chunks_count; }

struct mem_chunk*
debug_chunks_table_get() { return chunks_table; }

struct flist**
debug_free_list_table_get() { return flist_heads_table; }

void
debug_mem_corrupted()
{
    void* off_bound_mem_start = MEM_GET_BOUND_CHECK_START(void);
    const bool memory_corrupted = check_mem_off_boundaries_writes(off_bound_mem_start, PAGE_SIZE);
    if(memory_corrupted) {
        my_exit("ERROR: MEMORY CORRUPTION DETECTED", __FILE__, __LINE__);
    } else {
        print("%s MEMORY CORRUPTION: PASS %s\n", GREEN, CLEAR);
    }

}