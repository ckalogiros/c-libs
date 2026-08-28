/*
 * Created by ckalo on 1/4/26.
 */
#ifndef MEMORY_INTERNALS_H
#define MEMORY_INTERNALS_H

#include "my_types.h"
#include "my_macros.h"
#include "mem_man_structs.h"
#include "mem_man_interface.h"


#define COLOR_PAGE_FREE HACK_LITERAL_STR_TO_NUM("FREE", uint32_t) /// color a page to denote that belongs to free list
#define FREE_LIST_GET_PAGE_COLOR_OFFSET(fnode) ((char*)fnode + sizeof(struct flist))

#define MEM_DEBUG_FPRINT_ALL_USE_MEM
#define BLOCK_POISON_DATA_SIZE 64 /// poison data = 32 bytes

// #define MEM_POISON_PATTERN 0x0abccbcabeffdfdac
#define MEM_POISON_PATTERN HACK_LITERAL_STR_TO_NUM("|POISON|", uint64)

enum MEMORY_DEBUG {
    MALLOC_FAILED_TO_ALLOC_MEMORY = BIT_MASK_FROM_UINT(0),
    NO_MEMORY_MANAGER_INIT = BIT_MASK_FROM_UINT(1),
    NO_FREE_MEMORY = BIT_MASK_FROM_UINT(2),
    MEMMAN_ALREADY_INITIALIZED = BIT_MASK_FROM_UINT(3),
    ALLOC_NOT_CONTIGUOUS_MEM = BIT_MASK_FROM_UINT(4),
    MEMORY_IS_IN_USE = BIT_MASK_FROM_UINT(5),
    FREE_MEMORY_FAILED = BIT_MASK_FROM_UINT(6),
    FREE_PAGE_ALREADY_FREED = BIT_MASK_FROM_UINT(7),
    MEMORY_CORRUPTED = BIT_MASK_FROM_UINT(7),
    MEMORY_TERMINATE_SUCCESS = BIT_MASK_FROM_UINT(8),
};


/***************************************************************************************************
 * FUNCTIONS FROM INTERFACE
 **************************************************************************************************/
enum MEMORY_DEBUG mem_pool_destroy(void);

void mem_pool_realloc(); /// TODO: implement

memid_t mem_create_inst(const uint16 num_pages, const uint32 alignment, const char* info);

memid_t mem_destroy_inst(const memid_t memid);

struct memhnd_t mem_inst_request_pages(const uint16 num_pages, const memid_t mem_id);

bool mem_inst_release_pages(const struct memhnd_t hnd);

// struct memhnd_t mem_realloc(const pageid_t num_pages, const memid_t mem_id);

/* returns the memory chunk address(mem_table mem_block id) at page id index offset */
void* mem_inst_get_address(const struct memhnd_t hnd);





/***************************************************************************************************
 * PRIVATE
 **************************************************************************************************/
/* returns the free list head of a memory chunk. */
void mem_init_free_list_table(struct mem_block* mem);

/* returns the next free list node, and updates the list with the next free. */
struct memhnd_t flist_request_free_page(const uint16 num_pages, const memid_t mem_id);

struct flist* flist_get_head(const memid_t mem_id);

/* sets the free list head of a memory chunk. */
void flist_set_head(const memid_t mem_id, struct flist* head);

pageid_t calc_page_id_from_address(const void* addr, const memid_t mem_id);

void* calc_address_from_page_id(const pageid_t page_id, const memid_t mem_id);



/***************************************************************************************************
 * DEBUG
 **************************************************************************************************/
struct mem_block* mem_pool_get();

struct mem_block* debug_mem_get_table();

struct mem_block* mem_pool_get_block(const memid_t mem_id);

uint32 debug_mem_get_table_count();

void* debug_mem_get_start_address(const memid_t memid);

uint64 debug_mem_get_size(const memid_t memid);

/* returns the next free list node, no updates. */
void* flist_peek_next_free_page(memid_t mem_id);

bool debug_mem_freeing_already_freed_pages(const struct memhnd_t hnd);

void debug_mem_corrupted(const memid_t memid, const bool silent);

bool check_mem_off_boundaries_writes(void* mem_start, const uint32 size);

bool debug_flist_check_continuity(const memid_t memid);

#endif //MEMORY_INTERNALS_H
