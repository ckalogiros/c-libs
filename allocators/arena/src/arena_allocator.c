/*
 * Created by ckalo on 3/14/26.
 *
 * TODO:
 * # implement realloc() for arena_allocator_table
 * # implement realloc() for offsets table
 * # implement realloc() for data storage
 */

#include "arena_allocator.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "ascii_characters.h"
#include "logger.h"
#include "mem_man_interface.h"
#include "my_macros.h"
#include "alignment.h"

enum ARENA_ALLOCATOR {
    ARENA_ALLOCATOR_INSTANCE_INITIAL_TABLE_SIZE = 64,
    ARENA_ALLOCATOR_INSTANCE_INITIAL_NUM_PAGES = 1,
};

static struct arena_allocator_instance* arena_allocator_table = NULL;
static uint32 table_count = 0;



/***************************************************************************************************
 * INTERNAL
 **************************************************************************************************/
memid_t
arena_allocator_create_new_instance(const pageid_t num_pages)
{
    const uint32 struct_type_size = sizeof(struct arena_allocator_instance);
    const size_t mem_address_alignment = 1024;

    /* case first time stack allocator instance creation */
    if(!arena_allocator_table) {
        arena_allocator_table = aligned_alloc(mem_address_alignment, mem_address_alignment * ARENA_ALLOCATOR_INSTANCE_INITIAL_TABLE_SIZE);
    }

    /* create memory for data storage */
    const uint32 data_alignment = PAGE_SIZE;
    const memid_t data_memid = memman_allocate_block(num_pages, data_alignment, "arena_allocator: data storage");
    void* const data_address = memman_get_mem_start_by_memid(data_memid);

    /* FOR DEBUG */
    memset(data_address, 'Q', num_pages * PAGE_SIZE);

    struct arena_allocator_instance inst = {

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
        // const uint64 val = (ASCII_F << 8) | ASCII_R; /* ascii:"FR" */
        // /* get stack pointer */
        // const char* p_addr = (char*)inst.data.start + inst.stack_ptr_offset;
        // memcpy(p_addr, &val, inst.data.size);
    }

    const uint32 idx = table_count;
    arena_allocator_table[idx] = inst;
    table_count++;

    return idx;
}



/***************************************************************************************************
 * INTERFACE
 **************************************************************************************************/

void*
arena_allocator_alloc(const memid_t inst_id, const size_t size, const typesize_t alignment)
{
    assert(alignment <= SIZE_I64);    /// limit maximum alignement to 8 bytes
    assert(IS_POWER_OF_2(alignment)); /// alignment value must be power of 2
    assert(arena_allocator_table);
    const uint32 aligned_size = ALIGN_SIZE(size, alignment);
    struct arena_allocator_instance* instance = &arena_allocator_table[inst_id];

    /* get stack pointer */
    const char* p_addr = (char*)instance->data.start + instance->stack_ptr_offset;

    /* align: align allocation, and previous allocation's offset */
    const uint32 unaligned = align_pointer_to_type(p_addr, alignment);
    if(unaligned) {
        /* advance stack pointer to aligned offset */
        instance->stack_ptr_offset += unaligned;
    }

    void* free_address = (char*) instance->data.start + instance->stack_ptr_offset;
    instance->data.used_size += aligned_size;
    /* advance the stack pointer */
    instance->stack_ptr_offset += aligned_size;

    char* p = free_address + size;
    *p = '\0';

    return (void*) free_address;
}

void*
arena_allocator_release(const memid_t inst_id)
{
    struct arena_allocator_instance* instance = &arena_allocator_table[inst_id];

    // void* free_address = (char*) instance->data.start + instance->stack_ptr_offset;
    void* free_address = (char*) instance->data.start;

    instance->data.used_size = 0;
    instance->data.start = instance->data.start;
    instance->stack_ptr_offset = 0;


    /* debug-mark released memory with value */
    // memset(free_address, 0xCD, instance->data.size);

    return (void*) free_address;
}



/***************************************************************************************************
 * TEST-DEBUG
 **************************************************************************************************/

struct arena_allocator_instance*
debug_get_arena_allocator_table()
{return arena_allocator_table;}

struct arena_allocator_instance*
debug_get_arena_allocator_instance(const memid_t inst_id)
{return &arena_allocator_table[inst_id];}

void*
debug_get_arena_allocator_instance_start_address(const memid_t inst_id)
{return arena_allocator_table[inst_id].data.start;}

uint32 debug_get_arena_allocator_init_table_size()
{return ARENA_ALLOCATOR_INSTANCE_INITIAL_TABLE_SIZE;}

uint32 debug_get_arena_allocator_init_instance_num_pages()
{return ARENA_ALLOCATOR_INSTANCE_INITIAL_NUM_PAGES*PAGE_SIZE;}
