/*
 * Created by ckalo on 12/29/25.
 */
#include "file_sys_interface.h"
#include "mem_man_interface.h"
#include "mem_man_internals.h"
#include "stringc.h"
#include "mem_log/mem_log.h"


void
memman_init(void)
{
    const bool initialized = mem_table_init();

    /* initialize file system files table */
    fs_init_table();

#ifdef MEM_DEBUG_FPRINT_ALL_USE_MEM
    string_init_table();
#endif

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
memman_alloc_create(const uint16 num_pages, const uint32 alignment)
{
    const memid_t mem_id = mem_create(num_pages, alignment);
    return mem_id;
}

struct memhnd_t
memman_alloc_get(const uint16 num_pages, const enum MEM_ID_SELECTOR chunk_selector)
{
    const memid_t mem_id = (memid_t) chunk_selector;
    const struct memhnd_t hnd = mem_alloc(num_pages, mem_id);
    return hnd;
}

bool
memman_alloc_release(const struct memhnd_t hnd)
{
    return mem_release(hnd);
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

void*
memman_temp_alloc_sys_call(const uint32 alignment, const uint32 total_size)
{
    return mem_temp_alloc_sys_call(alignment, total_size);
}
