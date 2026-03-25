/*
 * Created by ckalo on 1/8/26.
 */
#ifndef SPARSE_ARR_DESIGNATED_INITIALIZERS_H
#define SPARSE_ARR_DESIGNATED_INITIALIZERS_H

#include "my_types.h"
#include "my_macros.h"

typedef struct {
    sparse_arr_t* buffer;
    uint32 print_num_elems;
} sparse_arr_print_params;

static sparse_arr_print_params
sparse_arr_print_config(sparse_arr_print_params in)
{
    sparse_arr_print_params out = {
        .buffer = NULL,
        .print_num_elems = 0,
    };

    /* override defaults if user provided values */
    if(in.buffer) out.buffer = in.buffer;
    if(in.print_num_elems) out.print_num_elems = in.print_num_elems;

    return out;
}

#endif //SPARSE_ARR_DESIGNATED_INITIALIZERS_H
