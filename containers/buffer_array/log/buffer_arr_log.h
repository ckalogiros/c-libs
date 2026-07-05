/*
 * Created by ckalo on 1/6/26.
 */
#ifndef BUFFER_ARR_BUFFER_ARR_LOG_H
#define BUFFER_ARR_BUFFER_ARR_LOG_H

#include "buffer_arr.h"
#include "designated_initializers.h"


void
buffer_arr_print_uint32(buffer_arr_print_params);

void
buffer_arr_print_free_list(const buffer_t* const buffer);

void
buffer_arr_print_char(const buffer_t* const buffer);


// ERROR handling
void
buffer_arr_print_error(const enum BUFFER_ARR_ERROR_CODE code);

#endif //BUFFER_ARR_BUFFER_ARR_LOG_H
