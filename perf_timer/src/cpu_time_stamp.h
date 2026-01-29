//
// Created by ckalo on 12/28/25.
//

#ifndef PERF_TIMER_CPU_TIME_STAMP_H
#define PERF_TIMER_CPU_TIME_STAMP_H

void cycle_counter_start();
void cycle_counter_stop();
uint32_t cycle_counter_get_avg();
uint32_t cycle_counter_get_count();
uint32_t cycle_counter_get_fps();
uint64_t cycle_counter_get_accum();

#endif //PERF_TIMER_CPU_TIME_STAMP_H