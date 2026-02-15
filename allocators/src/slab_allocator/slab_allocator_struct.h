/*
 * Created by ckalo on 1/20/26.
 */
#ifndef ALLOCATORS_SLAB_ALLOCATOR_STRUCT_H
#define ALLOCATORS_SLAB_ALLOCATOR_STRUCT_H

/*
 * MEMORY IMPLEMENTATION
 *
 * memory manager should malloc()
 *
 * 1. storage for memory manager structs
 * 2. storage for allocators
 *      slab allocator's mem_caches and slab lists
 * 3. actual storage data
 *      buffer container should be close
 *
 * That way we keep actual program's data together
 * and all structures for handling that data on their own storage.
 * if slab allocator needs to access its structures to handle slabs
 * then the cpu caches does not realy matter
 */


#include <stdio.h>

#include "char_buf.h"
#include "my_types.h"

enum MEM_CACHE_SIZES {
    MEM_CACHE_SIZE_8 = 0,
    MEM_CACHE_SIZE_16,
    MEM_CACHE_SIZE_32,
    MEM_CACHE_SIZE_64,
    MEM_CACHE_SIZES_CAPACITY,
    MEM_CACHE_SIZE_128,
    MEM_CACHE_SIZE_256,
    MEM_CACHE_SIZE_512,
    MEM_CACHE_SIZE_1024, /// this is the current memory page size
    MEM_CACHE_SIZE_2048,
    MEM_CACHE_SIZE_4096,
    MEM_CACHE_SIZE_8192,
    MEM_CACHE_SIZE_16384,
    MEM_CACHE_SIZE_32768, /// this is the current memory chunk size. Above that we ask for whole chunks
};

enum SLAB_ALLOCATOR_ERROR_CODE {
  SLAB_ALLOC_MEMCACHE_NOT_FOUND = 0,
};

typedef uint16 slot_size_t;
typedef uint16 buffer_size_t;
typedef uint16 slab_size_t;

struct slab {
    void* addr;
    uint32 inuse;
    uint32 capacity;
    // If slots are too small the bufctl is stored outside allocated cache
    uint64 mem_bufctl; // array of uint, as many as the slots of the slab.
};

struct slab_node {
    struct slab_node* next;
    struct slab* slab;
};

struct slab_flist {
    struct slab_flist* next;
};



struct mem_cache {
    void* slabs_address;
    struct slab_flist* slabs_free_list;
    uint16 slab_size; // each slab's size in bytes
    uint16 slabs_count; // total slabs count
    uint16 slabs_used; // used slabs count
    uint8 slabs_num_pages; // number of pages of all the slabs
    uint8 type_size; // the stored objects type

    char_buf16_t name; // todo: later use indexed string buffer
};

struct mem_cache_node {
    struct mem_cache_node* next;
    struct slab_node* slab_address; // the memory address of the mem_cache.
    struct mem_cache mem_cache; // todo: maybe mem_cache should be stored later
                                // in mem, so that list is compacted
};

struct mem_cache_list {
    struct mem_cache_node* head;
    uint16 count;
};

static void slab_alloc_print_structs_sizes(void)
{
    printf("mem_cache_list: %lu\n", sizeof(struct mem_cache_list));
    printf("mem_cache_node: %lu\n", sizeof(struct mem_cache_node));
    printf("mem_cache: %lu\n", sizeof(struct mem_cache));
    printf("-- mem_cache struct has a slabs list\n");
    printf("   slab_list: %lu\n", sizeof(struct slab_flist));
}

#endif // MEMORY_SLAB_ALLOCATOR_STRUCT_H
