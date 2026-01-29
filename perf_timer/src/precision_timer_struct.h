/*
 * Created by ckalo on 1/28/26.
 */
#ifndef PERF_TIMER_PRECISION_TIMER_STRUCT_H
#define PERF_TIMER_PRECISION_TIMER_STRUCT_H

#include "my_types.h"

struct timer{
  uint64 start;
  uint64 stop;
  uint64 accum;
  uint32 delta;
  uint32 avg;
  uint32 count;
};


#endif //PERF_TIMER_PRECISION_TIMER_STRUCT_H
