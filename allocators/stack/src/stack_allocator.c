/*
 * Created by ckalo on 3/14/26.
 *
 * TODO:
 * # implement realloc() for stack_allocator_table
 * # implement realloc() for offsets table
 * # implement realloc() for data storage
 */

#include "stack_allocator.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "ascii_characters.h"
#include "logger.h"
#include "mem_man_interface.h"
#include "my_macros.h"
#include "alignment.h"

enum STACK_ALLOCATOR {
    STACK_ALLOCATOR_INSTANCE_INITIAL_TABLE_SIZE = 64,
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
    // print("%s###################################################\n"
    //   "STACK CREATE NEW INSTANCE: %s\n", br3, CLEAR);

    // const uint32 struct_type_size = sizeof(struct stack_allocator_instance);
    const uint32 struct_type_size = 1024;

    /* case first time stack allocator instance creation */
    if(!stack_allocator_table) {
        stack_allocator_table = aligned_alloc(struct_type_size, struct_type_size * STACK_ALLOCATOR_INSTANCE_INITIAL_TABLE_SIZE);
    }

    /* create memory for allocator's offsets table */
    const uint32 offsets_table_alignment = struct_type_size;
    const int32 offsets_table_num_pages = STACK_ALLOCATOR_INSTANCE_INITIAL_NUM_PAGES;
    const memid_t offsets_table_memid = memman_allocate_block(offsets_table_num_pages, offsets_table_alignment, "stack_allocator: offsets table");
    void* const offsets_table_address = memman_get_mem_start_by_memid(offsets_table_memid);

    /* create memory for data storage */
    const uint32 data_alignment = PAGE_SIZE;
    const memid_t data_memid = memman_allocate_block(num_pages, data_alignment, "stack_allocator: data storage");
    void* const data_address = memman_get_mem_start_by_memid(data_memid);

    /* FOR DEBUG */
    memset(data_address, 'Q', num_pages * PAGE_SIZE);

    struct stack_allocator_instance inst = {

        /* internal storage: offsets table */
        .offsets_table_start = offsets_table_address,
        .instance_id = offsets_table_memid,
        .offsets_table_size = offsets_table_num_pages * PAGE_SIZE,
        .offsets_table_count = 0,
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
        const uint32 len = (inst.offsets_table_size / sizeof(memoffset_t));
        memoffset_t offset = inst.stack_ptr_offset;


        for(uint32 i = 0; i < len; i++) {
            char** ptr1 = inst.offsets_table_start + offset;
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
 * INTERFACE
 **************************************************************************************************/

void*
stack_allocator_alloc(const memid_t inst_id, const size_t size, const typesize_t alignment)
{
    assert(IS_POWER_OF_2(alignment)); // alignment value must be power of 2
    assert(stack_allocator_table);
    const uint32 aligned_size = ALIGN_SIZE(size, alignment);
    struct stack_allocator_instance* instance = &stack_allocator_table[inst_id];

    /* get stack pointer */
    const char* sp_addr = (char*)instance->data.start + instance->stack_ptr_offset;

    /* align: align allocation, and previous allocation's offset */
    if(instance->offsets_table_count > 0) {

        uint32 prev_count = instance->offsets_table_count-1;
        const uint32 unaligned = align_pointer_to_type(sp_addr, alignment);
        if(unaligned) {

            /* set table's previous offset to new alignement */
            char** offsets_table = instance->offsets_table_start + (sizeof(memoffset_t) * prev_count);
            *(memoffset_t*) offsets_table += unaligned;

            /* advance stack pointer to aligned offset */
            instance->stack_ptr_offset += unaligned;
        }

    }

    const uint32 padding_size = 0;

    void* free_address = (char*) instance->data.start + instance->stack_ptr_offset + padding_size;


    /* set current offset to table */
    char** offsets_table = instance->offsets_table_start + (sizeof(memoffset_t) * instance->offsets_table_count);
    *(memoffset_t*) offsets_table = aligned_size;

    instance->data.used_size += aligned_size;
    /* advance the stack pointer */
    instance->stack_ptr_offset += aligned_size;
    instance->offsets_table_count++;

    return (void*) free_address;
}

void*
stack_allocator_release(const memid_t inst_id)
{
    struct stack_allocator_instance* instance = &stack_allocator_table[inst_id];

    if(instance->offsets_table_count == 0)
        return NULL;

    instance->offsets_table_count--;

    memoffset_t offset_size = *(memoffset_t*) (instance->offsets_table_start + (sizeof(memoffset_t) * instance->offsets_table_count));
    instance->stack_ptr_offset -= offset_size;
    instance->data.used_size -= offset_size;

    void* free_address = (char*) instance->data.start + instance->stack_ptr_offset;

    /* debug-mark released memory with value */
    // memset(free_address, 0xCD, offset_size);

    return (void*) free_address;
}


/* Get the size of the last stored element(the one to be popped() next)
 * In case the caller does not "know" the size */
memoffset_t stack_allocator_get_last_elem_size(const memid_t inst_id)
{
    struct stack_allocator_instance* instance = &stack_allocator_table[inst_id];
    memoffset_t offset_size = *(memoffset_t*) (instance->offsets_table_start + (sizeof(memoffset_t) * (instance->offsets_table_count-1)));
    return offset_size;
}


/***************************************************************************************************
 * TEST-DEBUG
 **************************************************************************************************/

struct stack_allocator_instance*
debug_get_stack_allocator_table()
{return stack_allocator_table;}

struct stack_allocator_instance*
debug_get_stack_allocator_instance(const memid_t inst_id)
{return &stack_allocator_table[inst_id];}

void*
debug_get_stack_allocator_instance_start_address(const memid_t inst_id)
{return stack_allocator_table[inst_id].data.start;}

uint32 debug_get_stack_allocator_init_table_size()
{return STACK_ALLOCATOR_INSTANCE_INITIAL_TABLE_SIZE;}

uint32 debug_get_stack_allocator_init_instance_num_pages()
{return STACK_ALLOCATOR_INSTANCE_INITIAL_NUM_PAGES*PAGE_SIZE;}
