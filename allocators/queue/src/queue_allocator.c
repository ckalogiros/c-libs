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
#include <sanitizer/common_interface_defs.h>
#include "ascii_characters.h"
#include "logger.h"
#include "mem_man_interface.h"
#include "my_macros.h"
#include "alignment.h"
#include "str_utils.h"
#include "sys_utils.h"


static struct queue_allocator_instance* queue_allocator_table = NULL;
static uint32 instance_count = 0;


void queue_allocator_table_init(void)
{
    const uint32 capacity   = QUEUE_ALLOCATOR_INSTANCE_INITIAL_TABLE_COUNT;
    const uint32 size       = sizeof(struct queue_allocator_instance)*capacity;
    const uint32 alloc_size = ALIGN_SIZE(size, QUEUE_ALLOCATOR_MEMORY_ALIGNMENT);
    queue_allocator_table   = aligned_alloc(QUEUE_ALLOCATOR_MEMORY_ALIGNMENT, alloc_size);
    // memset(mem_pool, 0x0, alloc_size);

#   ifdef ENABLE_SANITIZER
    /* enable santizers to the stderr out */
    __sanitizer_set_report_path("stderr");
#   endif

    print("%s# INITIALIZE queue_allocator_table #%s\n\n", YELLOW, CLEAR);
    print("%s# Allocate queue_allocator_table: %p %s\n", YELLOW, queue_allocator_table, CLEAR);
    print(
        " capacity:        %s%u  %s\n"
        " size bytes:      %s%lu %s\n"
        " aloc size:       %s%lu %s\n"
        " data start:      %s%x%s\n",
        GREEN, capacity,    CLEAR,
        YELLOW, size,       CLEAR,
        YELLOW, alloc_size, CLEAR,
        CYAN, REDUCE_ADDR((char*)queue_allocator_table, F6), CLEAR
    );

}
void queue_allocator_table_realloc(){} /// TODO: implement



/***************************************************************************************************
 * INTERFACE
 **************************************************************************************************/
memid_t queue_allocator_create_new_instance(const pageid_t num_pages)
{
    /* case first time stack allocator instance creation */
    if(!queue_allocator_table) {
        queue_allocator_table_init();
    }

    /* create memory for allocator's offsets table */
    const uint32 offsets_table_alignment = QUEUE_ALLOCATOR_MEMORY_ALIGNMENT;
    const int32 offsets_table_num_pages  = QUEUE_ALLOCATOR_OFFSETS_INITIAL_NUM_PAGES;
    const memid_t offsets_table_memid    = memman_allocate_block(offsets_table_num_pages, offsets_table_alignment, "queue_allocator: offsets table");
    struct memhnd_t table_handle         = memman_page_request(offsets_table_num_pages, offsets_table_memid);
    void* offsets_table_address = memman_get_alloc_mem_by_handle(table_handle);

    /* create memory for data storage */
    const uint32 data_alignment = QUEUE_ALLOCATOR_MEMORY_ALIGNMENT;
    const memid_t data_memid    = memman_allocate_block(num_pages, data_alignment, "queue_allocator: data storage");
    struct memhnd_t data_handle = memman_page_request(num_pages, data_memid);
    void* data_address          = memman_get_alloc_mem_by_handle(data_handle);


    struct queue_allocator_instance inst = {

        /* internal storage: offsets table */
        .offsets_table_start = offsets_table_address,
        .instance_id = offsets_table_memid,
        .offsets_table_size = offsets_table_num_pages * PAGE_SIZE,
        .offsets_table_count = 0,
        .front_ptr_offset = 0,       /* points to offsets table [0]*/
        .front_ptr_elems_counter = 0, /* ISSUES */
        .back_ptr_offset  = 0,       /* points to offsets table [0]*/
        .back_ptr_elems_counter = 0,  /* ISSUES */
        .data = {                    /* data storage */
            .start = data_address,
            .mem_id = data_memid,
            .size = num_pages * PAGE_SIZE,
            .used_size = 0,
        },
    };


    /* FOR DEBUG */
    char c[8] = {'Q','U','E','U','E', '\0', '\0', '\0'};
    str_conv_int32_to_str(&c[5], data_memid);
    uint64 s = make_uint64_from_string_max_8chars(c, 8);
    memset_64bit(data_address, s, num_pages * PAGE_SIZE);
    // memset(data_address, 0x0, num_pages * PAGE_SIZE);

    const uint32 idx = instance_count;
    queue_allocator_table[idx] = inst;
    instance_count++;

    return idx;
}

