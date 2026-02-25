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
#include "mem_man_interface.h"
#include "my_types.h"

enum MEM_CACHE_SIZES {
    SLAB_SIZE_8 = 0,
    SLAB_SIZE_16,
    SLAB_SIZE_32,
    SLAB_SIZE_64,
    SLAB_SIZE_128,
    SLAB_SIZE_256,
    SLAB_SIZE_512,
    SLAB_SIZE_1024, /// this is the current memory page size
    SLAB_SIZE_2048,
    SLAB_SIZE_4096,
    SLAB_SIZES_LEN,
    SLAB_SIZE_8192,
    SLAB_SIZE_16384,
    SLAB_SIZE_32768, /// this is the current memory chunk size. Above that we ask for whole chunks
};

enum SLAB_ALLOCATOR_ERROR_CODE {
    SLAB_ALLOC_MEMCACHE_NOT_FOUND = 0,
    SLAB_ALLOC_MEMCACHE_NO_INIT,
};

typedef uint16 buffer_size_t;
typedef uint16 slab_size_t;
typedef uint16 offset16_t;




struct slab {
    offset16_t next;
    memid_t page_id;
    uint16 flags;
};

struct slab_flist {
    struct slab_flist* next;
};



struct mem_cache {
    struct slab_flist* slabs_free_list;
    struct slab* slabs;
    uint16 slab_size; // each slab's size in bytes
    uint16 type_size; // the stored objects type
    uint16 slabs_count; // used slabs count
    uint16 slabs_capacity; // total slabs count
    uint16 slabs_used; // used slabs count
    uint8 slabs_num_pages; // number of pages of all the slabs
    uint8 is_active; // the stored objects type
    struct memhnd_t mem_handle;

    char_buf16_t name; // todo: later use indexed string buffer
};


static void
slab_alloc_print_structs_sizes(void)
{
    printf("mem_cache: %lu\n", sizeof(struct mem_cache));
    printf("-- mem_cache struct has a slabs list\n");
    printf("   slab_list: %lu\n", sizeof(struct slab_flist));
}

#endif // ALLOCATORS_SLAB_ALLOCATOR_STRUCT_H
