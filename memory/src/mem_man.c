/*
 * Created by ckalo on 12/29/25.
 */
#include "mem_man.h"

#include "mem_log.h"
#include "mem_man_internals.h"
#include <stdlib.h>
#include <string.h>

#include "sys_utils.h"


void
memory_manager_init(const uint32 num_pages)
{
    if(mem_block.addr) {
        mem_print_error(MEMMAN_ALREADY_INITIALIZED);
        // my_exit("MEMMAN_ALREADY_INITIALIZED", __FILE__, __LINE__);

        mem_log_print_init_mem();
        const uint32 alignment = (num_pages > 2) ? PAGE_SIZE * 64 : PAGE_SIZE * 16;
        // mem_block.addr = aligned_alloc(PAGE_SIZE, num_pages * PAGE_SIZE);
        mem_block.addr = aligned_alloc(alignment, num_pages * PAGE_SIZE);
        mem_block.size = num_pages * PAGE_SIZE;
        mem_block.num_pages = num_pages;
        free_list_init(mem_block.addr, num_pages);
    }

    mem_log_print_init_mem();
    const uint32 alignment = (num_pages > 2) ? PAGE_SIZE * 64 : PAGE_SIZE * 16;
    // mem_block.addr = aligned_alloc(PAGE_SIZE, num_pages * PAGE_SIZE);
    mem_block.addr = aligned_alloc(alignment, num_pages * PAGE_SIZE);
    mem_block.size = num_pages * PAGE_SIZE;
    mem_block.num_pages = num_pages;
    free_list_init(mem_block.addr, num_pages);
}

void
memory_manager_terminate()
{
    mem_log_print_terminate_mem();
    free(mem_block.addr);
}

pageid_t
mms_allocate_pages(const pageid_t num_pages)
{
    const pageid_t id = mms_alloc(num_pages);
    return id;
}

bool
TEMP_free_list_insert(const pageid_t id)
{
    // const bool ret = free_list_insert(id);
    // used_mem_remove(id);
    const bool success = mms_release(id);
    return success;
}

void
memory_manager_memset(const uint32 id, const uint32 num_pages, const uint64 val)
{
    used_mem_t* um = used_mem_get_used_mem_by_id(id);
    memset(um->addr, val, num_pages * PAGE_SIZE);
}

void* const Temp_get_address_by_id(const pageid_t id)
{
    void* const ptr = get_address(id);
    return ptr;
}

// debug-print
// void
// mem_print_free_list()
// {
//     const free_list_t* const free_list = free_list_get_head();
//     mem_log_print_free_list(free_list);
// }
