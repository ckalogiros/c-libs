/*
 * Created by ckalo on 1/4/26.
 */
#ifndef MEM_MAN_H
#define MEM_MAN_H

#include "my_types.h"

#define START_USED_COUNT 128



extern struct mem_chunk mem_block;

void* mem_get_mem();

void* mem_get_address();

uint64 mem_get_total_size();

uint32 mem_get_num_pages();

typedef struct free_list_t {
    void* addr; // the start address of 'this' page
    struct free_list_t* next; // structure to the next free page
    // the start address of 'this', inside free page in memory block
    uint32 self_addr_offset;
    uint32 page_id;
} free_list_t;

const free_list_t* const free_list_get_head();

void free_list_init(void* addr, const pageid_t num_pages);

void* free_list_get_next_free_page(const pageid_t num_pages);

bool free_list_insert(const pageid_t id, const pageid_t num_pages);

typedef struct used_mem_t {
    void* addr;
    uint32 id; // handle
    uint32 num_pages;
} used_mem_t;

used_mem_t* const used_mem_get_used_mem();


used_mem_t* const used_mem_get_used_mem_by_id(const uint32 id);

uint32 used_mem_get_count();

int32 used_mem_get_tail();

pageid_t used_mem_insert(void* addr, pageid_t num_pages);

void used_mem_remove(const pageid_t id);

void* const get_address(const pageid_t id);

pageid_t mms_alloc(pageid_t num_pages);

bool mms_release(const pageid_t id);

#endif //MEM_MAN_H
