/*
 * Created by ckalo on 8/26/26.
 */

#include "slab_allocator_fprint.h"
#include "my_types.h"
#include "file_sys_interface.h"
#include "debug_defines.h"
#include "logger.h"

// #define DEBUG_FPRINT_MCACHE
// #define DEBUG_FPRINT_MCACHES_IN_FILES
#ifdef DEBUG_FPRINT_MCACHE
#   define fs_register_mcache_create(fs, mcache)\
        fs_append(fs,\
        "%8s: | "          \
        "name: %014s | "       \
        "pages: %3u | "       \
        "capac: %5u slabs | "        \
        "count: %3u | "        \
        "flist: %08x %08x\n",   \
        "CREATE",       \
        mcache->name.buf,       \
        mcache->slabs_num_pages,\
        mcache->slabs_capacity, \
        mcache->slabs_count,    \
        (uint64)mcache->slabs_free_list >> 32,(uint64)mcache->slabs_free_list & 0xFFFFFFFF\
        )

#   define fs_register_mcache_alloc_slab(fs, mcache)\
        fs_append(fs,\
        "%8s: | "        \
        "name: %014s | "       \
        "pages: %3u | "       \
        "capac: %5u slabs | "        \
        "count: %3u | "        \
        "flist: %08x %08x | "   \
        "free: %08x %08x | "    \
        "dif: %04llu bytes\n",  \
        "ALLOCATE",       \
        mcache->name.buf,       \
        mcache->slabs_num_pages,\
        mcache->slabs_capacity, \
        mcache->slabs_count,    \
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
        "flist: %08x %08x | "   \
        "addr: %08x %08x | "    \
        "dif: %04llu bytes\n",  \
        "EXTEND",       \
        mcache->name.buf,       \
        mcache->slabs_num_pages,\
        mcache->slabs_capacity, \
        mcache->slabs_count,    \
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
        "flist: %08x %08x | "   \
        "free: %08x %08x | "    \
        "dif: %04llu bytes\n",  \
        "RELEASE",       \
        mcache->name.buf,       \
        mcache->slabs_num_pages,\
        mcache->slabs_capacity, \
        mcache->slabs_count,    \
        (uint64)mcache->slabs_free_list >> 32,(uint64)mcache->slabs_free_list & 0xFFFFFFFF,\
        (uint64)free >> 32,(uint64)free & 0xFFFFFFFF,\
        (uint64*)mcache->slabs_free_list - (uint64*)free\
        )

#   define fs_register_mcache_append(fs, fmt, ...)\
        fs_append(fs, fmt, ##__VA_ARGS__)

#endif

/***************************************************************************************************
 * FOR DEBUG */
/* output one debug file and register all created mcaches  */
static struct fs_t* fs_created_mcaches = NULL;
static struct fs_t* fs_created_slabs   = NULL;
static struct fs_t* fs_created_slab    = NULL;
/* FOR DEBUG
 **************************************************************************************************/

void fprint_mcache(struct slab_allocator_mcache* mcache, const char* operation) {

    if(fs_created_mcaches == NULL) {
        fs_created_mcaches = fs_create("Debug/allocators/slab/mcaches.txt");
        fs_append(fs_created_mcaches, "OPERATIONS: "
                  "Create-Allocate-Release operations "
                  "of slab allocator mcaches\n"
                  "________________________________________________________\n"
        );
    }

    uint64 lower = 0, upper = 0;
    if(mcache->slabs_free_list) {
        upper = (uint64) mcache->slabs_free_list>>32;
        lower = (uint64) mcache->slabs_free_list&0xFFFFFFFF;
    }

    print("");

    fs_append(fs_created_mcaches,
        "%14s | "
        "id: %3u | "
        "pages: %3u | "
        "capac: %5u slabs | "
        "count: %3u | "
        "slab size: %3u | "
        "flist: %08x %08x | "
        "name: %14s\n",
        operation,
        mcache->idx,
        mcache->slabs_num_pages,
        mcache->slabs_capacity,
        mcache->slabs_count,
        mcache->slab_size,
        upper, lower,
        mcache->name
    );
}

void fprint_append_all_slabs_of_mcache(struct slab_allocator_mcache* mcache, const char* info) {

    if(fs_created_slabs == NULL) {
        fs_created_slabs = fs_create("Debug/allocators/slab/slabs_selective.txt");
        fs_append(fs_created_slabs, "PRINT MEMORY OF ALL SLABS IN MCACHE"
                  "________________________________________________________\n"
        );
    }

    fs_append(fs_created_slabs,
        "\n"
        "###################\n"
        "# info             %s \n"
        "# name             %s \n"
        "# slabs_free_list  %016x \n"
        "# slabs_start      %016x \n"
        "# last_slab_end    %u \n"
        "# slab_size        %u \n"
        "# type_size        %u \n"
        "# num_elems        %u \n"
        "# slabs_capacity   %u \n"
        "# slabs_count      %u \n"
        "# slabs_num_pages  %u \n"
        "# idx              %u \n"
        "# is_active        %u \n"
        "# mem_handle:     id: %d, page_id: %d, num_pages: %d \n"
        "###################\n",
        info,
        mcache->name,
        mcache->slabs_free_list,
        mcache->slabs_start,
        mcache->last_slab_end,
        mcache->slab_size,
        mcache->type_size,
        mcache->num_elems,
        mcache->slabs_capacity,
        mcache->slabs_count,
        mcache->slabs_num_pages,
        mcache->idx,
        mcache->is_active,
        mcache->mem_handle.mem_id, mcache->mem_handle.page_id, mcache->mem_handle.num_pages
    );
    const uint32 data_size = mcache->slab_size * mcache->slabs_count;
    fs_append_mem(fs_created_slabs, mcache->slabs_start, data_size, "slab allocator:mcache slabs");

}

void fprint_append_a_slab(void* slab_start, const uint32 slab_size, const char* info) {

    if(fs_created_slab == NULL) {
        fs_created_slab = fs_create("Debug/allocators/slab/slabs_selective.txt");
        fs_append(fs_created_slab, "PRINT MEMORY OF A SLAB"
                  "________________________________________________________\n"
        );
    }

    fs_append(fs_created_slab, "# info: %s \n", info);
    fs_append_mem(fs_created_slab, slab_start, slab_size, "slab allocator: a slab");

}
