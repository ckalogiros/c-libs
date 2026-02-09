/*
 * Created by ckalo on 12/29/25.
 */
#include "mem_man.h"

#include "mem_log.h"
#include "mem_man_internals.h"
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "sys_utils.h"


void
memory_manager_init()
{
    bool initialized = mem_init();

    if(!initialized) {
        mem_print_error(MEMMAN_ALREADY_INITIALIZED);
        // my_exit("MEMMAN_ALREADY_INITIALIZED", __FILE__, __LINE__);
    }

    // mem_log_print_init_mem();
    // const uint32 alignment = (num_pages > 2) ? PAGE_SIZE * 64 : PAGE_SIZE * 16;
    // // mem_chunk.addr = aligned_alloc(PAGE_SIZE, num_pages * PAGE_SIZE);
    // mem_block.addr = aligned_alloc(alignment, num_pages * PAGE_SIZE);
    // mem_block.size = num_pages * PAGE_SIZE;
    // mem_block.num_pages = num_pages;
    // free_list_init(mem_block.addr, num_pages);
}

void
memory_manager_terminate()
{
    mem_log_print_terminate_mem();
    free(mem_block.addr);
}

struct memhnd_t
mms_allocate_pages(const uint16 num_pages, enum MEM_CHUNK_SELECTOR chunk_selector)
{
    struct memhnd_t hnd = mms_alloc(num_pages, chunk_selector);
    return hnd;
}

// bool
// TEMP_free_list_insert(const pageid_t id)
// {
//     const bool success = mms_release(id);
//     return success;
// }

void
memory_manager_memset(const memid_t id, const uint16 num_pages, const uint64 val)
{
    used_m* um = used_mem_get_used_mem_by_id(id);
    memset(um->addr, val, num_pages * PAGE_SIZE);
}

void* const Temp_get_address_by_id(struct memhnd_t hnd)
{
    // void* const ptr = get_address(id);
    // return ptr;
}
