//
// Created by ckalo on 12/29/25.
//

#include "buffer.h"
#include "buffer_internals.h"
#include "sys_utils.h"
#include "bits.h"

#include <string.h>

#include "buffer_log.h"
#include "logger.h"

#define BUFFER_DEBUG_PRINT 1
#undef BUFFER_DEBUG_PRINT

#define buffer_free_list_insert(value, bit) (bit_set(value, bit, uint64))
#define buffer_free_list_remove(value, bit) (bit_clear(value, bit, uint64))
#define buffer_free_list_is_free(value, bit) (bit_is_set(value, bit, uint64))


void
buffer_init()
{
    buffer_init_table();
}

buffer_t*
buffer_create_uint32(const uint32 capacity)
{
    return buffer_constructor_uint32(capacity);
}

buffer_t*
buffer_create_gentype(const uint32 capacity, const uint32 typesize)
{
    return buffer_constructor_gentype(capacity, typesize);
}

void
buffer_destroy(buffer_t* buffer)
{
    buffer_destructor(buffer);
}

int32
buffer_push_uint32(const uint32 val, buffer_t* const buffer)
{
    // todo: implement realloc
    if(buffer->tail + 1 >= buffer->capacity) {
        buffer_reallocate(buffer);
    } else {
        buffer->tail++;
        uint32* data = buffer->data;
        data[buffer->tail] = val;
        buffer->count++;

        const uint32 offset = buffer->tail / BITS64;
        uint64* free_list = buffer->free_list + offset;
        buffer_free_list_remove(*free_list, buffer->tail);

        return buffer->tail;
    }

    return INT_NULL;
}

void
buffer_insert_uint32(const uint32 val, buffer_t* const buffer, const bufidx_t index)
{
    // todo: implement realloc
    if(buffer->tail + 1 > buffer->capacity) { my_exit("buffer-implement realloc", __FILE__, __LINE__); } else {
        uint32* data = buffer->data;
        data[index] = val;
        buffer->count++; // todo: we assume that we insert in a none occupied element
        // increment tail only if index is greater
        if(index > buffer->tail) buffer->tail = index;

        const uint32 offset = index / BITS64;
        uint64* free_list = buffer->free_list + offset;
        buffer_free_list_remove(*free_list, index);
    }
}

void
buffer_free_elem(buffer_t* const buffer, const bufidx_t index)
{
    if(index >= buffer->capacity)
        buffer_print_error(BUFFER_INDEX_GREATER_THAN_CAPACITY);

    // cannot be that a stored elem has greter index than the tail
    if(index > buffer->tail)
        buffer_print_error(BUFFER_INDEX_GREATER_THAN_TAIL);


    const uint32 offset = index / BITS64; // calc which uint64
    uint32 sindex = index - (offset*BITS64); // find index in that uint64
    uint64* free_list = (buffer->free_list + offset);

    // print("index:%u  sindex:%u \n", index, sindex);

    const uint8 is_free = buffer_free_list_is_free(*free_list, sindex);
    if(!is_free) { // if element is not free, free it
        buffer_free_list_insert(*free_list, sindex);
        buffer->count--;
        // do not decrement tail if tail < index
        if(index == buffer->tail) buffer->tail--;
    }

}

void // sets all elems as free. buffer data remain untouched
buffer_empty(buffer_t* const buffer, const bufidx_t index)
{
    for(uint32 i = 0; i < buffer->free_list_capacity; i++)
        buffer->free_list[i] = 0x0;

    buffer->count = 0;
    buffer->tail = -1;
}

/*
 * getters
 */
uint16
buffer_get_next_free(buffer_t* const buffer)
{
    const uint16 first_free_index = buffer_find_next_free_elem(buffer);
    return first_free_index;
}


uint16 // sets up buffer's fields, but the data transform happens at caller site
buffer_set_for_push(buffer_t* const buffer)
{
    // todo: implement realloc
    if(buffer->tail + 1 >= buffer->capacity) {
        buffer_reallocate(buffer);
        my_exit("ERROR in buffer_set_for_push().\n"
                "buffer_reallocate deals with uint32 buffer type\n"
                "but buffer_set_for_push is a generic type function\n"
                "SO REALLOC DOES  NOT WORK!!!",
                __FILE__, __LINE__);
    } else {
        buffer->tail++;
        buffer->count++;
        const bufidx_t offset = buffer->tail / BITS64;
        uint64* free_list = &buffer->free_list[offset];
        buffer_free_list_remove(*free_list, buffer->tail);

        return buffer->tail;
    }

    return INT_NULL;
}

void // sets up buffer's fields, but the data transform happens at caller site
buffer_set_for_insert(buffer_t* const buffer, const bufidx_t index)
{
    if(index >= buffer->capacity)
        buffer_print_error(BUFFER_INDEX_GREATER_THAN_CAPACITY);


    buffer->count++; // todo: we assume that we insert in a none occupied element
    // increment tail only if index is greater
    if(index > buffer->tail) buffer->tail = index;

    const uint32 offset = index / BITS64;
    buffer_free_list_remove(buffer->free_list[offset], index);
    // uint64* free_list = buffer->free_list + offset;
    // buffer_free_list_remove(*free_list, index);
}
