/*
 * Created by ckalo on 3/14/26.
 */
#ifndef SLAB_ALLOC_FPRINT_DEBUG_H
#define SLAB_ALLOC_FPRINT_DEBUG_H

#include "my_types.h"
#include "file_sys_interface.h"
#include "str_utils.h"


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

#endif




struct fs_t;

/***************************************************************************************************
 * FOR DEBUG */
static struct fs_t* fs_flist = NULL;
/* output a debug file per mcache  */
static struct fs_t* fs_mcaches[SLAB_ALLOCATOR_MCACHE_SIZES_COUNT] = {NULL};
/* output one debug file and register all created mcaches  */
static struct fs_t* fs_register_created_mcaches = NULL;
/* FOR DEBUG
 **************************************************************************************************/

static void sslall_fprint_init()
{
    /* DEBUG-FPRINT */
    fs_flist = fs_create("allocators/slab/slab_flist.txt");
    fs_register_created_mcaches = fs_create("allocators/slab/register_created_slab_mcaches.txt");
    fs_append(fs_register_created_mcaches, "OPERATIONS: "
              "Create-Allocate-Release operations "
              "of slab allocator mcaches\n"
              "________________________________________________________"
              "________________________________________________________"
              "________________________________________________________\n"
    );
}

void fprint_register_mcache(struct fs_t* fs, struct slab_allocator_mcache* mcache, const char* operation) {

        fs_append(fs,
            "%9s | "
            "pages: %3u | "
            "capac: %5u slabs | "
            "count: %3u | "
            "used:  %3u | "
            "flist: %08x %08x | "
            "name: %14s\n",
            operation,
            mcache->slabs_num_pages,
            mcache->slabs_capacity,
            mcache->slabs_count,
            mcache->slabs_used,
            (uint64)mcache->slabs_free_list >> 32,(uint64)mcache->slabs_free_list & 0xFFFFFFFF,
            mcache->name.buf
        );
}

static void slall_fprint_mcaches(struct slab_allocator_mcache* mcache, const char* operation)
{
    const char buf[INT32_TO_STR_BUFFER_SIZE] = {0};
    str_conv_int32_to_str(buf, mcache->slab_size);
    fprint_register_mcache(fs_register_created_mcaches, mcache, operation);
}

// fs_write_list(fs_flist, flist_heads_table[mem_id],
//     "\n--------------------\n  new memory free list\n  head:%x\n", flist_heads_table[mem_id]
// );

#endif //SLAB_ALLOC_FPRINT_DEBUG_H