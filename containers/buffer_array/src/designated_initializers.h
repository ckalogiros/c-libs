/*
 * Created by ckalo on 1/8/26.
 */
#ifndef BUFFER_ARR_DESIGNATED_INITIALIZERS_H
#define BUFFER_ARR_DESIGNATED_INITIALIZERS_H

#include "my_types.h"
#include "my_macros.h"

typedef struct {
    buffer_t* buffer;
    uint32 print_num_elems;
} buffer_arr_print_params;

static buffer_arr_print_params
buffer_arr_print_config(buffer_arr_print_params in)
{
    buffer_arr_print_params out = {
        .buffer = NULL,
        .print_num_elems = 0,
    };

    /* override defaults if user provided values */
    if(in.buffer) out.buffer = in.buffer;
    if(in.print_num_elems) out.print_num_elems = in.print_num_elems;

    return out;
}

#endif //BUFFER_ARR_DESIGNATED_INITIALIZERS_H
