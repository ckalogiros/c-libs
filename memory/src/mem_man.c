/*
 * Created by ckalo on 12/29/25.
 */
#include "mem_fprint_debug.h"
#include "mem_man_interface.h"
#include "mem_log.h"

void
memman_terminate(void)
{
    mem_log_print_terminate_mem();
    enum MEMORY_DEBUG code = mem_pool_destroy();
    mem_print_error(code);
}

memid_t
memman_allocate_block(const uint16 num_pages, const uint32 alignment, const char* info)
{
    const memid_t mem_id = mem_create_inst(num_pages, alignment, info);
    return mem_id;
}

memid_t
memman_deallocate_block(const memid_t memid)
{
    const memid_t mem_id = mem_destroy_inst(memid);
    return mem_id;
}

struct memhnd_t
memman_page_request(const uint16 num_pages, const memid_t memid)
{
    const struct memhnd_t hnd = mem_inst_request_pages(num_pages, memid);
    mem_fprint_append_mem_block(hnd.mem_id);
    // mem_fprint_append_mem_block_poison(hnd.mem_id);
    return hnd;
}

bool
memman_page_release(const struct memhnd_t hnd)
{
    return mem_inst_release_pages(hnd);
}

void*
memman_get_alloc_mem_by_handle(const struct memhnd_t handle)
{
    return mem_inst_get_address(handle);
}

// /// TODO: delete. Should call only "memman_get_alloc_mem_by_handle()"
// void*
// memman_get_mem_start_by_memid(const memid_t memid)
// {
//     return mem_inst_get_start_address(memid);
// }