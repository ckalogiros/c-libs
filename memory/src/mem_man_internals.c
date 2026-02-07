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
#include "internal/mem_structs.h"
#include "logger.h"
#include <stdio.h>


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
 * so is plug&play to find the appropriate total mem_block size a program needs.
 **************************************************************************************************/


struct mem_block mem_block = {.addr = NULL, .size = 0, .num_pages = 0};

void*
mem_get_mem() { return &mem_block; }

void*
mem_get_address() { return mem_block.addr; }

uint64
mem_get_total_size() { return mem_block.size; }

uint32
mem_get_num_pages() { return mem_block.num_pages; }


/***************************************************************************************************
 * free list
 * adds a free list node per PAGE, storing the free list node
 * in the memory PAGE block.
 **************************************************************************************************/
free_list_t* free_list_head = NULL;

const free_list_t* const free_list_get_head() { return free_list_head; }

void
free_list_init(void* addr, const pageid_t num_pages)
{
    free_list_head = (free_list_t*) addr;
    free_list_head->addr = addr;
    free_list_head->next = NULL;
    free_list_head->page_id = 0;
    // use allocated memory block to store the free list nodes.
    // each node is stored in equally sized page of the memory block
    void* page_start = addr;
    free_list_t* prev_node = free_list_head;
    for(uint32 i = 1; i < num_pages; i++) {
        page_start = (addr + (PAGE_SIZE * (i)));
        free_list_t* node = (free_list_t*) page_start;
        node->addr = page_start; // pointer storage same with pointing address
        node->next = NULL;
        node->page_id = (page_start - addr) / PAGE_SIZE;
        prev_node->next = node;
        prev_node = node;
    }
}

void*
free_list_get_next_free_page(const pageid_t num_pages)
{
    const free_list_t* free = free_list_head;
    const free_list_t* prev = free_list_head;
    uint16 cnt = 0;
    while(cnt < num_pages) {
        CHECK_MEM_NULL(free_list_head, NO_FREE_MEMORY,
                       "free_list_get_next_free_page");

        CHECK_CONTIGUOUS_FREE_MEM(prev, PAGE_SIZE,
                                  free_list_head,
                                  ALLOC_NOT_CONTIGUOUS_MEM,
                                  "free_list_get_next_free_page");

        prev = free_list_head;
        free_list_head = free_list_head->next;
        cnt++;
    }
    mem_log_print_list_node("remove", free);
    return free->addr;
}

bool
free_list_insert(const pageid_t id, pageid_t num_pages)
{
    // get the address based on the page index id
    void* addr = mem_block.addr + (id * PAGE_SIZE);

    // case there is no free memory
    if(free_list_head == NULL) {
        free_list_head = (free_list_t*) addr;
        free_list_head->addr = addr;
        free_list_head->page_id = (addr - mem_block.addr) / PAGE_SIZE;


        free_list_t* fnode = free_list_head;
        num_pages--;

        while(num_pages > 0) {
            addr = (void*) (((uint8*) addr) + PAGE_SIZE); // increment to next page
            fnode->next = (free_list_t*) addr;
            fnode->next->page_id = (addr - mem_block.addr) / PAGE_SIZE;
            fnode->next->addr = addr;

            fnode = fnode->next;
            num_pages--;
        }

        fnode->next = NULL;
        // mem_log_print_list_node("insert", free_list_head);
        return TRUE;
    }

    // case freed addr is before the free list head
    if(((int64) addr - (int64) free_list_head) < 0) {
        free_list_t* temp = free_list_head; // so we don't loose ->next

        free_list_head = (free_list_t*) addr;
        free_list_head->addr = addr;
        free_list_head->page_id = (addr - mem_block.addr) / PAGE_SIZE;

        free_list_t* fnode = free_list_head;
        num_pages--;

        while(num_pages > 0) {
            addr = (void*) (((uint8*) addr) + PAGE_SIZE); // increment to next page
            fnode->next = (free_list_t*) addr;
            fnode->next->page_id = (addr - mem_block.addr) / PAGE_SIZE;
            fnode->next->addr = addr;

            fnode = fnode->next;
            num_pages--;
        }

        fnode->next = temp;
        return TRUE;
    }

    // else: walk through the list, find the free elem just before the address
    free_list_t* fnode = free_list_head;
    while(fnode != NULL) {
        // if(fnode->next && fnode->next > addr){
        if(fnode->next == NULL || (uint64*) fnode->next > (uint64*) addr) {
            free_list_t* temp = fnode->next; // so we don't loose ->next
            while(num_pages > 0) {
                fnode->next = (free_list_t*) addr;
                fnode->next->page_id = (addr - mem_block.addr) / PAGE_SIZE;
                fnode->next->addr = addr;

                fnode = fnode->next;
                num_pages--;
                addr = (void*) (((uint8*) addr) + PAGE_SIZE); // increment to next page
            }

            fnode->next = temp;
            return TRUE;
        }

        fnode = fnode->next;
    }

    return FALSE;
}


