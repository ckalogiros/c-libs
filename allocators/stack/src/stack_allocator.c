/*
 * Created by ckalo on 3/14/26.
 *
 * ISSUES:
 * # implement realloc() for stack_allocator_table
 * # implement realloc() for offsets table
 * # implement realloc() for data storage
 */

#include "stack_allocator.h"
#include "stack_allocator_macros.h"
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


/***************************************************************************************************
 * STATIC SCOPE VARIABLES
 * SLAB ALLOCATOR INSTANCES TABLE
 **************************************************************************************************/
static struct stack_allocator_instance* stack_allocator_table = NULL;
static uint32 instance_count = 0;
struct stack_allocator_instance* stack_allocator_table_get() { return stack_allocator_table; }

void stack_allocator_table_init(void)
{
    const uint32 capacity   = STACK_ALLOCATOR_INSTANCE_INITIAL_TABLE_COUNT;
    const uint32 size       = sizeof(struct stack_allocator_instance)*capacity;
    const uint32 alloc_size = ALIGN_SIZE(size, STACK_ALLOCATOR_MEMORY_ALIGNMENT);
    stack_allocator_table   = aligned_alloc(STACK_ALLOCATOR_MEMORY_ALIGNMENT, alloc_size);

#   ifdef ENABLE_SANITIZER
    /* enable santizers to the stderr out */
    __sanitizer_set_report_path("stderr");
#   endif


#   ifdef STACK_ALLOCATOR_FPRINT_DEBUG_ENABLED
    print("%s# INITIALIZE STACK ALLOCATOR #%s\n\n", YELLOW, CLEAR);
    print("%s# Allocate stack allocator table: %p %s\n", YELLOW, stack_allocator_table, CLEAR);
    print("--%s memory pool buffer %s--\n", ry, CLEAR);
    print(
        " capacity:        %s%u  %s\n"
        " size bytes:      %s%lu %s\n"
        " aloc size:       %s%lu %s\n"
        " data start:      %s%x%s\n",
        GREEN,  capacity,   CLEAR,
        YELLOW, size,       CLEAR,
        YELLOW, alloc_size, CLEAR,
        CYAN, REDUCE_ADDR((char*)stack_allocator_table, F6), CLEAR
    );
#   endif

}
void stack_allocator_table_realloc(){} /// TODO: implement


struct stack_allocator_instance*
stack_allocator_get_instance(const stack_table_t inst_id)
{

    assert(stack_allocator_table != NULL);
    assert(inst_id < instance_count);
    assert(&stack_allocator_table[inst_id] != NULL);

    struct stack_allocator_instance* inst =  &stack_allocator_table[inst_id];

    return inst;
}



/***************************************************************************************************
 * INTERFACE
 **************************************************************************************************/
stack_table_t stack_allocator_create_new_instance(const pageid_t num_pages)
{
    /* case first time stack allocator instance creation */
    if(!stack_allocator_table) {
        stack_allocator_table_init();
    }

    /* create memory for allocator's offsets table */
    const uint32 offsets_table_alignment = STACK_ALLOCATOR_MEMORY_ALIGNMENT;
    const int32 offsets_table_num_pages  = STACK_ALLOCATOR_OFFSETS_INITIAL_NUM_PAGES;
    const stack_table_t offsets_table_memid =
        memman_allocate_block(offsets_table_num_pages, offsets_table_alignment, "stack_allocator: offsets table");
    const struct memhnd_t hnd1  = memman_page_request(offsets_table_num_pages, offsets_table_memid);
    char* offsets_table_address = memman_get_alloc_mem_by_handle(hnd1);


    /* create memory for data storage */
    const uint32 data_alignment = STACK_ALLOCATOR_MEMORY_ALIGNMENT;
    const stack_table_t data_memid    = memman_allocate_block(num_pages, data_alignment, "stack_allocator: data storage");
    const struct memhnd_t hnd2  = memman_page_request(num_pages, data_memid);
    char* data_address          = memman_get_alloc_mem_by_handle(hnd2);

    /* FOR DEBUG */
    char c[8] = {'S','T','A','C','K', '\0', '\0', '\0'};
    str_conv_int32_to_str(&c[5], data_memid);
    uint64 s = make_uint64_from_string_max_8chars(c, 8);
    memset_64bit(data_address, s, num_pages * PAGE_SIZE);
    // memset(data_address, 0x0, num_pages * PAGE_SIZE);

    struct stack_allocator_instance inst = {

        /* internal storage: offsets table */
        .offsets_table_start = offsets_table_address,
        .instance_id = offsets_table_memid,
        .offsets_table_size = offsets_table_num_pages * PAGE_SIZE,
        .offsets_table_count = 0,
        .sp_offset = 0, /* points to offsets table [0] */
        .data = {       /* data storage */
            .start = data_address,
            .instance_id = data_memid,
            .size = num_pages * PAGE_SIZE,
            .used_size = 0,
        },
    };

    /* FOR DEBUG.
     * temporarily mark offsets table memory with a value */
    {
        const memoffset_t val = (ASCII_F << 8) | ASCII_R; /* ascii:"FR" */

        /* calculate the rest of the allocated memory, in size of address offsets */
        const uint32 len = (inst.offsets_table_size / sizeof(memoffset_t));
        memoffset_t offset = inst.sp_offset;

        for(uint32 i = 0; i < len; i++) {
            char** ptr1 = inst.offsets_table_start + offset;
            *(memoffset_t*) ptr1 = (memoffset_t) val;
            offset += sizeof(memoffset_t);
        }
    }

    const uint32 idx = instance_count;
    stack_allocator_table[idx] = inst;
    instance_count++;

    return idx;
}

