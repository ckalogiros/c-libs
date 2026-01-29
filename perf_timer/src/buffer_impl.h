/*
 * Created by ckalo on 1/28/26.
 */
#ifndef PERF_TIMER_BUFFER_IMPL_H
#define PERF_TIMER_BUFFER_IMPL_H

#include "precision_timer_struct.h"
#include "buffer_struct.h"

uint32 buffer_push_timer(struct timer t, buffer_t *const buffer);
void buffer_insert_timer(struct timer t, buffer_t* const buffer, bufidx_t index);
// void buffer_remove_vec4f(buffer_t *const buffer, const bufidx_t index);

#endif //PERF_TIMER_BUFFER_IMPL_H