/***************************************************************************************************
 * used memory
 * is a buffer registering only calls to mms_alloc().
 * That means each element in used_mem buffer may be 1-many PAGES,
 * compare to free_list that registers a free element per PAGE.
 * On free used memory, one element of used_mem is "release"
 * where as for the free_list we run a loop to set free each
 * individual page.
 **************************************************************************************************/
used_mem_t used_mem[START_USED_COUNT];
uint32 used_mem_count = 0; // number of used elements
int32 used_mem_tail = INT_NULL; // index of the last element in use. Ex. [1, 0,
// 0, 1, 0, 0]: tail should be at index 3

used_mem_t*
const used_mem_get_used_mem() { return used_mem; }

used_mem_t*
const used_mem_get_used_mem_by_id(const uint32 id) { return &used_mem[id]; }

uint32
used_mem_get_count() { return used_mem_count; }

int32
used_mem_get_tail() { return used_mem_tail; }

pageid_t
used_mem_insert(void* addr, const pageid_t num_pages)
{
    // calculate the index of the start of the page from pointer adresses
    const void* start_addr = mem_get_address();
    uint32 id = 0;
    if(addr == start_addr) id = 0;
    else id = (pageid_t) (addr - start_addr) / PAGE_SIZE;

    if(id >= START_USED_COUNT) {
        my_exit("ERROR: used_mem exceeds START_USED_COUNT", __FILE__, __LINE__);
    }

    used_mem[id].addr = addr;
    used_mem[id].id = id;
    used_mem[id].num_pages = num_pages;
    used_mem_count++;
    if(id < used_mem_count) used_mem_tail++;
    mem_log_print_used_mem_buf("insert", &used_mem[id]);
    return used_mem[id].id;
}


void
used_mem_remove(const pageid_t id)
{
    used_mem[id].addr = NULL;
    used_mem[id].num_pages = 0;
    used_mem_count--;

    if(id >= used_mem_tail)
        while(used_mem_tail >= INT_NULL && used_mem[used_mem_tail].addr == NULL)
            used_mem_tail--;


    mem_log_print_used_mem_buf("remove", &used_mem[id]);
}

/*
 * functionality
 */
void* const get_address(const pageid_t id) { return used_mem[id].addr; }

pageid_t
mms_alloc(const pageid_t num_pages)
{
    CHECK_MEM_NULL(mem_block.addr, NO_MEMORY_MANAGER_INIT, "mms_alloc");

    void* addr = free_list_get_next_free_page(num_pages);
    mem_log_print_free_list(free_list_head);
    // print("%s allocated address:%x\n", RED, REDUCE_ADDR(addr, F6), CLEAR);

    const pageid_t id = used_mem_insert(addr, num_pages);
    return id;
}

bool
mms_release(const pageid_t id)
{
    // cache data
    void* start_addr = used_mem[id].addr;
    const pageid_t num_pages = used_mem[id].num_pages;
    if(num_pages == 0) {
        mem_print_error(MEMORY_IS_IN_USE);
        return FALSE;
        // my_exit("mms_release", __FILE__, __LINE__);
    }
    used_mem_remove(id);

    const bool success = free_list_insert(id, num_pages);
    mem_log_print_free_list(free_list_head);

    return success;
}
