/*
 * Created by ckalo on 12/29/25.
 */
#include "file_sys_interface.h"
#include "mem_man_interface.h"
#include "mem_man_internals.h"
#include "mem_log/mem_log.h"


void
memman_init(void)
{
    const bool initialized = mem_table_init();

    /* initialize file system files table */
    fs_init_table();

// #ifdef MEM_DEBUG_FPRINT_ALL_USE_MEM
//     string_init_table();
// #endif

    if(!initialized)
        mem_print_error(MEMMAN_ALREADY_INITIALIZED);
}

void
memman_terminate(void)
{
    mem_log_print_terminate_mem();
    const bool success = mem_table_destroy();
    if(!success)
        mem_print_error(FREE_MEMORY_FAILED);
}

memid_t
memman_allocate_block(const uint16 num_pages, const uint32 alignment, const char* info)
{
    const memid_t mem_id = mem_allocate_block(num_pages, alignment, info);
    return mem_id;
}

memid_t
memman_deallocate_block(const memid_t memid)
{
    const memid_t mem_id = mem_deallocate_block(memid);
    return mem_id;
}

struct memhnd_t
memman_page_request(const uint16 num_pages, const memid_t memid)
{
    const struct memhnd_t hnd = mem_page_request(num_pages, memid);
    return hnd;
}

bool
memman_page_release(const struct memhnd_t hnd)
{
    return mem_page_release(hnd);
}

void*
memman_get_mem_start_by_memid(const memid_t memid)
{
    return mem_get_mem_start_by_memid(memid);
}


void*
memman_get_alloc_mem_by_handle(const struct memhnd_t handle)
{
    return mem_get_addr_by_handle(handle);
}

