/*
 * Created by ckalo on 12/29/25.
 */
#include "mem_man.h"

#include "mem_log.h"
#include "mem_man_internals.h"
#include <stdlib.h>

static bool is_mem_initialize = FALSE;

// public
void
memory_manager_init(const uint32 num_pages)
{
    mem_log_print_init_mem();
    mem.addr = aligned_alloc(PAGE_SIZE, num_pages * PAGE_SIZE);
    mem.size = num_pages * PAGE_SIZE;
    mem.num_pages = num_pages;
    free_list_init(mem.addr, num_pages);
    is_mem_initialize = TRUE;
}

void
memory_manager_terminate()
{
    mem_log_print_terminate_mem();
    free(mem.addr);
}

pageid_t
mmu_allocate_pages(const pageid_t num_pages)
{
    const pageid_t id = mmu_alloc(num_pages);
    return id;
}

bool
TEMP_free_list_insert(const pageid_t id)
{
    const bool ret = free_list_insert(id);
    used_mem_remove(id);
    return ret;
}

void* const Temp_get_address_by_id(const pageid_t id)
{
    void* const ptr = get_address(id);
    return ptr;
}

// debug-print
void
mem_print_free_list()
{
    const free_list_t* const free_list = free_list_get_head();
    mem_log_print_free_list(free_list);
}
