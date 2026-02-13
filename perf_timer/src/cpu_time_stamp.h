//
// Created by ckalo on 12/28/25.
//

#ifndef PERF_TIMER_CPU_TIME_STAMP_H
#define PERF_TIMER_CPU_TIME_STAMP_H

#include "my_types.h"

void cycle_counter_start();
void cycle_counter_stop();
uint32 cycle_counter_get_avg();
uint32 cycle_counter_get_count();
uint32 cycle_counter_get_fps();
uint64 cycle_counter_get_accum();

#endif //PERF_TIMER_CPU_TIME_STAMP_H