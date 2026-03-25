/*
 * Created by ckalo on 1/28/26.
 */

#include "sparse_arr_impl.h"
#include "sparse_arr.h"


uint32
sparse_arr_push_timer(struct timer t, sparse_arr_t* const buffer)
{
    const uint32 index = sparse_arr_set_for_push(buffer);
    struct timer* data = buffer->data;
    data[buffer->tail] = t;

    return index;
}

void
sparse_arr_insert_timer(struct timer t, sparse_arr_t* const buffer, bufidx_t index)
{
    sparse_arr_set_for_insert(buffer, index);
    struct timer* data = buffer->data;
    data[index] = t;
}
