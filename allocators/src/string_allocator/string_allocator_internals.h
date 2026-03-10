/*
 * Created by ckalo on 8/3/26.
 */
#ifndef ALLOCATORS_STRING_ALLOCATOR_INTERNALS_H
#define ALLOCATORS_STRING_ALLOCATOR_INTERNALS_H

#include "string_allocator_struct.h"
#include "debug_defines.h"



#ifdef DEBUG_FPRINT_MCACHE
#   define fs_register_mcache_create(fs, mcache)\
        fs_append(fs,\
        "%8s: | "          \
        "name: %014s | "       \
        "pages: %3u | "       \
        "capac: %5u slabs | "        \
        "count: %3u | "        \
        "used:  %3u | "         \
        "flist: %08x %08x\n",   \
        "CREATE",       \
        mcache->name.buf,       \
        mcache->slabs_num_pages,\
        mcache->slabs_capacity, \
        mcache->slabs_count,    \
        mcache->slabs_used,     \
        (uint64)mcache->slabs_free_list >> 32,(uint64)mcache->slabs_free_list & 0xFFFFFFFF\
        )

#   define fs_register_mcache_alloc_slab(fs, mcache)\
        fs_append(fs,\
        "%8s: | "        \
        "name: %014s | "       \
        "pages: %3u | "       \
        "capac: %5u slabs | "        \
        "count: %3u | "        \
        "used:  %3u | "         \
        "flist: %08x %08x | "   \
        "free: %08x %08x | "    \
        "dif: %04llu bytes\n",  \
        "ALLOCATE",       \
        mcache->name.buf,       \
        mcache->slabs_num_pages,\
        mcache->slabs_capacity, \
        mcache->slabs_count,    \
        mcache->slabs_used,     \
        (uint64)mcache->slabs_free_list >> 32,(uint64)mcache->slabs_free_list & 0xFFFFFFFF,\
        (uint64)free >> 32,(uint64)free & 0xFFFFFFFF,\
        (uint64*)mcache->slabs_free_list - (uint64*)free\
        )

#   define fs_register_mcache_extend_alloc(fs, mcache, address)\
        fs_append(fs,\
        "%8s: | "        \
        "name: %014s | "       \
        "pages: %3u | "       \
        "capac: %5u slabs | "        \
        "count: %3u | "        \
        "used:  %3u | "         \
        "flist: %08x %08x | "   \
        "addr: %08x %08x | "    \
        "dif: %04llu bytes\n",  \
        "EXTEND",       \
        mcache->name.buf,       \
        mcache->slabs_num_pages,\
        mcache->slabs_capacity, \
        mcache->slabs_count,    \
        mcache->slabs_used,     \
        (uint64)mcache->slabs_free_list >> 32,(uint64)mcache->slabs_free_list & 0xFFFFFFFF,\
        (uint64)address >> 32,(uint64)address & 0xFFFFFFFF,\
        (uint64*)address - (uint64*)mcache->slabs_free_list\
        )

#   define fs_register_mcache_release_slab(fs, mcache)\
        fs_append(fs,\
        "%8s: | "        \
        "name: %014s | "       \
        "pages: %3u | "       \
        "capac: %5u slabs | "        \
        "count: %3u | "        \
        "used:  %3u | "         \
        "flist: %08x %08x | "   \
        "free: %08x %08x | "    \
        "dif: %04llu bytes\n",  \
        "RELEASE",       \
        mcache->name.buf,       \
        mcache->slabs_num_pages,\
        mcache->slabs_capacity, \
        mcache->slabs_count,    \
        mcache->slabs_used,     \
        (uint64)mcache->slabs_free_list >> 32,(uint64)mcache->slabs_free_list & 0xFFFFFFFF,\
        (uint64)free >> 32,(uint64)free & 0xFFFFFFFF,\
        (uint64*)mcache->slabs_free_list - (uint64*)free\
        )

#   define fs_register_mcache_append(fs, fmt, ...)\
        fs_append(fs, fmt, ##__VA_ARGS__)

#else
#   define fs_register_mcache_create(fs, mcache)
#   define fs_register_mcache_alloc_slab(fs, mcache)
#   define fs_register_mcache_extend_alloc(fs, mcache, address)
#   define fs_register_mcache_release_slab(fs, mcache)
#endif

/*********************************************************************************************/
// nen implementation

bool is_string_mcache_table_init();

void string_mem_cache_table_init(void);

void string_mem_cache_init(enum STRING_MEM_CACHE_SIZES_IDXS slab_size);

void string_mem_cache_table_destroy(void);
/*********************************************************************************************/



struct string_mem_cache* string_mem_cache_create(const string_size_t string_size);

struct string_mem_cache* string_mem_cache_extend_allocation(struct string_mem_cache* const  mcache, const uint32 left_over_mem);

struct string_slab_flist* string_mem_cache_alloc_slab(struct string_mem_cache* mcache);

bool string_mem_cache_free_slab(struct string_mem_cache* mcache, void* slab_address);

struct string_mem_cache* string_mem_cache_find_cache(const slab_size_t slab_size);

struct string_mem_cache* string_mem_cache_get_next_free_cache(void);

/* DEBUG */
void* string_mem_cache_flist_peek_next_free_slab(const string_size_t string_size);



#endif //ALLOCATORS_STRING_ALLOCATOR_INTERNALS_H
