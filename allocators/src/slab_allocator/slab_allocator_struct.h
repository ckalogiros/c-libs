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

/*
 *  Without coloring (all objects same cache alignment)
 *  Slab 1: [CPU0][CPU0][CPU0]...  All in same cache lines
 *  Slab 2: [CPU0][CPU0][CPU0]...  Cache contention!
 *
 *  With coloring (offset each slab)
 *  Slab 1 (color 0): [CPU0][CPU0][CPU0]...
 *  Slab 2 (color 16): --[CPU1][CPU1][CPU1]...  Different cache lines
 *  Slab 3 (color 32): ----[CPU2][CPU2][CPU2]...  Better distribution
 */


#include <stdio.h>

#include "char_buf.h"
#include "mem_man_interface.h"
#include "my_types.h"

enum SLAB_ALLOCATOR_POOL_SIZES {
    /* -1 is for the '\0' character */
    SLAB_ALLOCATOR_POOL_SIZE_SMALL = 255 - 1, // 256 does no comply with the strlen_t type:1 byte
    SLAB_ALLOCATOR_POOL_SIZE_MEDIUM = 1024 - 1,
    SLAB_ALLOCATOR_POOL_SIZE_LARGE = 4096 - 1,
    SLAB_ALLOCATOR_POOL_SIZES_COUNT = 3,
};
enum SLAB_ALLOCATOR_MCACHE_SIZES {
    SLAB_ALLOCATOR_MCACHE_SIZE_8 = 8,
    SLAB_ALLOCATOR_MCACHE_SIZE_16 = 16,
    SLAB_ALLOCATOR_MCACHE_SIZE_32 = 32,
    SLAB_ALLOCATOR_MCACHE_SIZE_64 = 64,
    SLAB_ALLOCATOR_MCACHE_SIZE_128 = 128,
    SLAB_ALLOCATOR_MCACHE_SIZE_256 = 256,
    SLAB_ALLOCATOR_MCACHE_SIZE_512 = 512,
    SLAB_ALLOCATOR_MCACHE_SIZE_1024 = 1024,
    SLAB_ALLOCATOR_MCACHE_SIZE_2048 = 2048,
    SLAB_ALLOCATOR_MCACHE_SIZE_4096 = 4096,
    SLAB_ALLOCATOR_MCACHE_SIZE_8192 = 8192,
    SLAB_ALLOCATOR_MCACHE_SIZE_16384 = 16384,
    SLAB_ALLOCATOR_MCACHE_SIZE_32768 = 32768,
};
enum SLAB_ALLOCATOR_MCACHE_SIZES_IDXS {
    SLAB_ALLOCATOR_SIZE_8_IDX = 0,
    SLAB_ALLOCATOR_SIZE_16_IDX,
    SLAB_ALLOCATOR_SIZE_32_IDX,
    SLAB_ALLOCATOR_SIZE_64_IDX,
    SLAB_ALLOCATOR_SIZE_128_IDX,
    SLAB_ALLOCATOR_SIZE_256_IDX,
    SLAB_ALLOCATOR_SIZE_512_IDX,
    SLAB_ALLOCATOR_SIZE_1024_IDX, /// this is the current memory page size
    SLAB_ALLOCATOR_SIZE_2048_IDX,
    SLAB_ALLOCATOR_SIZE_4096_IDX,
    SLAB_ALLOCATOR_SIZE_8192_IDX,
    SLAB_ALLOCATOR_SIZE_16384_IDX,
    SLAB_ALLOCATOR_SIZE_32768_IDX, /// this is the current memory chunk size. Above that we ask for whole chunks
    SLAB_ALLOCATOR_MCACHE_SIZES_COUNT,
};

enum SLAB_ALLOCATOR_ERROR_CODE {
    SLAB_ALLOCATOR_MEMCACHE_NOT_FOUND = 0,
    SLAB_ALLOCATOR_MEMCACHE_NO_INIT,
};


// TODO: implement in arena_allocator.h
#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H
typedef int32 nameid_t;
struct bump_allocator_instance {
    memid_t instance_id;
    uint32 bump_ptr;
    uint16 count;
    uint16 size;
};
#endif //ARENA_ALLOCATOR_H

typedef int8 salltable_t;
typedef uint32 slab_size_t;
typedef uint16 slab_elems_t; // MAX:65535

struct slab_allocator_instance {
    memid_t instance_id;
    salltable_t count;
    salltable_t size; // TODO:UNUSED
};


/* devide sized mcaches into zones:
 * small, mid, large,
 * each one have its seperate pool of memory */
struct slab_allocator_pool {
    nameid_t name;
    void* start;
    uint64 size;
    pageid_t num_pages;
    memid_t mem_id;
};

// TODO: remove: slabs_used, slabs_num_pages
struct slab_allocator_mcache {
    struct flist* slabs_free_list;
    void* slabs_start; // the start of slabs alocated memory
    void* last_slab_end; // the start of slabs alocated memory
    slab_size_t slab_size; // each slab's size in bytes
    uint16 slabs_capacity; // total slabs count
    uint16 slabs_count; // used/unused slabs count
    uint16 slabs_used; // used slabs count
    uint16 slabs_num_pages; // number of pages of all the slabs
    struct memhnd_t mem_handle;
    uint8 idx; // index in the mcaches table
    uint8 is_active; // the stored objects type

    char_buf16_t name; // todo: later use indexed string buffer
};

#endif // ALLOCATORS_SLAB_ALLOCATOR_STRUCT_H
