/*
 * Created by ckalo on 1/22/26.
 *
 * todo:
 *    . mem_cache_list should be inserting at the head, NOT the tail
 *      so that inserting a new cache is with no CHECK
 */

#include "slab_allocator_internals.h"
#include "logger.h"
#include "mem_man.h"
#include "slab_allocator_log.h"
#include <string.h>


static struct mem_cache_list *mem_cache_list = NULL;

struct mem_cache_list *mem_cache_get_list() { return mem_cache_list; }


void mem_cache_constructor(struct mem_cache *mcache) {
    mcache->slab_size = 0;
    mcache->slabs_num_pages = 0;
    mcache->slabs_used = 0;
    mcache->slabs_count = 0;
    mcache->slabs_free_list = NULL;
    mcache->slabs_address = NULL;
}


struct mem_cache_node *mem_cache_alloc(struct mem_cache_node *mlist) {
    void *addr_ptr = mlist + 1;

    struct mem_cache_node *new_list_node = addr_ptr;
    mlist->next = new_list_node;
    new_list_node->next = NULL;
    mem_cache_list->count++;

    return new_list_node;
}


// initialize cache list
void mem_cache_init() {
    print("\n%s##################\n"
          "  SLAB ALLOCATOR%s\n\n", YELLOW, CLEAR);

    // Allocate memory to store a list of all size caches
    const pageid_t page_id = mmu_allocate_pages(1);
    void *mem_cache_address = Temp_get_address_by_id(page_id);

    void *addr_ptr = mem_cache_address;

    memset(mem_cache_address, 0xff, PAGE_SIZE);

    // initialize static global mem_cache_list
    mem_cache_list = (struct mem_cache_list *) mem_cache_address;
    addr_ptr += sizeof(struct mem_cache_list);
    mem_cache_list->count = 0;

    // assign memory for the "head" node of mem_cache_list struct
    mem_cache_list->head = (struct mem_cache_node *) addr_ptr;
    mem_cache_list->head->next = NULL;
}

struct slab_list *slab_alloc(const uint32 slab_size_bytes, struct mem_cache *mem_cache) {
    uint16 num_pages = 1;
    // todo: deal with allocations: (slab_size > PAGE)
    while (num_pages * PAGE_SIZE < slab_size_bytes){
        num_pages++;
    }
    mem_cache->slabs_num_pages += num_pages;

    if (slab_size_bytes > num_pages * PAGE_SIZE)
        print("%sERROR\nslab size > num_pages to be allocated\n"
              "slab_size_bytes:%u\nnum_pages*PAGE_SIZE:%u\n%s",
              RED, slab_size_bytes, num_pages * PAGE_SIZE, CLEAR);

    const uint16 slabs_count = (num_pages * PAGE_SIZE) / slab_size_bytes;

    // allocate memory for slabs
    const pageid_t page_id = mmu_allocate_pages(num_pages);
    void *addr_ptr = Temp_get_address_by_id(page_id);

    memset(addr_ptr, page_id, num_pages * PAGE_SIZE); // debug mem

    // update free list
    void *ptr = addr_ptr;
    for (uint16 i = 0; i < slabs_count; i++){
        struct slab_list *free_list = (struct slab_list *) ptr;
        free_list->next = mem_cache->slabs_free_list;
        mem_cache->slabs_free_list = free_list;
        ptr += slab_size_bytes;
        mem_cache->slabs_count++;
    }

    print_slabs_free_list(mem_cache->slabs_free_list);

    return addr_ptr;
}

struct mem_cache *mem_cache_create(const buffer_size_t buffer_size, const uint8 type_size) {
    /*****************
    * mem cache creation
    *****************/

    struct mem_cache_node *new_cache_node = NULL;

    if (mem_cache_list == NULL){
        mem_cache_init();
        new_cache_node = mem_cache_list->head;
    } else{
        // find the last cache and get the address+cache size to get the free memory for the new cache
        struct mem_cache_node *last_cache_node = mem_cache_list->head;
        while (last_cache_node->next)
            last_cache_node = last_cache_node->next;

        new_cache_node = mem_cache_alloc(last_cache_node);
    }


    // members initialization
    mem_cache_constructor(&new_cache_node->mem_cache);
    new_cache_node->mem_cache.slabs_count = 0;
    new_cache_node->mem_cache.slabs_used = 0;
    new_cache_node->mem_cache.type_size = type_size;
    // !IMPORTANT: slabs_free_list must be set to NULL for every new mem_cache(for the build of slabs_free_list)
    new_cache_node->mem_cache.slabs_free_list = NULL;


    /*****************
     * slabs creation
     *****************/
    const uint32 slab_size_bytes = buffer_size * type_size;
    new_cache_node->mem_cache.slab_size = slab_size_bytes;

    // create memory for slabs and initialize slabs free list
    new_cache_node->mem_cache.slabs_address = slab_alloc(slab_size_bytes, &new_cache_node->mem_cache);

    const char *name = number_to_string("mem_cache_", slab_size_bytes).buf; // cache's name
    char_buf16_copy_from_str(name, &new_cache_node->mem_cache.name);

    return &new_cache_node->mem_cache;
}

struct mem_cache *mem_cache_find_cache(const slab_size_t slab_size) {
    // todo: we cannot afford to check if mem_cache list is initialized on every find_cache() call
    if (mem_cache_list == NULL || mem_cache_list->head == NULL){
        return NULL;
    }

    struct mem_cache_node *mcache_list = mem_cache_list->head;
    while (mcache_list){
        if (mcache_list->mem_cache.slab_size == slab_size)
            return &mcache_list->mem_cache;

        mcache_list = mcache_list->next;
    }

    return NULL;
}

// TEMPORARY
char_buf16_t number_to_string(const char *str, const uint32 integer) {
    char_buf16_t buf = {
        .buf = {'\0'},
        .len = 0,
    };

    char_buf16_copy_from_str(str, &buf);

    switch (integer){
        case 8: {
            char_buf16_copy_from_str_append("8", &buf);
            break;
        }
        case 16: {
            char_buf16_copy_from_str_append("16", &buf);
            break;
        }
        case 32: {
            char_buf16_copy_from_str_append("32", &buf);
            break;
        }
        case 64: {
            char_buf16_copy_from_str_append("64", &buf);
            break;
        }
        case 128: {
            char_buf16_copy_from_str_append("128", &buf);
            break;
        }
        case 256: {
            char_buf16_copy_from_str_append("256", &buf);
            break;
        }
        case 512: {
            char_buf16_copy_from_str_append("512", &buf);
            break;
        }
        default: {
            char_buf16_copy_from_str_append("error", &buf);
            break;
        }
    }

    return buf;
}
