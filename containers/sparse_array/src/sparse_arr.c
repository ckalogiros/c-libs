//
// Created by ckalo on 12/29/25.
//

#include "sparse_arr.h"

#include "bits.h"
#include "sparse_arr_internals.h"
#include "sys_utils.h"
#include "sparse_arr_log.h"
#include "logger.h"
#include "mem_man_types.h"
#include "slab_allocator.h"

#define SPARSE_ARR_DEBUG_PRINT 1
#undef SPARSE_ARR_DEBUG_PRINT

#define sparse_arr_free_list_insert(value, bit) (bit_set(value, bit, uint64))
#define sparse_arr_free_list_remove(value, bit) (bit_clear(value, bit, uint64))
#define sparse_arr_free_list_is_free(value, bit) (bit_is_set(value, bit, uint64))


void
sparse_arr_table_init(const pageid_t num_pages)
{
    sparse_arr_init_table(num_pages);
}

void
sparse_arr_table_destroy(void)
{
    sparse_arr_destroy_table();
}

sparse_arr_t*
sparse_arr_create_uint32(const uint32 capacity)
{
    return sparse_arr_constructor_uint32(capacity);
}

sparse_arr_t*
sparse_arr_create_gentype(const uint32 capacity, const typesize_t typesize)
{
    return sparse_arr_constructor_gentype(capacity, typesize);
}

void
sparse_arr_destroy(sparse_arr_t* buffer)
{ // TODO: what if bufer has children buffers in its data?? how should children buffers handled??
    sparse_arr_destructor(buffer);
}

int32
sparse_arr_push_uint32(const uint32 val, sparse_arr_t* const buffer)
{
    // todo: implement realloc
    if(buffer->tail + 1 >= buffer->capacity) {
        sparse_arr_reallocate(buffer);
    } else {
        buffer->tail++;
        uint32* data = buffer->data;
        data[buffer->tail] = val;
        buffer->count++;

        const uint32 offset = buffer->tail / BITS64;
        uint64* free_list = buffer->free_list + offset;
        sparse_arr_free_list_remove(*free_list, buffer->tail);

        return buffer->tail;
    }

    return INT_NULL;
}

void
sparse_arr_insert_uint32(const uint32 val, sparse_arr_t* const buffer, const bufidx_t index)
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
        sparse_arr_free_list_remove(*free_list, index);
    }
}

void
sparse_arr_free_elem(sparse_arr_t* const buffer, const bufidx_t index)
{
    if(index >= buffer->capacity)
        sparse_arr_print_error(SPARSE_ARR_INDEX_GREATER_THAN_CAPACITY);

    // cannot be that a stored elem has greter index than the tail
    if(index > buffer->tail)
        sparse_arr_print_error(SPARSE_ARR_INDEX_GREATER_THAN_TAIL);


    const uint32 offset = index / BITS64; // calc which uint64
    uint32 sindex = index - (offset * BITS64); // find index in that uint64
    uint64* free_list = (buffer->free_list + offset);

    // print("index:%u  sindex:%u \n", index, sindex);

    const uint8 is_free = sparse_arr_free_list_is_free(*free_list, sindex);
    if(!is_free) { // if element is not free, free it
        sparse_arr_free_list_insert(*free_list, sindex);
        buffer->count--;
        // do not decrement tail if tail < index
        if(index == buffer->tail) buffer->tail--;
    }
}

void // sets all elems as free. buffer data remain untouched
sparse_arr_empty(sparse_arr_t* const buffer, const bufidx_t index)
{
    for(uint32 i = 0; i < buffer->free_list_capacity; i++)
        buffer->free_list[i] = 0x0;

    buffer->count = 0;
    buffer->tail = -1;
}

uint16
sparse_arr_get_next_free(sparse_arr_t* const buffer)
{
    const uint16 first_free_index = sparse_arr_find_next_free_elem(buffer);
    return first_free_index;
}

/// return: elements index
uint16 // sets up buffer's fields, but the data transform happens at caller site
sparse_arr_set_for_push(sparse_arr_t* const buffer)
{
    // todo: implement realloc
    if(buffer->tail + 1 >= buffer->capacity) {
        sparse_arr_reallocate(buffer);
        print("%s#### SPARSE_ARR REALLOC\n new capacity:%u%s\n", YELLOW, buffer->capacity, CLEAR);
    }

    buffer->tail++;
    buffer->count++;
    const bufidx_t offset = buffer->tail / BITS64;
    uint64* free_list = &buffer->free_list[offset];
    sparse_arr_free_list_remove(*free_list, buffer->tail);

    return buffer->tail;

}

void // sets up buffer's fields, but the data transform happens at caller site
sparse_arr_set_for_insert(sparse_arr_t* const buffer, const bufidx_t index)
{
    if(index >= buffer->capacity)
        sparse_arr_print_error(SPARSE_ARR_INDEX_GREATER_THAN_CAPACITY);


    buffer->count++; // todo: we assume that we insert in a none occupied element
    // increment tail only if index is greater
    if(index > buffer->tail) buffer->tail = index;

    const uint32 offset = index / BITS64;
    sparse_arr_free_list_remove(buffer->free_list[offset], index);
    // uint64* free_list = buffer->free_list + offset;
    // sparse_arr_free_list_remove(*free_list, index);
}



/*
 * getters
 */

memid_t
sparse_arr_get_memid(sparse_arr_t* const buffer)
{
    assert(buffer->capacity > 0);
    assert(buffer->typesize > 0);
    const uint16 inst_id = sparse_arr_get_allocator_instance_id();
    memid_t memid = slab_allocator_get_memid(inst_id, buffer->capacity, buffer->typesize);
    return memid;
}



/* DEBUG-FPRINT */
void sparse_arr_fprint(sparse_arr_t* buffer)
{
    sparse_arr_print_to_file(buffer);
}