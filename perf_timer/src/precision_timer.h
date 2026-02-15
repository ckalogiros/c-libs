//
// Created by ckalo on 12/27/25.
//

#ifndef PERF_TIMER_PERF_TIMER_H
#define PERF_TIMER_PERF_TIMER_H

#include "my_types.h"

#define NANO_SEC 1000000000LU
#define MICRO_SEC 1000000LU
#define MILI_SEC 1000LU
#define NANO_TO_MICRO (NANO_SEC/MICRO_SEC)
#define NANO_TO_MILI (NANO_SEC/MILI_SEC)


void timer_init_table(void);

struct timer* timer_create_perf_timer(void);

void perf_timer_start(struct timer*);

void perf_timer_stop(struct timer*);

void perf_timer_stop_with_count(const uint32 count, struct timer*);

void perf_timer_reset(struct timer*);

void perf_timer_start_accum(struct timer*);

void perf_timer_stop_accum(struct timer*);

void perf_timer_calc_accum(struct timer*);

uint32 perf_timer_get_delta(struct timer*);

uint64 perf_timer_get_accum(struct timer*);

uint32 perf_timer_get_avg(struct timer*);

uint32 perf_timer_get_count(struct timer*);

uint32 perf_timer_get_fps(struct timer*);

uint32 perf_timer_calc_fps_from_nano(const uint32 avg);


#endif //PERF_TIMER_PERF_TIMER_H
