/*
 * Created by ckalo on 1/17/26.
 */

#include "slab_allocator_log.h"
#include "logger.h"
#include "slab_allocator_internals.h"


void
print_mem_cache_struct(const struct mem_cache* const mem_cache)
{
    print("%s-- mem_cache struct --%s\n", ry, CLEAR);
    print("  name:%s\n", mem_cache->name.buf);
    print("  slab_size:%u\n", mem_cache->slab_size);
    print("  slabs_count:%u\n", mem_cache->slabs_count);
    print("  slabs_capacity:%u\n", mem_cache->slabs_capacity);
    print("  slabs_num_pages:%u\n", mem_cache->slabs_num_pages);
    print("  type_size:%u\n", mem_cache->type_size);
    print("  is_active:%u\n", mem_cache->is_active);
    print("  slabs_free_list:%x\n", REDUCE_ADDR(mem_cache->slabs_free_list, F5));
    print("  mem_handle:%u\n", mem_cache->mem_handle);
    print("      chunk_id:%u\n", mem_cache->mem_handle.chunk_id);
    print("      page_id:%u\n", mem_cache->mem_handle.page_id);
    print("      num_pages:%u\n", mem_cache->mem_handle.num_pages);
    print("\n");
}

void
print_mem_cache_create(const uint32 slab_size, const uint8 slabs_num_pages,
                       struct mem_cache* mcahche, void* slabs_addr)
{
    // debug-print
    print("\n%s-- Create Mem Cache --%s\n"
          "struct mem_cache: %s%u%s bytes\n"
          "slab size:        %s%u%s bytes\n"
          "slab pages:       %s%u%s\n"
          "mem_cache addr: %s%x%s\n"
          "slabs addr:     %s%x%s\n",
          YELLOW, CLEAR,
          YELLOW, sizeof(struct mem_cache), CLEAR,
          YELLOW, slab_size, CLEAR,
          YELLOW, slabs_num_pages, CLEAR,
          CYAN, REDUCE_ADDR(mcahche, F6), CLEAR,
          CYAN, REDUCE_ADDR(slabs_addr, F6), CLEAR
    );
    print("%s-----------------------%s\n", YELLOW, CLEAR);
}

void
print_slabs_free_list(struct slab_flist* slabs_free_list)
{
    print("%s-- SLABS FREE LIST --%s\n", YELLOW, CLEAR);

    struct slab_flist* list = slabs_free_list;
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
        case SLAB_ALLOC_MEMCACHE_NOT_FOUND: {
            print("%sERROR:\n"
                  "Slab allocator: mem_cache not found%s\n",
                  RED, CLEAR
            );
            break;
        }
    }
}
