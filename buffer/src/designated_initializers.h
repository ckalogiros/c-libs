/*
 * Created by ckalo on 1/8/26.
 */
#ifndef BUFFER_DESIGNATED_INITIALIZERS_H
#define BUFFER_DESIGNATED_INITIALIZERS_H

#include "my_types.h"
#include "my_macros.h"

typedef struct {
    buffer_t* buffer;
    uint32 print_num_elems;
} buffer_print_p;

static inline buffer_print_p buffer_print_config(buffer_print_p in)
{
    buffer_print_p out = {
        .buffer = NULL,
        .print_num_elems = 0,
    };

    /* override defaults if user provided values */
    if (in.buffer)      out.buffer = in.buffer;
    if (in.print_num_elems) out.print_num_elems = in.print_num_elems;

    return out;
}

#endif //BUFFER_DESIGNATED_INITIALIZERS_H