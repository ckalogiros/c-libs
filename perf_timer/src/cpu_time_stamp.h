//
// Created by ckalo on 12/28/25.
//

#ifndef PERF_TIMER_CPU_TIME_STAMP_H
#define PERF_TIMER_CPU_TIME_STAMP_H

#include "my_types.h"

void cycle_counter_start(void);
void cycle_counter_stop(void);
uint32 cycle_counter_get_avg(void);
uint32 cycle_counter_get_count(void);
uint32 cycle_counter_get_fps(void);
uint64 cycle_counter_get_accum(void);

#endif //PERF_TIMER_CPU_TIME_STAMP_H