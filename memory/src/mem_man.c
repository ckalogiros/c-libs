/*
 * Created by ckalo on 12/29/25.
 */
#include "mem_man_interface.h"
#include "mem_man_internals.h"
#include "logger.h"
#include "mem_log/mem_log.h"


void
memman_init(void)
{
    bool initialized = mem_init();

    if(!initialized)
        mem_print_error(MEMMAN_ALREADY_INITIALIZED);
}

void
memman_terminate(void)
{
    mem_log_print_terminate_mem();
    bool success = mem_free();
    if(!success)
        mem_print_error(FREE_MEMORY_FAILED);
}

struct memhnd_t
memman_alloc_pages(const uint16 num_pages, enum MEM_CHUNK_SELECTOR chunk_selector)
{
    struct memhnd_t hnd = mem_alloc(num_pages, chunk_selector);
    return hnd;
}

bool
memman_release(const struct memhnd_t hnd)
{
    return mem_release(hnd);
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
