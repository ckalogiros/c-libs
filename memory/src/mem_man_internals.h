/*
 * Created by ckalo on 1/4/26.
 */
#ifndef MEM_MAN_H
#define MEM_MAN_H

#include "my_types.h"
#include "mem_man_interface.h"


#define MEM_GET_DATA_START ((char*)(mem->start + mem->self_size))
#define MEM_GET_CHUNKS_TABLE_START ((char*)(mem->start + sizeof(struct mem)))
#define MEM_GET_BOUND_CHECK_START(void) ((char*)(mem->start + mem->self_size + mem->data_size))


#define FREE_LIST_GET_PAGE_COLOR_START(fnode) ((char*)fnode + sizeof(struct flist))

typedef uint16 chunkid_t;
typedef int16 pageid_t;


bool mem_init();

bool mem_free();

struct memhnd_t mem_alloc(pageid_t num_pages, enum MEM_CHUNK_SELECTOR);

bool mem_release(const struct memhnd_t hnd);

/* returns the memory address based on the chunk and page ids */
void* mem_get_addr_by_handle(const struct memhnd_t hnd);

void* mem_temp_alloc_sys_call(const uint32 alignment, const uint32 total_size);


#define flist_table_get_next_free(chunk_id)\
(flist_heads_table[chunk_id] = flist_heads_table[chunk_id]->next)


void mem_init_chunks_table();

void mem_init_free_list_table();


/* returns the next free list node, and updates the list with the next free. */
// void* flist_get_next_free_page(const pageid_t num_pages, const memid_t chunk_id);
struct memhnd_t flist_get_next_free_page(const pageid_t num_pages, const memid_t chunk_id);

/* returns the next free list node, no updates. */
void* flist_peek_next_free_page(memid_t chunk_id);

/* returns the free list head of a memory chunk. */
struct flist* flist_table_get_head(const memid_t chunk_id);

/* sets the free list head of a memory chunk. */
void flist_table_set_head(const memid_t chunk_id, struct flist* head);

memid_t chunk_selector_get_chunk(enum MEM_CHUNK_SELECTOR chunk_selector);

memid_t chunk_selector_get_next_free_chunk(void);

memid_t calc_page_id_from_address(const void* addr);

void* calc_address_from_page_id(const memid_t page_id);

bool check_mem_off_boundaries_writes(void* mem_start, const uint32 byte_size);


/* DEBUG */
struct mem* debug_mem_get();
void* debug_mem_get_start_address();
uint64 debug_mem_get_size();

uint16 debug_mem_get_chunks_count();

struct mem_chunk* debug_chunks_table_get();

struct flist** debug_free_list_table_get();

void debug_mem_corrupted();

#endif //MEM_MAN_H
