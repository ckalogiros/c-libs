/*
 * Created by ckalo on 17/6/26.
 *
 * This allocator DOES NOT handle data storage, it only handles the
 * internal pointers, offsets, alignment, reallocation etc, of the data to be stored.
 * The data storage (memcpy or any other implementation) is handled
 * at caller site.
 *
 * !!IMPORTANT!!
 * The data stored in a queue, is of variable size.
 * So alignment must be applied to all stored elements.
 * The problem is, that we never know the next stored element,
 * so in order to align properly, in each new data store we 
 * need to calculate the alignment from the previous stored data's end. 
 * BUT also we need to apply the alignment, to the previous offset in
 * the offsets table.
 * !!IMPORTANT!!
 *
 * !!IMPORTANT!!
 * -- ALIGNMENT --
 * Because of the allocators generic aproach, variable size data,
 * we must be very careful of the alignment rules.
 * - The maximum alignment is 8 bytes.
 * - The caller should always pass the alignment parameter using the alignof operator.
 *   That is, in case of passing a struct, the alignof operator will pass the alignment
 *   of the largest type of the struct(8 bytes maximum).
 * User's responsibility: the data structure to be stored
 *  struct BadExample {
 *     char c;
 *     int64 big;
 *  };
 *  In this case except from the fact that 7 bytes are pad for the struct its self,
 *  there is also the possibility of loosin another 7 bytes of storage, if the free
 *  memory address for the struct to be stored is misaligned.
 *  If next free memory is 0x1001, then the allocator will have to pad 7 bytes (0x1008),
 *  because the big variable could not start at any alignment except 0x1008 or 0x1016.
 *  The 0x1008 is not an option because of the structs internal alignment, if the char c
 *  would be stored at 0x1001 then the big would go to 0x1009, which is a crash.
 *
 * ISSUES:
 * # implement realloc() for queue_allocator_table
 * # implement realloc() for offsets table
 * # implement realloc() for data storage
 */

#include "queue_allocator.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "ascii_characters.h"
#include "logger.h"
#include "mem_man_interface.h"
#include "my_macros.h"
#include "alignment.h"

enum QUEUE_ALLOCATOR {
    QUEUE_ALLOCATOR_INSTANCE_INITIAL_TABLE_SIZE = 64,
    QUEUE_ALLOCATOR_INSTANCE_INITIAL_NUM_PAGES = 1,
};

static struct queue_allocator_instance* queue_allocator_table = NULL;
static uint32 table_count = 0;



/***************************************************************************************************
 * INTERNAL
 **************************************************************************************************/
memid_t
queue_allocator_create_new_instance(const pageid_t num_pages)
{
    const uint32 alignment = 64;


    /* case first time stack allocator instance creation */
    if(!queue_allocator_table) {
        queue_allocator_table = aligned_alloc(alignment, sizeof(struct queue_allocator_instance) * QUEUE_ALLOCATOR_INSTANCE_INITIAL_TABLE_SIZE);
    }

    /* create memory for allocator's offsets table */
    const uint32 offsets_table_alignment = sizeof(memoffset_t);
    const int32 offsets_table_num_pages = QUEUE_ALLOCATOR_INSTANCE_INITIAL_NUM_PAGES;
    const memid_t offsets_table_memid = memman_allocate_block(offsets_table_num_pages, offsets_table_alignment, "queue_allocator: offsets table");
    void* const offsets_table_address = memman_get_mem_start_by_memid(offsets_table_memid);

    /* create memory for data storage */
    const uint32 data_alignment = PAGE_SIZE;
    const memid_t data_memid = memman_allocate_block(num_pages, data_alignment, "queue_allocator: data storage");
    void* const data_address = memman_get_mem_start_by_memid(data_memid);

    /* FOR DEBUG */
    memset(data_address, 'Q', num_pages * PAGE_SIZE);

    struct queue_allocator_instance inst = {

        /* internal storage: offsets table */
        .offsets_table_start = offsets_table_address,
        .instance_id = offsets_table_memid,
        .offsets_table_size = offsets_table_num_pages * PAGE_SIZE,
        .offsets_table_count = 0,
        .front_ptr_offset = 0,       /* points to offsets table [0]*/
        .front_ptr_offset_index = 0, /* ISSUES */
        .back_ptr_offset  = 0,       /* points to offsets table [0]*/
        .back_ptr_offset_index = 0,  /* ISSUES */

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
        memoffset_t offset = inst.front_ptr_offset;


        for(uint32 i = 0; i < len; i++) {
            char** ptr1 = inst.offsets_table_start + offset;
            *(memoffset_t*) ptr1 = (memoffset_t) val;
            offset += sizeof(memoffset_t);
        }
    }

    const uint32 idx = table_count;
    queue_allocator_table[idx] = inst;
    table_count++;

    return idx;
}



