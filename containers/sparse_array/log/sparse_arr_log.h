/*
 * Created by ckalo on 1/6/26.
 */
#ifndef SPARSE_ARR_SPARSE_ARR_LOG_H
#define SPARSE_ARR_SPARSE_ARR_LOG_H

#include "sparse_arr.h"
#include "designated_initializers.h"


void
sparse_arr_print_uint32(sparse_arr_print_params);

void
sparse_arr_print_free_list(const sparse_arr_t* const buffer);

void
sparse_arr_print_char(const sparse_arr_t* const buffer);


// ERROR handling
void
sparse_arr_print_error(const enum SPARSE_ARR_ERROR_CODE code);

#endif //SPARSE_ARR_SPARSE_ARR_LOG_H