void* stack_allocator_alloc(const stack_table_t inst_id, const stack_size_t size, const typesize_t type_size)
{
    assert(IS_POWER_OF_2(type_size)); // alignment value must be power of 2
    assert(stack_allocator_table);
    assert(instance_count < STACK_ALLOCATOR_INSTANCE_INITIAL_TABLE_COUNT);
    const uint32 aligned_size = ALIGN_SIZE(size, type_size);
    struct stack_allocator_instance* instance = &stack_allocator_table[inst_id];

    /* get stack pointer */
    const char* sp_addr = (char*)instance->data.start + instance->sp_offset;

    /* align: align allocation, and previous allocation's offset */
    if(instance->offsets_table_count > 0) {

        uint32 prev_count = instance->offsets_table_count-1;
        const uint32 unaligned = align_pointer_to_type(sp_addr, type_size);
        if(unaligned) {

            /* set table's previous offset to new alignement */
            char** offsets_table = instance->offsets_table_start + (sizeof(memoffset_t) * prev_count);
            *(memoffset_t*) offsets_table += unaligned;

            /* advance stack pointer to aligned offset */
            instance->sp_offset += unaligned;
        }

    }

    const uint32 padding_size = 0;

    void* free_address = (char*) instance->data.start + instance->sp_offset + padding_size;


    /* set current offset to table */
    char** offsets_table = instance->offsets_table_start + (sizeof(memoffset_t) * instance->offsets_table_count);
    *(memoffset_t*) offsets_table = aligned_size;

    instance->data.used_size += aligned_size;
    /* advance the stack pointer */
    instance->sp_offset += aligned_size;
    instance->offsets_table_count++;

    return (void*) free_address;
}

void* stack_allocator_pop(const stack_table_t inst_id)
{
    struct stack_allocator_instance* instance = &stack_allocator_table[inst_id];

    if(instance->offsets_table_count == 0)
        return NULL;

    instance->offsets_table_count--;

    memoffset_t offset_size = *(memoffset_t*) (instance->offsets_table_start + (sizeof(memoffset_t) * instance->offsets_table_count));
    instance->sp_offset -= offset_size;
    instance->data.used_size -= offset_size;

    void* free_address = (char*) instance->data.start + instance->sp_offset;

    /* debug-mark released memory with value */
    // memset(free_address, 0xCD, offset_size);

    return (void*) free_address;
}

void stack_allocator_destroy(const stack_table_t inst_id)
{
    memman_deallocate_block(inst_id);
}

/* Get the size of the last stored element(the one to be popped() next)
 * In case the caller does not "know" the size */
memoffset_t stack_allocator_get_last_elem_size(const stack_table_t inst_id)
{
    struct stack_allocator_instance* instance = &stack_allocator_table[inst_id];
    memoffset_t offset_size = *(memoffset_t*) (instance->offsets_table_start + (sizeof(memoffset_t) * (instance->offsets_table_count-1)));
    return offset_size;
}



/***************************************************************************************************
 * GETTERS
 **************************************************************************************************/
stack_size_t stack_allocator_offsets_table_get_size(const stack_table_t inst_id)
{return stack_allocator_table[inst_id].offsets_table_size;}
stack_size_t stack_allocator_offsets_table_get_count(const stack_table_t inst_id)
{return stack_allocator_table[inst_id].offsets_table_count;}

stack_size_t stack_allocator_instance_data_get_size(const stack_table_t inst_id)
{return stack_allocator_table[inst_id].data.size;}
stack_size_t stack_allocator_instance_data_get_used_size(const stack_table_t inst_id)
{return stack_allocator_table[inst_id].data.used_size;}



/***************************************************************************************************
 * TEST-DEBUG
 **************************************************************************************************/
struct stack_allocator_instance*
debug_get_stack_allocator_table()
{return stack_allocator_table;}

struct stack_allocator_instance*
debug_get_stack_allocator_instance(const stack_table_t inst_id)
{return &stack_allocator_table[inst_id];}

void* debug_get_stack_allocator_instance_start_address(const stack_table_t inst_id)
{return stack_allocator_table[inst_id].data.start;}

uint32 debug_get_stack_allocator_init_table_size()
{return STACK_ALLOCATOR_INSTANCE_INITIAL_TABLE_COUNT;}

