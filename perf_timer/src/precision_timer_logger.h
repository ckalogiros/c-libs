/*
 * Created by ckalo on 1/28/26.
 */
#ifndef PERF_TIMER_PRECISION_TIMER_LOGGER_H
#define PERF_TIMER_PRECISION_TIMER_LOGGER_H

#include "precision_timer.h"
#include "precision_timer_struct.h"

void
perf_timer_print(const timer_print_params params)
;

// void
// perf_timer_print(struct timer* t, const uint32 repeat, const uint32 buf_capacity);

void
perf_timer_print_simple(struct timer* t, const uint32 repeat, const uint32 buf_capacity);

void
perf_timer_print_simple_with_repeat(struct timer* t, const uint32 repeat, const uint32 buf_capacity);

#endif //PERF_TIMER_PRECISION_TIMER_LOGGER_H
