/*
 * Created by ckalo on 8/3/26.
 */
#ifndef ALLOCATORS_STRING_ALLOCATOR_STRUCT_H
#define ALLOCATORS_STRING_ALLOCATOR_STRUCT_H

#include <stdio.h>
#include "char_buf.h"
#include "mem_man_interface.h"
#include "my_types.h"

#define INITIAL_NUM_INSTANCES 8


enum STRING_MEM_CACHE_SIZES {
    STRING_SLAB_SIZE_8 = 8,
    STRING_SLAB_SIZE_16 = 16,
    STRING_SLAB_SIZE_32 = 32,
    STRING_SLAB_SIZE_64 = 64,
    STRING_SLAB_SIZE_128 = 128,
    STRING_SLAB_SIZE_256 = 256,
    STRING_SLAB_SIZE_512 = 512,
    STRING_SLAB_SIZE_1024 = 1024,
    STRING_SLAB_SIZE_2048 = 2048,
    STRING_SLAB_SIZE_4096 = 4096,
    STRING_SLAB_SIZE_8192 = 8192,
    STRING_SLAB_SIZE_16384 = 16384,
    STRING_SLAB_SIZE_32768 = 32768,

    /* -1 is for the '\0' character */
    STRING_SLAB_ALLOCATOR_INSTANCE_SIZE_SMALL = 255 - 1, // 256 does no comply with the strlen_t type:1 byte
    STRING_SLAB_ALLOCATOR_INSTANCE_SIZE_MEDIUM = 1024 - 1,
    STRING_SLAB_ALLOCATOR_INSTANCE_SIZE_LARGE = 4096 - 1,
    STRING_SLAB_ALLOCATOR_INSTANCE_SIZES = 3,
};

enum STRING_MEM_CACHE_SIZES_IDXS {
    STRING_SLAB_SIZE_8_IDX = 0,
    STRING_SLAB_SIZE_16_IDX,
    STRING_SLAB_SIZE_32_IDX,
    STRING_SLAB_SIZE_64_IDX,
    STRING_SLAB_SIZE_128_IDX,
    STRING_SLAB_SIZE_256_IDX,
    STRING_SLAB_SIZE_512_IDX,
    STRING_SLAB_SIZE_1024_IDX, /// this is the current memory page size
    STRING_SLAB_SIZE_2048_IDX,
    STRING_SLAB_SIZE_4096_IDX,
    STRING_SLAB_SIZE_8192_IDX,
    STRING_SLAB_SIZE_16384_IDX,
    STRING_SLAB_SIZE_32768_IDX, /// this is the current memory chunk size. Above that we ask for whole chunks
    STRING_SLAB_SIZES_LEN,
};

enum STRING_ALLOCATOR_ERROR_CODE {
    STRING_ALLOC_MEMCACHE_NOT_FOUND = 0,
    STRING_ALLOC_MEMCACHE_NO_INIT,
};

typedef uint32 slab_size_t;
typedef uint16 string_size_t;

struct string_slab_flist {
    struct string_slab_flist* next;
};

struct string_mem_cache {
    struct string_slab_flist* slabs_free_list;
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

struct string_slab_allocator_instance {
    char_buf16_t* name;
    void* start;
    uint64 size;
    pageid_t num_pages;
    memid_t mem_id;
};


#endif // ALLOCATORS_STRING_ALLOCATOR_STRUCT_H
