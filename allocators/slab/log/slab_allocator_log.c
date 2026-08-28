/*
 * Created by ckalo on 1/17/26.
 */

#include "slab_allocator_log.h"
#include "logger.h"
#include "slab_allocator_internals.h"
#include "sys_utils.h"


void
print_mem_cache_struct(const struct slab_allocator_mcache* const mem_cache)
{
    print("%s-- mem_cache struct --%s\n", ry, CLEAR);
    print("  name:%s\n", mem_cache->name);
    print("  slab_size:%u\n", mem_cache->slab_size);
    print("  slabs_count:%u\n", mem_cache->slabs_count);
    print("  slabs_capacity:%u\n", mem_cache->slabs_capacity);
    print("  slabs_num_pages:%u\n", mem_cache->slabs_num_pages);
    print("  is_active:%u\n", mem_cache->is_active);
    print("  slabs_free_list:%x\n", REDUCE_ADDR(mem_cache->slabs_free_list, F5));
    print("  mem_handle:%u\n", mem_cache->mem_handle);
    print("      mem_id:%u\n", mem_cache->mem_handle.mem_id);
    print("      page_id:%u\n", mem_cache->mem_handle.page_id);
    print("      num_pages:%u\n", mem_cache->mem_handle.num_pages);
    print("\n");
}

void
print_mem_cache_create(const struct slab_allocator_mcache* const mem_cache)
{
    // debug-print
    print("\n%s-- Create Mem Cache --%s\n", YELLOW, CLEAR);
    print("  name: %s\n", mem_cache->name);
    print("  slab_size:      %s%u%s\n", YELLOW, mem_cache->slab_size, CLEAR);
    print("  slabs_count:    %s%u%s\n", YELLOW, mem_cache->slabs_count, CLEAR);
    print("  slabs_capacity: %s%u%s\n", YELLOW, mem_cache->slabs_capacity, CLEAR);
    print("  slabs_num_pages:%s%u%s\n", YELLOW, mem_cache->slabs_num_pages, CLEAR);
    print("  is_active:      %s%u%s\n", YELLOW, mem_cache->is_active, CLEAR);
    print("  slabs_free_list:%s%x%s\n", YELLOW, REDUCE_ADDR(mem_cache->slabs_free_list, F6), CLEAR);
    print("%s-----------------------%s\n", YELLOW, CLEAR);
}

void
print_slabs_free_list(struct flist* slabs_free_list)
{
    print("%s-- SLABS FREE LIST --%s\n", YELLOW, CLEAR);

    struct flist* list = slabs_free_list;
    uint32 slabs_count = 0;
    while(list) {
        print("[%s%u%s]"
              " addr:%s%x%s"
              " next:%s%x%s"
              "\n",
              CYAN, slabs_count, CLEAR,
              CYAN, REDUCE_ADDR(list, F6), CLEAR,
              CYAN, REDUCE_ADDR(list->next, F6), CLEAR
        );
        list = list->next;
        slabs_count++;
    }
    print("slabs count: %s%u%s\n", YELLOW, slabs_count, CLEAR);
    print("%s---------------------%s\n\n", YELLOW, CLEAR);
}

// ERROR handling
void
slab_allocator_print_error(const enum SLAB_ALLOCATOR_ERROR_CODE code)
{
    switch(code) {
        case SLAB_ALLOCATOR_MEMCACHE_NOT_FOUND: {
            print("%sERROR:\n"
                  "Slab allocator: mem_cache not found%s\n",
                  RED, CLEAR
            );
            my_exit("", __FILE__, __LINE__);
        }
        case SLAB_ALLOCATOR_MEMCACHE_NO_INIT: {
            print("%sERROR:\n"
                  "Slab allocator: mem_cache_table not initialized.\nForget to call \"mem_chache_table_init()\"??%s\n",
                  RED, CLEAR
            );
            break;
        }
    }
}