/***************************************************************************************************
 * INTERFACE
 **************************************************************************************************/

void*
queue_allocator_alloc(const memid_t inst_id, const size_t size, const typesize_t alignment, const uint64 debug_flag)
{
    // assert(IS_POWER_OF_2(alignment)); // alignment value must be power of 2
    assert(queue_allocator_table);
    const uint32 aligned_size = ALIGN_SIZE(size, alignment);
    struct queue_allocator_instance* instance = &queue_allocator_table[inst_id];

    /* get front pointer */
    const char* fp_ptr = (char*)instance->data.start + instance->front_ptr_offset;

    /* align: align allocation, and previous allocation's offset */
    if(instance->front_ptr_offset_index > 0) {

        uint32 prev_count = instance->front_ptr_offset_index-1;
        const uint32 unaligned = align_pointer_to_type(fp_ptr, alignment);
        if(unaligned) {

            /* set table's previous offset to new alignement */
            char** offsets_table = instance->offsets_table_start + (sizeof(memoffset_t) * prev_count);
            *(memoffset_t*) offsets_table += unaligned;

            /* advance front pointer to aligned offset */
            instance->front_ptr_offset += unaligned;
        }

    }

    const uint32 padding_size = 0;

    void* free_address = (char*) instance->data.start + instance->front_ptr_offset + padding_size;

    /* set current offset to table */
    char** offsets_table = instance->offsets_table_start + (sizeof(memoffset_t) * instance->front_ptr_offset_index);
    *(memoffset_t*) offsets_table = aligned_size;

    instance->data.used_size += aligned_size;
    /* advance the front pointer */
    instance->front_ptr_offset += aligned_size;
    instance->front_ptr_offset_index++;
    instance->offsets_table_count++;

    memset(free_address, debug_flag, aligned_size);

    return (void*) free_address;
}

void*
queue_allocator_release(const memid_t inst_id)
{
    struct queue_allocator_instance* instance = &queue_allocator_table[inst_id];

    if(instance->offsets_table_count == 0)
        return NULL;

    instance->offsets_table_count--;

    memoffset_t offset_size = *(memoffset_t*) (instance->offsets_table_start + (sizeof(memoffset_t) * instance->back_ptr_offset_index));
    /// grab the first element of the queue
    void* free_address = (char*) instance->data.start + instance->back_ptr_offset;


    /// increment back pointer to the next element in the queue
    instance->back_ptr_offset += offset_size;
    /// increment back pointer ofssets table index to the next
    instance->back_ptr_offset_index++;
    instance->data.used_size -= offset_size;






    /* debug-mark released memory with value */
    // memset(free_address, 0xCD, offset_size);

    return (void*) free_address;
}

/* Get the size of the fist stored element(the one to be popped/dequed next)
 * Used in case the caller does not "know" the size */
memoffset_t queue_allocator_get_last_elem_size(const memid_t inst_id)
{
    struct queue_allocator_instance* instance = &queue_allocator_table[inst_id];
    memoffset_t offset_size = *(memoffset_t*) (instance->offsets_table_start + (sizeof(memoffset_t) * (instance->back_ptr_offset_index)));
    return offset_size;
}




/***************************************************************************************************
 * TEST-DEBUG
 **************************************************************************************************/

struct queue_allocator_instance*
debug_get_queue_allocator_table()
{return queue_allocator_table;}

struct queue_allocator_instance*
debug_get_queue_allocator_instance(const memid_t inst_id)
{return &queue_allocator_table[inst_id];}

void*
debug_get_queue_allocator_instance_start_address(const memid_t inst_id)
{return queue_allocator_table[inst_id].data.start;}

uint32 debug_get_queue_allocator_init_table_size()
{return QUEUE_ALLOCATOR_INSTANCE_INITIAL_TABLE_SIZE;}

uint32 debug_get_queue_allocator_init_instance_num_pages()
{return QUEUE_ALLOCATOR_INSTANCE_INITIAL_NUM_PAGES*PAGE_SIZE;}
