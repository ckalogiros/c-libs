//
// Created by ckalo on 12/29/25.
//

#include "buffer_arr.h"

#include <string.h>

#include "bits.h"
#include "buffer_arr_internals.h"
#include "sys_utils.h"
#include "buffer_arr_log.h"
#include "logger.h"
#include "mem_man_types.h"
#include "slab_allocator_interface.h"

#define BUFFER_ARR_DEBUG_PRINT 1
#undef BUFFER_ARR_DEBUG_PRINT

#define buffer_arr_free_list_insert(value, bit)  (bit_set(value, bit, uint64))
#define buffer_arr_free_list_remove(value, bit)  (bit_clear(value, bit, uint64))
#define buffer_arr_free_list_is_free(value, bit) (bit_is_set(value, bit, uint64))


void
buffer_table_init(const pageid_t num_pages)
{
    buffer_arr_init_table(num_pages);
}

void
buffer_table_destroy(void)
{
    buffer_arr_destroy_table();
}

buffer_t*
buffer_arr_create_uint32(const uint32 capacity)
{
    return buffer_arr_constructor_uint32(capacity);
}

buffer_t*
buffer_arr_create_gentype(const uint32 capacity, const typesize_t typesize)
{
    return buffer_arr_constructor_gentype(capacity, typesize);
}

void
buffer_arr_destroy(buffer_t* buffer)
{ // ISSUES: what if bufer has children buffers in its data?? how should children buffers handled??
    buffer_arr_destructor(buffer);
}

int32
buffer_arr_push_uint32(const uint32 val, buffer_t* const buffer)
{
    // todo: implement realloc
    if(buffer->tail + 1 >= buffer->capacity) {
        buffer_arr_reallocate(buffer);
    } else {
        buffer->tail++;
        uint32* data = buffer->data;
        data[buffer->tail] = val;
        buffer->count++;

        const uint32 offset = buffer->tail / BITS64;
        uint64* free_list = buffer->free_list + offset;
        buffer_arr_free_list_remove(*free_list, buffer->tail);

        return buffer->tail;
    }

    return INT_NULL;
}

void
buffer_arr_insert_uint32(const uint32 val, buffer_t* const buffer, const bufidx_t index)
{
    // todo: implement realloc
    if(buffer->tail + 1 > buffer->capacity) { my_exit("buffer-implement realloc", __FILE__, __LINE__); } else {

        uint32* data = buffer->data;
        data[index] = val;
        buffer->count++; // todo: we assume that we insert in a none occupied element

        /* increment tail only if index is greater */
        if(index > buffer->tail) buffer->tail = index;

        const uint32 offset = index / BITS64;
        uint64* free_list = buffer->free_list + offset;
        buffer_arr_free_list_remove(*free_list, index);
    }
}

void
buffer_arr_free_elem(buffer_t* const buffer, const bufidx_t index)
{
    if(index >= buffer->capacity)
        buffer_arr_print_error(BUFFER_ARR_INDEX_GREATER_THAN_CAPACITY);

    // cannot be that a stored elem has greter index than the tail
    if(index > buffer->tail)
        buffer_arr_print_error(BUFFER_ARR_INDEX_GREATER_THAN_TAIL);


    const uint32 offset = index / BITS64; // calc which uint64
    uint32 sindex = index - (offset * BITS64); // find index in that uint64
    uint64* free_list = (buffer->free_list + offset);

    // print("index:%u  sindex:%u \n", index, sindex);

    const uint8 is_free = buffer_arr_free_list_is_free(*free_list, sindex);
    if(!is_free) { // if element is not free, free it
        buffer_arr_free_list_insert(*free_list, sindex);
        buffer->count--;
        // do not decrement tail if tail < index
        if(index == buffer->tail) buffer->tail--;
    }
    /// reset memory
    memset(&buffer->data[index*buffer->typesize], 0xAB, buffer->typesize);
}

void // sets all elems as free. buffer data remain untouched
buffer_arr_empty(buffer_t* const buffer, const bufidx_t index)
{
    for(uint32 i = 0; i < buffer->free_list_capacity; i++)
        buffer->free_list[i] = 0x0;

    buffer->count = 0;
    buffer->tail  = -1;
}

int16
buffer_arr_get_next_free(buffer_t* const buffer)
{
    const uint16 first_free_index = buffer_arr_find_next_free_elem(buffer);
    return first_free_index;
}

/// return: elements index
int16 // sets up buffer's fields, but the data transform happens at caller site
buffer_arr_set_for_push(buffer_t* const buffer)
{
    // todo: implement realloc
    if(buffer->tail + 1 >= buffer->capacity) {
        buffer_arr_reallocate(buffer);
        print("%s#### SPARSE_ARR REALLOC\n new capacity:%u%s\n", YELLOW, buffer->capacity, CLEAR);
    }

    buffer->tail++;
    buffer->count++;
    const bufidx_t offset = buffer->tail / BITS64;
    uint64* free_list = &buffer->free_list[offset];
    buffer_arr_free_list_remove(*free_list, buffer->tail);

    return buffer->tail;
}

void // sets up buffer's fields, but the data transform happens at caller site
buffer_arr_set_for_insert(buffer_t* const buffer, const bufidx_t index)
{
    if(index >= buffer->capacity)
        buffer_arr_print_error(BUFFER_ARR_INDEX_GREATER_THAN_CAPACITY);

    buffer->count++; // todo: we assume that we insert in a none occupied element
    // increment tail only if index is greater
    if(index > buffer->tail) buffer->tail = index;

    const uint32 offset = index / BITS64;
    buffer_arr_free_list_remove(buffer->free_list[offset], index);
}



/*
 * getters
 */

memid_t
buffer_arr_get_memid(buffer_t* const buffer)
{
    assert(buffer->capacity > 0);
    assert(buffer->typesize > 0);
    const uint16 inst_id = buffer_arr_get_allocator_instance_id();
    memid_t memid = slab_allocator_get_memid(inst_id, buffer->capacity, buffer->typesize);
    return memid;
}



/* DEBUG-FPRINT */
void buffer_arr_fprint(buffer_t* buffer)
{
    buffer_arr_print_to_file(buffer);
}