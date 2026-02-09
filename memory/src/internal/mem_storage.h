/*
 * Created by ckalo on 2/7/26.
 */
#ifndef MEMORY_MEM_STORAGE_H
#define MEMORY_MEM_STORAGE_H
#include "my_types.h"

// TODO: correct message
/***************************************************************************************************
 * memory storage layout *
 *
 * memory manager deals with a buffer of memory chunks
 * mem_table:[chunk][chunk][chunk][chunk][chunk]...
 * mem_table buffer stores data for:
 *      # capacity
 *      # count
 *      # flags: (in_use, the user(enum),...)
 * functionality:
 *      # alloc(chunk)
 *      # release(chunk)
 *      #
 *
 * for now, we preallocate 512 PAGES of size 1024 bytes.
 * each chunk is 32 PAGES(32768b). devide 512/32=16 chunks.
 * these 16 chunks are contiguous memory.
 * On further allocations, we'll need to malloc new chunks.
 *
 * memory chunk *
 * memory manager handles memory in PAGES (min alloc: 1 PAGE=1024b).
 * it also handles each chunk separately.
 * each chunk stores data for:
 *      memory in use: buffer
 *      free list, per PAGE: list stored in memory PAGES
 *
 * memory manager uses internal storage for:
 *      # mem_table: struct mem[]
 *      # mem_chunk: struct mem_chunk
 *      # mem_used: struct
 *
 **************************************************************************************************/


#endif //MEMORY_MEM_STORAGE_H