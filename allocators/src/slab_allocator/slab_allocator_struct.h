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

enum MEM_CACHE_SIZES {
    SLAB_SIZE_8 = 8,
    SLAB_SIZE_16 = 16,
    SLAB_SIZE_32 = 32,
    SLAB_SIZE_64 = 64,
    SLAB_SIZE_128 = 128,
    SLAB_SIZE_256 = 256,
    SLAB_SIZE_512 = 512,
    SLAB_SIZE_1024 = 1024,
    SLAB_SIZE_2048 = 2048,
    SLAB_SIZE_4096 = 4096,
    SLAB_SIZE_8192 = 8192,
    SLAB_SIZE_16384 = 16384,
    SLAB_SIZE_32768 = 32768,
};
enum MEM_CACHE_SIZES_IDXS {
    SLAB_SIZE_8_IDX = 0,
    SLAB_SIZE_16_IDX,
    SLAB_SIZE_32_IDX,
    SLAB_SIZE_64_IDX,
    SLAB_SIZE_128_IDX,
    SLAB_SIZE_256_IDX,
    SLAB_SIZE_512_IDX,
    SLAB_SIZE_1024_IDX, /// this is the current memory page size
    SLAB_SIZE_2048_IDX,
    SLAB_SIZE_4096_IDX,
    SLAB_SIZE_8192_IDX,
    SLAB_SIZE_16384_IDX,
    SLAB_SIZE_32768_IDX, /// this is the current memory chunk size. Above that we ask for whole chunks
    SLAB_SIZES_LEN,
};

enum SLAB_ALLOCATOR_ERROR_CODE {
    SLAB_ALLOC_MEMCACHE_NOT_FOUND = 0,
    SLAB_ALLOC_MEMCACHE_NO_INIT,
};

typedef uint16 buffer_size_t; // MAX:65535
typedef uint32 slab_size_t;
typedef uint16 offset16_t;



struct slab_flist {
    struct slab_flist* next;
};




struct mem_cache {
    struct slab_flist* slabs_free_list;
    void* slabs_start;     // the start of slabs alocated memory
    void* last_slab_end;   // the start of slabs alocated memory
    slab_size_t slab_size; // each slab's size in bytes
    uint16 type_size;      // the stored objects type
    uint16 slabs_capacity; // total slabs count
    uint16 slabs_count;    // used/unused slabs count
    uint16 slabs_used;     // used slabs count
    uint16 slabs_num_pages; // number of pages of all the slabs
    struct memhnd_t mem_handle;
    uint8 idx;             // index in the mcaches table
    uint8 is_active;       // the stored objects type

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
