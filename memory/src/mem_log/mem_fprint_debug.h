/*
 * Created by ckalo on 8/23/26.
 */
#ifndef MEMORY_FPRINT_DEBUG_H
#define MEMORY_FPRINT_DEBUG_H

#include "my_types.h"
#include "mem_man_structs.h"



/* debug-fprint the memory pool buffer */
void mem_fprint_append_mem_pool(struct mem_block* mem_pool, const uint32 capacity,
                                       const uint32 size, const uint32 allocated_size);

struct fs_t;
void mem_fprint_append_mem_block_to_file(struct fs_t* fs, const memid_t memid);

void mem_fprint_append_mem_block(const memid_t memid);

void mem_fprint_append_mem_block_poison(const memid_t memid);

void mem_fprint_append_all_mem_block_poison(struct mem_block* mem_pool, const uint32 capacity);

#endif //MEMORY_FPRINT_DEBUG_H