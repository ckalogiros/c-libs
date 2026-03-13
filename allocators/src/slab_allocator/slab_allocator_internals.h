/*
 * Created by ckalo on 1/22/26.
 */
#ifndef ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H
#define ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H

#include "slab_allocator_struct.h"

#define DEBUG_FPRINT_MCACHE
// #define DEBUG_FPRINT_MCACHES_IN_FILES
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



void slab_allocator_init(void);

memid_t slab_allocator_create_new_instance();

void slab_allocator_create_new_pool(const pageid_t num_pages);

struct slab_allocator_mcache* slab_allocator_create_mcache(const slab_elems_t num_elems, const uint8 type_size);

struct slab_allocator_mcache* mcache_extend_allocation(struct slab_allocator_mcache* const mcache, const uint32 left_over_mem);

struct flist* mcache_alloc_slab(struct slab_allocator_mcache* mcache);

bool mcache_free_slab(struct slab_allocator_mcache* mcache, void* slab_address);

void mcache_table_destroy(void);


/* HELPER FUNCTIONS */
memid_t pool_find_size(const slab_size_t slab_size);

struct slab_allocator_mcache* mcache_find_size(const slab_size_t slab_size);

struct slab_allocator_mcache* mcache_get_next_free(void);


/* DEBUG */
void* mcache_flist_peek_next_free_slab(slab_elems_t num_elems, const uint8 type_size);


/*
 * these are a copies of the function(s) of slab_allocator, for testing purposes
 * MUST BE KEPT UPDATED TO THE ACTUAL SLAB ALLOCATOR'S FUNCTIONS
 */
char* mcache_free_slab_test_debug(struct slab_allocator_mcache* mcache, void* slab_address);

char* slab_allocator_release_test_debug(void* ptr, const slab_size_t string_size);

#endif //ALLOCATORS_SLAB_ALLOCATOR_INTERNALS_H
