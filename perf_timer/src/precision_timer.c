//
// Created by ckalo on 12/28/25.
//

#include "precision_timer.h"
#include "buffer.h"
#include "buffer_impl.h"
#include "precision_timer_logger.h"
#include "logger.h"
#include "sys_utils.h"
#include <time.h>

#include "mem_structs.h"
#include "slab_allocator_internals.h"


#define MIN_TIMER_TABLE_CAPACITY 16

#define TIMER_CHECK_TABLE(t_table)\
if(!t_table) {\
    print("%sERROR\nNO timer_table initialization\nForget to call \"timer_init_table()\"??%s", RED, CLEAR);\
    my_exit("timer_create_perf_timer", __FILE__, __LINE__);\
}

#define TIMER_CHECK_TABLE_OVERFLOW_INDEX(index)\
if(index > MIN_TIMER_TABLE_CAPACITY) {\
    print("%sERROR\ntimer_table: INDEX OUT OF BOUNDS\nIncrease \"MIN_TIMER_TABLE_CAPACITY\" MACRO%s", RED, CLEAR);\
    my_exit("timer_create_perf_timer", __FILE__, __LINE__);\
}

static memid_t slab_allocator_instance_id = 0;


void
constructor(struct timer* t)
{
    t->start = 0;
    t->stop = 0;
    t->accum = 0;
    t->delta = 0;
    t->avg = 0;
    t->count = 0;
}

static buffer_t* timer_table = NULL;

void
timer_init_table(void)
{
    if(timer_table) return;

    slab_allocator_init();
    slab_allocator_instance_id = slab_allocator_create_new_instance();
    // allocate memory, construct buffer
    timer_table = buffer_create_gentype(MIN_TIMER_TABLE_CAPACITY, sizeof(struct timer));
}


struct timer*
timer_create_perf_timer(void)
{
    TIMER_CHECK_TABLE(timer_table);

    // todo: we need to insert(), after we have a free index from buffer
    // OR implement insert at next free element in buffer.c
    // const uint16 index = buffer_get_next_free(timer_table);
    const uint16 index = buffer_set_for_push(timer_table);

    TIMER_CHECK_TABLE_OVERFLOW_INDEX(index);


    struct timer* data = (struct timer*) timer_table->data; // cast (for complains)
    struct timer t = {0};
    data[index] = t;
    return &data[index];
}


static uint64
perf_timer_get(void)
{ // https://www.man7.org/linux/man-pages/man3/clock_gettime.3.html

    //  tv_sec: seconds, tv_nsec: nanoseconds
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    const uint64_t nanosec = ts.tv_sec * NANO_SEC + ts.tv_nsec;
    return nanosec;
}


void
perf_timer_start(struct timer* const t)
{
    t->start = perf_timer_get();
}

void
perf_timer_stop(struct timer* const t)
{
    t->stop = perf_timer_get();
    t->delta = t->stop - t->start;
    t->accum += t->delta;
    t->count++;
    t->avg = t->accum / t->count;
}

void
perf_timer_stop_with_count(const uint32 count, struct timer* const t)
{
    t->stop = perf_timer_get();
    t->delta = t->stop - t->start;
    t->accum += t->delta;
    t->count += count;
    t->avg = t->accum / t->count;
}


void
perf_timer_start_accum(struct timer* const t)
{
    t->start = perf_timer_get();
}

void
perf_timer_stop_accum(struct timer* const t)
{
    t->stop = perf_timer_get();
    t->delta = t->stop - t->start;
    t->accum += t->delta;
    t->count++;
}

void
perf_timer_calc_accum(struct timer* const t)
{
    t->avg = t->accum / t->count;
}


void
perf_timer_reset(struct timer* const t)
{
    t->start = 0;
    t->stop = 0;
    t->count = 0;
    t->accum = 0;
    t->delta = 0;
    t->avg = 0;
}

uint32
perf_timer_get_delta(struct timer* const t) { return t->delta; }

uint32
perf_timer_get_avg(struct timer* const t) { return t->avg; }

uint32
perf_timer_get_count(struct timer* const t) { return t->count; }

uint64
perf_timer_get_accum(struct timer* const t) { return t->accum; }

uint32
perf_timer_get_fps(struct timer* const t) { return NANO_SEC / t->avg; }

uint32
perf_timer_calc_fps_from_nano(const uint32 avg) { return NANO_SEC / avg; }


//#include "logger.h"
//void perf_timer_calc_timer_overhead()
//{
//    uint32 repeat = 10000000;
//    perf_timer_start();
//    for (uint32 i = 0; i < repeat; i++)
//    {
//        struct timespec ts;
//        clock_gettime(CLOCK_MONOTONIC, &ts);
//        const uint64_t nanosec = ts.tv_sec * NANO_SEC + ts.tv_nsec;
//    }
//    perf_timer_stop_with_count(repeat);
//    {
//        perf_timer_calc_accum();
//        print("--- Timer\n");
//        const uint32 fps = perf_timer_get_fps();
//        print("fps: %d\n", fps);
//        const uint32 avg = perf_timer_get_avg();
//        print("avg: %d nanosec\n", avg);
//        const uint64 accum = perf_timer_get_accum();
//        print("avg: %ld microsec\n", avg / NANO_TO_MICRO);
//        perf_timer_reset();
//    }
//
//}
