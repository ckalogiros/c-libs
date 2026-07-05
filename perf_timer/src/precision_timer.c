/*
 * Created by ckalo on 12/28/25.
 */

#include "precision_timer.h"

#include <assert.h>
#include <stdlib.h>
#include "buffer_arr_impl.h"
#include "precision_timer_logger.h"
#include "logger.h"
#include "sys_utils.h"
#include <time.h>
#include "my_macros.h"
// #include "slab_allocator_internals.h"
#include "slab_allocator_struct.h"


#define TIMER_TABLE_MIN_CAPACITY 16
#define TIMER_ALIGNMENT 64


#define TIMER_CHECK_TABLE(t_table)\
if(!t_table) {\
    print("%sERROR\nNO timer_table initialization\nForget to call \"timer_init_table()\"??%s", RED, CLEAR);\
    my_exit("timer_create_perf_timer", __FILE__, __LINE__);\
}

#define TIMER_CHECK_TABLE_OVERFLOW_INDEX(index)\
if(index > TIMER_TABLE_MIN_CAPACITY) {\
    print("%sERROR\ntimer_table: INDEX OUT OF BOUNDS\nIncrease \"TIMER_TABLE_MIN_CAPACITY\" MACRO%s", RED, CLEAR);\
    my_exit("timer_create_perf_timer", __FILE__, __LINE__);\
}


/*******************************************************************************
 * timer_table: a buffer to store all applications timers
 ******************************************************************************/
struct timer* timer_table = NULL;
struct flist* timer_table_free_list_head = NULL;
static uint32 timer_table_count = 0;
static uint32 timer_table_allocations_count = 0;


/*******************************************************************************
 * timer objects: a free list is created inside the timer_table
 * timer_table_flist_get_free():
 *     gives back a free timer from table
 * timer_table_flist_add():
 *     frees a timer and adds it to the free list
 ******************************************************************************/
void
timer_table_flist_add(void* ptr)
{
    struct flist* list = ptr;
    list->next = timer_table_free_list_head;
    timer_table_free_list_head = list;
    timer_table_allocations_count++;
}

buffer_t*
timer_table_flist_get_free(void)
{
    if(!timer_table) {
        print("%sERROR: timer_table already initialized\n%s", RED, CLEAR);
        my_exit("ERROR: timer_table not initialized\nforget to call timer_init_table()?", __FILE__, __LINE__);
    }

    /* ERROR: if there are no free timer elements */
    assert(timer_table_free_list_head);

    struct flist* list = timer_table_free_list_head;
    timer_table_free_list_head = timer_table_free_list_head->next;
    timer_table_count++;
    timer_table_allocations_count--;
    return (buffer_t*) list;
}

void
timer_init_table(void)
{
    if(timer_table) {
        print("%sERROR: timer_table already initialized\n%s", RED, CLEAR);
        return;
    }

    const uint32 minsize = ALIGN_SIZE(sizeof(struct timer) * TIMER_TABLE_MIN_CAPACITY, TIMER_ALIGNMENT);
    timer_table = aligned_alloc(TIMER_ALIGNMENT, minsize);

    /* initialize table, with empty timer objects */
    struct timer* ptr = timer_table;
    while((uint64) ptr < (uint64) (timer_table + TIMER_TABLE_MIN_CAPACITY)) {
        timer_table_flist_add(ptr);
        ptr++;
    }

}


struct timer*
timer_create_perf_timer(void)
{
    if(!timer_table) timer_init_table();

    struct timer* timer = (struct timer*)timer_table_flist_get_free();

    struct timer t = {0};
    *timer = t; /* copy new timer to table */
    return timer;
}


static inline uint64
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
perf_timer_calc_avg(struct timer* const t) { return t->accum / t->count; }

uint32
perf_timer_calc_avg_from_count(struct timer* const t, const uint32 count) { return t->accum / count; }

uint32
perf_timer_get_count(struct timer* const t) { return t->count; }

uint64
perf_timer_get_accum(struct timer* const t) { return t->accum; }

uint32
perf_timer_get_fps(struct timer* const t) { return NANO_SEC / t->avg; }

uint32
perf_timer_calc_fps_from_nano(const uint32 avg) { return NANO_SEC / avg; }