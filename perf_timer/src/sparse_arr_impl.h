/*
 * Created by ckalo on 1/28/26.
 */
#ifndef PERF_TIMER_BUFFER_IMPL_H
#define PERF_TIMER_BUFFER_IMPL_H

#include "precision_timer_struct.h"
#include "sparse_arr_struct.h"

uint32 sparse_arr_push_timer(struct timer t, sparse_arr_t* const buffer);

void sparse_arr_insert_timer(struct timer t, sparse_arr_t* const buffer, bufidx_t index);

#endif //PERF_TIMER_BUFFER_IMPL_H
