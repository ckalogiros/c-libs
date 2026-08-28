/*
 * Created by ckalo on 8/26/26.
 */
#ifndef SLAB_ALLOCATOR_MACROS_H
#define SLAB_ALLOCATOR_MACROS_H


/***************************************************************************************************
 * FUNCTION MACROS
 **************************************************************************************************/
#define SLABS_FREE_LIST_GET_PAGE_COLOR_START(fnode) ((char*)fnode + sizeof(struct flist))

#define CHECK_NULLPTR(table) \
if(table == NULL) { slab_allocator_print_error(SLAB_ALLOCATOR_MEMCACHE_NO_INIT); }

// /* enable file print debugging */
// #define SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
// #ifdef SLAB_ALLOCATOR_FPRINT_DEBUG_ENABLED
// #   include "slab_allocator_fprint.h"
// #endif

#endif //SLAB_ALLOCATOR_MACROS_H