void* queue_allocator_alloc(const memid_t inst_id, const size_t size, const typesize_t alignment)
{
    // assert(IS_POWER_OF_2(alignment)); // alignment value must be power of 2
    assert(queue_allocator_table);
    const uint32 aligned_size = ALIGN_SIZE(size, alignment);
    struct queue_allocator_instance* instance = &queue_allocator_table[inst_id];

    assert(instance->offsets_table_count < QUEUE_ALLOCATOR_INSTANCE_INITIAL_TABLE_COUNT);


    /* get front pointer */
    const char* fp_ptr = (char*)instance->data.start + instance->front_ptr_offset;

    /* align: align allocation, and previous allocation's offset */
    if(instance->front_ptr_elems_counter > 0) {

        uint32 prev_count = instance->front_ptr_elems_counter-1;
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
    char** offsets_table = instance->offsets_table_start + (sizeof(memoffset_t) * instance->front_ptr_elems_counter);
    *(memoffset_t*) offsets_table = aligned_size;

    instance->data.used_size += aligned_size;
    /* advance the front pointer */
    instance->front_ptr_offset += aligned_size;
    instance->front_ptr_elems_counter++;
    instance->offsets_table_count++;
    // print("%s alloc:offsets_table_count:%u %s\n", YELLOW, instance->offsets_table_count, CLEAR);

    // memset(free_address, 0x0, aligned_size);

    return (void*) free_address;
}

void* queue_allocator_dequeue(const memid_t inst_id)
{
    struct queue_allocator_instance* instance = &queue_allocator_table[inst_id];

    assert(instance->offsets_table_count != 0);

    instance->offsets_table_count--;
    // print("%s release:offsets_table_count:%u %s\n", YELLOW, instance->offsets_table_count, CLEAR);

    memoffset_t offset_size = *(memoffset_t*) (instance->offsets_table_start + (sizeof(memoffset_t) * instance->back_ptr_elems_counter));
    /// grab the first element of the queue
    void* free_address = (char*) instance->data.start + instance->back_ptr_offset;

    /* If this is the last element in the queue:
     * After getting the return address
     * we reset the queue to start storing
     * from the begining of the allocatd memory again
     */
    if(instance->offsets_table_count == 0) {
        instance->data.used_size          = 0;
        instance->back_ptr_offset         = 0;
        instance->front_ptr_offset        = 0;
        instance->back_ptr_elems_counter  = 0;
        instance->front_ptr_elems_counter = 0;
        // memset(free_address, 0x0, instance->data.size);
        return (void*) free_address;
    }

    /// increment back pointer to the next element in the queue
    instance->back_ptr_offset += offset_size;
    /// increment back pointer ofssets table index to the next
    instance->back_ptr_elems_counter++;
    instance->data.used_size -= offset_size;

    return (void*) free_address;
}

/* reset front and back poiners, zero the memory */
void* queue_allocator_reset(const memid_t inst_id)
{
    struct queue_allocator_instance* instance = &queue_allocator_table[inst_id];

    void* free_address = (char*) instance->data.start;

    instance->data.used_size = 0;

    instance->back_ptr_offset = 0;
    instance->front_ptr_offset = 0;
    instance->back_ptr_elems_counter = 0;
    instance->front_ptr_elems_counter = 0;
    memset(free_address, 0x0, instance->data.size);

    /* debug-mark released memory with value */
    // memset(free_address, 0xCD, instance->data.size);

    return (void*) free_address;
}

/* Get the size of the fist stored element(the one to be popped/dequed next)
 * Used in case the caller does not "know" the size */
memoffset_t queue_allocator_get_last_elem_size(const memid_t inst_id)
{
    struct queue_allocator_instance* instance = &queue_allocator_table[inst_id];
    memoffset_t offset_size = *(memoffset_t*) (instance->offsets_table_start + (sizeof(memoffset_t) * (instance->back_ptr_elems_counter)));
    return offset_size;
}



/***************************************************************************************************
 * GETTERS
 **************************************************************************************************/
queue_size_t queue_allocator_offsets_table_get_size(const queue_table_t inst_id)
{return queue_allocator_table[inst_id].offsets_table_size;}
queue_size_t queue_allocator_offsets_table_get_count(const queue_table_t inst_id)
{return queue_allocator_table[inst_id].offsets_table_count;}

queue_size_t queue_allocator_instance_data_get_size(const queue_table_t inst_id)
{return queue_allocator_table[inst_id].data.size;}
queue_size_t queue_allocator_instance_data_get_used_size(const queue_table_t inst_id)
{return queue_allocator_table[inst_id].data.used_size;}



/***************************************************************************************************
 * TEST-DEBUG
 **************************************************************************************************/

struct queue_allocator_instance* debug_queue_allocator_get_table()
{return queue_allocator_table;}

struct queue_allocator_instance* debug_queue_allocator_get_instance(const memid_t inst_id)
{return &queue_allocator_table[inst_id];}

void* debug_queue_allocator_instance_get_data(const memid_t inst_id)
{return queue_allocator_table[inst_id].data.start;}

uint32 debug_queue_allocator_get_table_size()
{return QUEUE_ALLOCATOR_INSTANCE_INITIAL_TABLE_COUNT;}

