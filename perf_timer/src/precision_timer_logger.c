/*
 * Created by ckalo on 1/28/26.
 */

#include "precision_timer_logger.h"
#include "logger.h"

void temp_debug_print_timer(struct timer* t) {

  print(
    "  accum:%llu"
    "  count:%u"
    "  avg:%u"
    "  delta:%u"
    "  start:%llu"
    "  stop:%llu",
    t->accum,
    t->count,
    t->avg,
    t->delta,
    t->start,
    t->stop
    );
  print("\n");
}

