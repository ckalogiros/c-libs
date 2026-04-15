/*
 * Created by ckalo on 1/4/26.
 */
#ifndef MEMORY_INTERNALS_H
#define MEMORY_INTERNALS_H

#include "my_types.h"
#include "mem_man_interface.h"


#define COLOR_PAGE_FREE (*(uint32_t*)"FREE") // color a page to denote that belongs to free list
#define FREE_LIST_GET_PAGE_COLOR_OFFSET(fnode) ((char*)fnode + sizeof(struct flist))

#define MEM_DEBUG_FPRINT_ALL_USE_MEM

struct mem_block;

typedef uint16 chunkid_t;


bool mem_table_init(void);

bool mem_table_destroy(void);

memid_t mem_allocate_block(const uint16 num_pages, const uint32 alignment, const char* info);

memid_t mem_deallocate_block(const memid_t memid);

struct memhnd_t mem_page_request(const uint16 num_pages, const memid_t mem_id);

bool mem_page_release(const struct memhnd_t hnd);

/* returns the memory chunk address(mem_table mem_block id) start */
void* mem_get_mem_start_by_memid(const memid_t memid);

/* returns the memory chunk address(mem_table mem_block id) at page id index offset */
void* mem_get_addr_by_handle(const struct memhnd_t hnd);

void mem_init_free_list_table(struct mem_block* mem);


/* returns the next free list node, and updates the list with the next free. */
struct memhnd_t flist_get_next_free_page(const uint16 num_pages, const memid_t mem_id);

/* returns the free list head of a memory chunk. */
struct flist* mem_table_get_head(const memid_t mem_id);

/* sets the free list head of a memory chunk. */
void mem_table_set_head(const memid_t mem_id, struct flist* head);

memid_t chunk_selector_get_next_free_chunk(void);

pageid_t calc_page_id_from_address(const void* addr, const memid_t mem_id);

void* calc_address_from_page_id(const pageid_t page_id, const memid_t mem_id);



/* DEBUG */

struct mem_block* debug_mem_get_table();

struct mem_block* mem_get_byid(const memid_t mem_id);

uint32 debug_mem_get_table_count();

void* debug_mem_get_start_address(const memid_t memid);

uint64 debug_mem_get_size(const memid_t memid);

/* returns the next free list node, no updates. */
void* flist_peek_next_free_page(memid_t mem_id);

bool debug_mem_freeing_already_freed_pages(const struct memhnd_t hnd);

void debug_mem_corrupted(const memid_t memid, const bool silent);

bool check_mem_off_boundaries_writes(void* mem_start, const uint32 byte_size);

bool debug_flist_check_continuity(const memid_t memid);

#endif //MEMORY_INTERNALS_H
