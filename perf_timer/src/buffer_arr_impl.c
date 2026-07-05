/*
 * Created by ckalo on 1/28/26.
 */

#include "buffer_arr_impl.h"
#include "buffer_arr.h"


uint32
buffer_arr_push_timer(struct timer t, buffer_t* const buffer)
{
    const uint32 index = buffer_arr_set_for_push(buffer);
    struct timer* data = buffer->data;
    data[buffer->tail] = t;

    return index;
}

void
buffer_arr_insert_timer(struct timer t, buffer_t* const buffer, bufidx_t index)
{
    buffer_arr_set_for_insert(buffer, index);
    struct timer* data = buffer->data;
    data[index] = t;
}
