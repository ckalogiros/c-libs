/*
 * Created by ckalo on 3/14/26.
 *
 * ISSUES:
 * # implement realloc() for arena_allocator_table
 * # implement realloc() for data storage
 */

#include "arena_allocator.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sanitizer/common_interface_defs.h>
#include "logger.h"
#include "mem_man_interface.h"
#include "my_macros.h"
#include "alignment.h"
#include "str_utils.h"
#include "sys_utils.h"



static struct arena_allocator_instance* arena_allocator_table = NULL;
static uint32 instance_count = 0;


void arena_allocator_table_init(void)
{
    const uint32 capacity = ARENA_ALLOCATOR_INSTANCE_INITIAL_TABLE_COUNT;
    const uint32 size = sizeof(struct arena_allocator_instance)*capacity;
    const uint32 alloc_size = ALIGN_SIZE(size, ARENA_ALLOCATOR_MEMORY_ALIGNMENT);
    arena_allocator_table = aligned_alloc(ARENA_ALLOCATOR_MEMORY_ALIGNMENT, alloc_size);
    // memset(mem_pool, 0x0, alloc_size);

    // mem_fprint_append_mem_pool(mem_pool, MEMORY_INTERNAL_POOL_INITIAL_COUNT, size, alloc_size);

#   ifdef ENABLE_SANITIZER
    /* enable santizers to the stderr out */
    __sanitizer_set_report_path("stderr");
#   endif

    print("%s# INITIALIZE arena_allocator_table #%s\n\n", YELLOW, CLEAR);
    print("%s# Allocate arena_allocator_table: %p %s\n", YELLOW, arena_allocator_table, CLEAR);
    print(
        " capacity:        %s%u  %s\n"
        " size bytes:      %s%lu %s\n"
        " aloc size:       %s%lu %s\n"
        " data start:      %s%x%s\n",
        GREEN, capacity,    CLEAR,
        YELLOW, size,       CLEAR,
        YELLOW, alloc_size, CLEAR,
        CYAN, REDUCE_ADDR((char*)arena_allocator_table, F6), CLEAR
    );

}
void arena_allocator_table_realloc(){} /// TODO: implement



/***************************************************************************************************
 * INTERFACE
 **************************************************************************************************/
memid_t arena_allocator_create_new_instance(const pageid_t num_pages)
{
    /* case first time stack allocator instance creation */
    if(arena_allocator_table == NULL)
        arena_allocator_table_init();

    /* create memory for data storage */
    const uint32 data_alignment = ARENA_ALLOCATOR_MEMORY_ALIGNMENT;
    const memid_t memid = memman_allocate_block(num_pages, data_alignment, "arena_allocator: data storage");
    struct memhnd_t hnd1 = memman_page_request(num_pages, memid);
    void* const data_address = memman_get_alloc_mem_by_handle(hnd1);


    /* FOR DEBUG */
    char c[8] = {'A','R','E','N','A', '\0', '\0', '\0'};
    str_conv_int32_to_str(&c[5], memid);
    uint64 s = make_uint64_from_string_max_8chars(c, 8);
    memset_64bit(data_address, s, num_pages * PAGE_SIZE);
    // memset(data_address, 0x0, num_pages * PAGE_SIZE);

    struct arena_allocator_instance inst = {

        .sp_offset = 0, /* points to offsets table [0]*/
        .data = {       /* data storage */
            .start = data_address,
            .mem_id = memid,
            .size = num_pages * PAGE_SIZE,
            .used_size = 0,
        },
    };

    const uint32 idx = instance_count;
    arena_allocator_table[idx] = inst;
    instance_count++;

    return idx;
}

void* arena_allocator_alloc(const memid_t inst_id, const arena_size_t size, const typesize_t type_size)
{
    assert(type_size <= SIZE_I64);    /// limit maximum alignement to 8 bytes
    assert(IS_POWER_OF_2(type_size)); /// alignment value must be power of 2
    assert(arena_allocator_table);
    const uint32 aligned_size = ALIGN_SIZE(size, type_size);
    struct arena_allocator_instance* instance = &arena_allocator_table[inst_id];

    /* get stack pointer */
    const char* p_addr = (char*)instance->data.start + instance->sp_offset;

    /* align: align allocation, and previous allocation's offset */
    const uint32 unaligned = align_pointer_to_type(p_addr, type_size);
    if(unaligned) {
        /* advance stack pointer to aligned offset */
        instance->sp_offset += unaligned;
    }

    void* free_address = (char*) instance->data.start + instance->sp_offset;
    instance->data.used_size += aligned_size;
    /* advance the stack pointer */
    instance->sp_offset += aligned_size;

    char* p = free_address + size;
    // *p = '\0';

    return (void*) free_address;
}

void* arena_allocator_release(const memid_t inst_id)
{
    struct arena_allocator_instance* instance = &arena_allocator_table[inst_id];

    void* free_address = (char*) instance->data.start;

    instance->data.used_size = 0;
    // instance->data.start = instance->data.start;
    instance->sp_offset = 0;
    memset(free_address, 0x0, instance->data.size);

    /* debug-mark released memory with value */
    // memset(free_address, 0xCD, instance->data.size);

    return (void*) free_address;
}

void arena_allocator_destroy(const memid_t inst_id)
{
    memman_deallocate_block(inst_id);
}




/***************************************************************************************************
 * GETTERS
 **************************************************************************************************/
arena_size_t arena_allocator_instance_get_size(const arena_table_t inst_id)
{ return arena_allocator_table[inst_id].data.size; }

arena_size_t arena_allocator_instance_data_get_used_size(const arena_table_t inst_id)
{ return arena_allocator_table[inst_id].data.used_size; }

memid_t arena_allocator_instance_data_get_memid(const arena_table_t inst_id)
{ return arena_allocator_table[inst_id].data.mem_id; }



/***************************************************************************************************
 * TEST-DEBUG
 **************************************************************************************************/
struct arena_allocator_instance*
debug_arena_allocator_instance_get_table()
{return arena_allocator_table;}

struct arena_allocator_instance*
debug_arena_allocator_get_instance(const memid_t inst_id)
{return &arena_allocator_table[inst_id];}

void* debug_arena_allocator_instance_get_data(const memid_t inst_id)
{return arena_allocator_table[inst_id].data.start;}

uint32 debug_arena_allocator_get_table_size()
{return ARENA_ALLOCATOR_INSTANCE_INITIAL_TABLE_COUNT;}

