/*
 * Created by ckalo on 1/4/26.
 */
#ifndef MEM_MAN_H
#define MEM_MAN_H

#include "my_types.h"
#include "mem_man_interface.h"
// #include "mem_structs.h"


#define MEM_GET_DATA_START ((char*)(mem->start + mem->self_size))
#define MEM_GET_CHUNKS_TABLE_START ((char*)(mem->start + sizeof(struct mem)))
#define MEM_GET_BOUND_CHECK_START ((char*)(mem->start + mem->self_size + mem->data_size))


#define FREE_LIST_GET_PAGE_COLOR_START(fnode) ((char*)fnode + sizeof(struct free_list))

typedef uint16 chunkid_t;
typedef uint16 pageid_t;

struct mem* debug_mem_get();
uint16 debug_mem_get_chunks_count();
struct mem_chunk* debug_chunks_table_get();
struct free_list** debug_free_list_table_get();


bool mem_init();

bool mem_free();

struct memhnd_t mem_alloc(pageid_t num_pages, enum MEM_CHUNK_SELECTOR);

bool mem_release(const struct memhnd_t hnd);

/* returns the memory address based on the chunk and page ids */
void* mem_get_addr_by_handle(const struct memhnd_t hnd);


#define flist_table_get_next_free(chunk_id)\
(flist_heads_table[chunk_id] = flist_heads_table[chunk_id]->next)


void mem_init_chunks_table();
void mem_init_free_list_table();


/* returns the next free list node, and updates the list with the next free. */
void* flist_get_next_free_page(const pageid_t num_pages, const memid_t chunk_id);
/* returns the next free list node, no updates. */
void* flist_peek_next_free_page(memid_t chunk_id);
/* returns the free list head of a memory chunk. */
struct free_list* flist_table_get_head(const memid_t chunk_id);
/* sets the free list head of a memory chunk. */
void flist_table_set_head(const memid_t chunk_id, struct free_list* head);

memid_t chunk_selector_get_chunk(enum MEM_CHUNK_SELECTOR chunk_selector);
memid_t calc_page_id_from_address(const void* addr);
void* calc_address_from_page_id(const memid_t page_id);
bool check_mem_off_boundaries_writes(void* mem_start, const uint32 byte_size);



#endif //MEM_MAN_H
