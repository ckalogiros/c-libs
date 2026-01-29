/*
 * Created by ckalo on 1/4/26.
 */
#ifndef MEM_MAN_H
#define MEM_MAN_H

#include "my_types.h"

#define START_USED_COUNT 128

enum MEMORY_ERROR_CODE {
    NO_MEMORY_MANAGER_INIT = BIT_MASK_FROM_UINT(1),
    NO_FREE_MEMORY         = BIT_MASK_FROM_UINT(2),
};

struct mem_t {
    void* addr;
    uint64 size;
    uint32 num_pages;
};

extern struct mem_t mem;
void* mem_get_mem();
void* mem_get_address();
uint64 mem_get_total_size();
uint32 mem_get_num_pages();

typedef struct free_list_t {
    void* addr;               // the start address of 'this' page
    struct free_list_t* next; // structure to the next free page
    // the start address of 'this', inside free page in memory block
    uint32 self_addr_offset;
    uint32 page_id;
} free_list_t;

const free_list_t* const free_list_get_head();
void free_list_init(void* addr, const pageid_t num_pages);
void* free_list_get_next_free_page(const pageid_t num_pages);
bool free_list_insert(const pageid_t id);

typedef struct used_mem_t {
    void* addr;
    uint32 id; // handle
    uint32 num_pages;
} used_mem_t;

used_mem_t* const used_mem_get_used_mem();
const uint32 used_mem_get_count();
const int32 used_mem_get_tail();
const pageid_t used_mem_insert(void* addr, const pageid_t num_pages);
void used_mem_remove(const pageid_t id);

void* const get_address(const pageid_t id);
const pageid_t mmu_alloc(pageid_t num_pages);

#endif //MEM_MAN_H
