/*
 * Created by ckalo on 3/14/26.
 *
 * TODO:
 * # implement realloc() for stack_allocator_table
 * # implement realloc() for offsets table
 * # implement realloc() for data storage
 */

#include "stack_allocator.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "ascii_characters.h"
#include "logger.h"
#include "mem_man_interface.h"

enum STACK_ALLOCATOR {
    STACK_ALLOCATOR_INSTANCE_INITIAL_NUM_PAGES = 1,
};

static struct stack_allocator_instance* stack_allocator_table = NULL;
static uint32 table_count = 0;


/***************************************************************************************************
 * INTERNAL
 **************************************************************************************************/
memid_t
stack_allocator_create_new_instance(const pageid_t num_pages)
{
    const uint32 struct_type_size = sizeof(struct stack_allocator_instance);

    /* case first time stack allocator instance creation */
    if(!stack_allocator_table) {
        stack_allocator_table = aligned_alloc(struct_type_size, struct_type_size * 64);
    }

    /* create memory for allocator's offsets table */
    const uint32 offsets_table_alignment = struct_type_size;
    const int32 offsets_table_num_pages = STACK_ALLOCATOR_INSTANCE_INITIAL_NUM_PAGES;
    const memid_t offsets_table_memid = memman_alloc_create(offsets_table_num_pages, offsets_table_alignment);
    void* const offsets_table_address = memman_get_mem_start_by_memid(offsets_table_memid);

    /* create memory for data storage */
    const uint32 data_alignment = PAGE_SIZE;
    const memid_t data_memid = memman_alloc_create(num_pages, data_alignment);
    void* const data_address = memman_get_mem_start_by_memid(data_memid);

    /* FOR DEBUG */
    memset(data_address, 'Q', num_pages * PAGE_SIZE);

    struct stack_allocator_instance inst = {

        /* internal storage: offsets table */
        .table_start = offsets_table_address,
        .instance_id = offsets_table_memid,
        .table_size = offsets_table_num_pages * PAGE_SIZE,
        .table_count = 0,
        .stack_ptr_offset = 0, /* points to offsets table [0]*/

        .data = { /* data storage */
            .start = data_address,
            .instance_id = data_memid,
            .size = num_pages * PAGE_SIZE,
            .used_size = 0,
        },
    };

    /* FOR DEBUG.
     * temporarily mark the memory with a value */
    {
        const memoffset_t val = (ASCII_F << 8) | ASCII_R; /* ascii:"FR" */

        /* calculate the rest of the allocated memory, in size of address offsets */
        const uint32 len = (inst.table_size / sizeof(memoffset_t));
        memoffset_t offset = inst.stack_ptr_offset;


        for(uint32 i = 0; i < len; i++) {
            char** ptr1 = inst.table_start + offset;
            *(memoffset_t*) ptr1 = (memoffset_t) val;
            offset += sizeof(memoffset_t);
        }
    }

    const uint32 idx = table_count;
    stack_allocator_table[idx] = inst;
    table_count++;

    return idx;
}


/***************************************************************************************************
 *
 **************************************************************************************************/

void*
stack_allocator_alloc(const memid_t inst_id, const size_t size)
{
    struct stack_allocator_instance* instance = &stack_allocator_table[inst_id];

    void* free_address = (char*) instance->data.start + instance->stack_ptr_offset;

    instance->data.used_size += size;
    instance->stack_ptr_offset += size;
    char** offsets_table = instance->table_start + (sizeof(memoffset_t) * instance->table_count);
    *(memoffset_t*) offsets_table = size;
    instance->table_count++;

    return (void*) free_address;
}

void*
stack_allocator_release(const memid_t inst_id)
{
    struct stack_allocator_instance* instance = &stack_allocator_table[inst_id];

    instance->table_count--;

    memoffset_t offset_size = *(memoffset_t*) (instance->table_start + (sizeof(memoffset_t) * instance->table_count));
    instance->stack_ptr_offset -= offset_size;
    instance->data.used_size -= offset_size;

    void* free_address = (char*) instance->data.start + instance->stack_ptr_offset;

    return (void*) free_address;
}
