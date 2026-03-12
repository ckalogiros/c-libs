/*
 * Created by ckalo on 1/4/26.
 */
#ifndef MEM_MAN_H
#define MEM_MAN_H

#include "my_types.h"
#include "mem_man_interface.h"


#define COLOR_PAGE_FREE (*(uint32_t*)"FREE") // color a page to denote that belongs to free list
#define FREE_LIST_GET_PAGE_COLOR_OFFSET(fnode) ((char*)fnode + sizeof(struct flist))

struct mem;

typedef uint16 chunkid_t;


bool mem_table_init(void);

bool mem_table_destroy(void);

memid_t mem_create(const uint16 num_pages);

struct memhnd_t mem_alloc(const uint16 num_pages, const memid_t mem_id);

bool mem_release(const struct memhnd_t hnd);

/* returns the memory address based on the chunk and page ids */
void* mem_get_addr_by_handle(const struct memhnd_t hnd);

void* mem_temp_alloc_sys_call(const uint32 alignment, const uint32 total_size);

void mem_init_free_list_table(struct mem* mem);


/* returns the next free list node, and updates the list with the next free. */
struct memhnd_t flist_get_next_free_page(const uint16 num_pages, const memid_t mem_id);

/* returns the next free list node, no updates. */
void* flist_peek_next_free_page(memid_t mem_id);

/* returns the free list head of a memory chunk. */
struct flist* mem_table_get_head(const memid_t mem_id);

/* sets the free list head of a memory chunk. */
void mem_table_set_head(const memid_t mem_id, struct flist* head);

memid_t chunk_selector_get_next_free_chunk(void);

pageid_t calc_page_id_from_address(const void* addr, const memid_t mem_id);

void* calc_address_from_page_id(const pageid_t page_id, const memid_t mem_id);

bool check_mem_off_boundaries_writes(void* mem_start, const uint32 byte_size);


/* DEBUG */

struct mem* debug_mem_get_table();

uint32 debug_mem_get_table_count();

void* debug_mem_get_start_address(const memid_t memid);

uint64 debug_mem_get_size(const memid_t memid);

uint16 debug_mem_get_chunks_count(const memid_t memid);

struct mem_chunk* debug_chunks_table_get(const memid_t memid);

void debug_mem_corrupted(const memid_t memid);

bool debug_flist_check_continuity(const memid_t memid);

#endif //MEM_MAN_H
