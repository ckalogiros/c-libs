//
// Created by ckalo on 12/28/25.
//

#include <stdint.h>
#include <x86intrin.h>  // For __rdtsc()
#include <unistd.h>

#include "cpu_time_stamp.h"


static uint64_t
rdtsc(void)
{
    unsigned int lo, hi;
    __asm__ volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t) hi << 32) | lo;
}

// For CPUs with constant/invariant TSC (most modern Intel/AMD)
uint64_t
get_tsc_frequency(void)
{
    const uint64_t freq = sysconf(_SC_CLK_TCK); // Not exact TSC frequency
    // Better: read from CPU model-specific registers or /proc/cpuinfo
    return freq * 1000000; // Approximate
}

typedef struct {
    uint64 start;
    uint64 stop;
    uint64 accum;
    uint32 delta;
    uint32 avg;
    uint32 count;
} cycles;

static cycles t = {0, 0, 0};

void
cycle_counter_start(void)
{
    // t.start = rdtsc();
    t.start = get_tsc_frequency();
}

void
cycle_counter_stop(void)
{
    // t.stop = rdtsc();
    t.stop = get_tsc_frequency();
    t.delta = t.stop - t.start;

    t.count++;
    t.accum += t.delta;

    t.avg = t.accum / t.count;
}

uint32
cycle_counter_get_delta(void) { return t.delta; }

uint32
cycle_counter_get_avg(void) { return t.avg; }

uint32
cycle_counter_get_count(void) { return t.count; }

uint64
cycle_counter_get_accum(void) { return t.accum; }

// uint32_t cycle_counter_get_fps()   { return NANO_SEC / t.avg; }
