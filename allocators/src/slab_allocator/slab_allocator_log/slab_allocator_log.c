/*
 * Created by ckalo on 1/17/26.
 */

#include "slab_allocator_log.h"
#include "logger.h"
#include "slab_allocator_internals.h"



void print_mem_cache_list()
{
    // cache data struct
    struct mem_cache_list* list = mem_cache_get_list();
    struct mem_cache_node* list_node = list->head;
    print("-- %sMem Cache List Nodes%s --\n", ry, CLEAR);

    uint32 k = 0;
    while (list_node) {
        print("  addr:%s%x%s ", CYAN, REDUCE_ADDR(list_node, F6), CLEAR, CYAN);
        print("  name:%s\n", list_node->mem_cache.name.buf);
        print("  slab_size:%u\n", list_node->mem_cache.slab_size);
        print("  type_size:%u\n", list_node->mem_cache.type_size);
        print("  num_elems:%u\n", list_node->mem_cache.slab_size/list_node->mem_cache.type_size);
        list_node = list_node->next;
      k++;
    }
    print("mem caches count: %s%u%s\n", YELLOW, list->count, CLEAR);
    print("\n");
}

void print_mem_cache_struct(const struct mem_cache* const mem_cache)
{
    print("%s-- mem_cache struct --%s\n", ry, CLEAR);
    print("  name:%s\n", mem_cache->name.buf);
    print("  name len:%u\n", mem_cache->name.len);
    print("  slab_size:%u\n", mem_cache->slab_size);
    print("\n");
}

void print_mem_cache_create(const uint32 slab_size, const uint8 slabs_num_pages,
                            struct mem_cache_node* cache_node, void* free_mem_start_addr) {
    // debug-print
    print("\n%s-- Create Mem Cache --%s\n"
          "mem cache size: %s%u%s bytes\n"
          "slab size:      %s%u%s bytes\n"
          "slab pages:     %s%u%s\n"
          "mem_cache node addr: %s%x%s\n"
          "mem_cache addr:      %s%x%s\n"
          "dif:                 %s%x%s bytes\n",
          YELLOW, CLEAR,
          YELLOW, sizeof(struct mem_cache_node), CLEAR,
          YELLOW, slab_size, CLEAR,
          YELLOW, slabs_num_pages, CLEAR,
          CYAN, REDUCE_ADDR(*(&cache_node), F6), CLEAR,
          CYAN, REDUCE_ADDR(free_mem_start_addr, F6), CLEAR,
          YELLOW, REDUCE_ADDR(free_mem_start_addr, F6) - REDUCE_ADDR(*(&cache_node), F6), CLEAR
          );
    print("%s-----------------------%s\n", YELLOW, CLEAR);
}

void print_slabs_free_list(struct slab_list* slabs_free_list) {

    print("%s-- SLABS FREE LIST --%s\n", YELLOW, CLEAR);

    struct slab_list* list = slabs_free_list;
    uint32 slabs_count = 0;
    while (list){
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



bool print_all_slabs_free_list() {

    struct mem_cache_list* mem_cache_list = mem_cache_get_list();
    struct mem_cache_node* list_node = mem_cache_list->head;
    print("-- %sMem Cache List Nodes%s --\n", ry, CLEAR);
    while (list_node) {
        print("  next address:%s%x%s "
              "  slab address:%s%x%s\n",
              CYAN, REDUCE_ADDR(list_node->next, F6), CLEAR, CYAN,
              REDUCE_ADDR(&list_node->mem_cache, F6), CLEAR);

        print_slabs_free_list(list_node->mem_cache.slabs_free_list);

        list_node = list_node->next;
    }
    print("\n");

    return TRUE;
}


// ERROR handling
void slab_allocator_print_error(const enum SLAB_ALLOCATOR_ERROR_CODE code)
{
  switch (code) {
    case SLAB_ALLOC_MEMCACHE_NOT_FOUND: {
      print("%sERROR:\n"
          "Slab allocator: mem_cache not found%s\n",
          RED, CLEAR
          );
      break;
    }
  }
}