/*
 * Created by ckalo on 8/19/26.
 */
#ifndef OPERATING_SYSTEM_CALLS_CPU_H
#define OPERATING_SYSTEM_CALLS_CPU_H

#define CPU_THREAD_0  0
#define CPU_THREAD_1  1
#define CPU_THREAD_2  2
#define CPU_THREAD_3  3
#define CPU_THREAD_4  4
#define CPU_THREAD_5  5
#define CPU_THREAD_6  6
#define CPU_THREAD_7  7
#define CPU_THREAD_8  8
#define CPU_THREAD_9  9
#define CPU_THREAD_10 10
#define CPU_THREAD_11 11
#define CPU_THREAD_12 12
#define CPU_THREAD_13 13
#define CPU_THREAD_14 14
#define CPU_THREAD_15 15

void pin_to_cpu(int cpu);
void set_high_priority(void);

#endif //OPERATING_SYSTEM_CALLS_CPU_H