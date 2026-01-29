/*
 * Created by ckalo on 1/4/26.
 *
 * todo:
 *  reserve 4 PAGES(4096) for 1024 elements of type {uint64, uint32, uint32}.
 *  4 PAGES used_mem buffer is 1024 page allocations to be addressed with each
 *  requested allocation be 1 PAGE size. In case of more that 1 PAGE allocation
 *  request, the 1024 grows.
 */

#include "mem_man_internals.h"
#include "mem_log.h"
#include "sys_utils.h"
#include <stdio.h>

#define GREATER_THAN(a, b) (((a) > (b)) ? (a) : (b))
#define LESS_THAN(a, b)    (((a) < (b)) ? (a) : (b))

struct mem_t mem = { .addr = NULL, .size = 0, .num_pages = 0 };

void* mem_get_mem() { return &mem; }

void* mem_get_address() { return mem.addr; }

uint64 mem_get_total_size() { return mem.size; }

uint32 mem_get_num_pages() { return mem.num_pages; }

/*
 * free list
 */

free_list_t* free_list_head = NULL;

const free_list_t* const free_list_get_head() { return free_list_head; }

void free_list_init(void* addr, const pageid_t num_pages)
{
    free_list_head          = (free_list_t*)addr;
    free_list_head->addr    = addr;
    free_list_head->next    = NULL;
    free_list_head->page_id = 0;
    // use allocated memory block to store the free list nodes.
    // each node is stored in equally sized page of the memory block
    void* page_start       = addr;
    free_list_t* prev_node = free_list_head;
    for (uint32 i = 1; i < num_pages; i++) {
        page_start        = (addr + (PAGE_SIZE * (i)));
        free_list_t* node = (free_list_t*)page_start;
        node->addr        = page_start; // pointer storage same with pointing address
        node->next        = NULL;
        node->page_id     = (page_start - addr) / PAGE_SIZE;
        prev_node->next   = node;
        prev_node         = node;
    }
}

void* free_list_get_next_free_page(const pageid_t num_pages)
{
    const free_list_t* free = free_list_head;
    uint16 cnt              = 0;
    while (cnt < num_pages) {
        if (!free_list_head) { // check if free memory is available
            mem_print_error(NO_FREE_MEMORY);
            my_exit("free_list_get_next_free_page", __FILE__, __LINE__);
        }

        free_list_head = free_list_head->next;
        cnt++;
    }
    // mem_log_print_list_node("remove", free);
    return free->addr;
}

bool free_list_insert(const pageid_t id)
{
    void* addr = mem.addr + (id * PAGE_SIZE);

    if (free_list_head == NULL) {
        free_list_head          = (free_list_t*)addr;
        free_list_head->addr    = addr;
        free_list_head->next    = NULL;
        free_list_head->page_id = (addr - mem.addr) / PAGE_SIZE;
        // mem_log_print_list_node("insert", free_list_head);
        return TRUE;
    }
    // else: walk through the list, insert at the list's end
    free_list_t* node = free_list_head;
    while (node != NULL) {
        if (node->next == NULL) {
            node->next          = (free_list_t*)addr;
            node->next->addr    = addr;
            node->next->next    = NULL;
            node->next->page_id = (addr - mem.addr) / PAGE_SIZE;
            // mem_log_print_list_node("insert", node->next);
            return TRUE;
        }
        node = node->next;
    }
    return FALSE;
}

/*
 * used memory
 */
used_mem_t used_mem[START_USED_COUNT];
uint32 used_mem_count = 0;        // number of used elements
int32 used_mem_tail   = INT_NULL; // index of the last element in use. Ex. [1, 0,
// 0, 1, 0, 0]: tail should be at index 3

used_mem_t* const used_mem_get_used_mem() { return used_mem; }

const uint32 used_mem_get_count() { return used_mem_count; }

const int32 used_mem_get_tail() { return used_mem_tail; }

const pageid_t used_mem_insert(void* addr, const pageid_t num_pages)
{
    // calculate the index of the start of the page from pointer adresses
    const void* start_addr = mem_get_address();
    uint32 id              = 0;
    if (addr == start_addr) id = 0;
    else id = (pageid_t)(addr - start_addr) / PAGE_SIZE;
    used_mem[id].addr      = addr;
    used_mem[id].id        = id;
    used_mem[id].num_pages = num_pages;
    used_mem_count++;
    if (id < used_mem_count) used_mem_tail++;
    // mem_log_print_used_mem_buf("insert", &used_mem[id]);
    return used_mem[id].id;
}

void used_mem_remove(const pageid_t id)
{
    // mem_log_print_used_mem_buf("remove", &used_mem[id]);
    used_mem[id].addr      = 0x0;
    used_mem[id].num_pages = 0;
    used_mem_count--;
}

/*
 * functionality
 */
void* const get_address(const pageid_t id) { return used_mem[id].addr; }

const pageid_t mmu_alloc(const pageid_t num_pages)
{
    if (!mem.addr) {
        // check if memory manager is initialized
        mem_print_error(NO_MEMORY_MANAGER_INIT);
        my_exit("mmu_alloc", __FILE__, __LINE__);
    }

    void* addr        = free_list_get_next_free_page(num_pages);
    const pageid_t id = used_mem_insert(addr, num_pages);
    return id;
}
