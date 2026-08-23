/*
 * Created by ckalo on 8/19/26.
 */

#define _GNU_SOURCE  // Must be BEFORE any includes!
#include <sched.h>
#include <sys/resource.h>
// #define __USE_GNU
// #include <sched.h>

#include "cpu.h"

#include <stdio.h>


void pin_to_cpu(int cpu) {
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);
}

void set_high_priority(void)
{
    if (setpriority(PRIO_PROCESS, 0, -20) != 0) {
        perror("setpriority failed");
    }
}

