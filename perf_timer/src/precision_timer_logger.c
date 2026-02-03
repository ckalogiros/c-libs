/*
 * Created by ckalo on 1/28/26.
 */

#include "precision_timer_logger.h"
#include "logger.h"
#include "precision_timer.h"


void
perf_timer_print(const timer_print_params params)
{
    /*
    PERF_T_FPS = BIT_MASK_FROM_UINT(0),
    PERF_T_AVG_NANO = BIT_MASK_FROM_UINT(1),
    PERF_T_AVG_MICRO = BIT_MASK_FROM_UINT(2),
    PERF_T_AVG_MILI = BIT_MASK_FROM_UINT(3),
    PERF_T_COUNT = BIT_MASK_FROM_UINT(4),
    PERF_T_ACCUM = BIT_MASK_FROM_UINT(5),
    PERF_T_DELTA = BIT_MASK_FROM_UINT(6),
    PERF_T_REPEAT = BIT_MASK_FROM_UINT(7),
    PERF_T_BUF_CAPACITY = BIT_MASK_FROM_UINT(8),
    PERF_T_USE_REPEAT = BIT_MASK_FROM_UINT(9),
    PERF_T_USE_BUF_CAPACITY = BIT_MASK_FROM_UINT(10),*/

    uint64 accum = params.t->accum;
    uint32 count = params.t->count;

    if(params.flags & PERF_T_USE_REPEAT) count *= params.repeat;
    if(params.flags & PERF_T_USE_BUF_CAPACITY) count *= params.buf_capacity;

    uint32 avg = accum / count;

    // avg /= count;

    // HEADER
    if(!(params.flags & PERF_T_IN_CONTINUATION))
        print("\n%s-- Perf Timer --%s\n",YELLOW, CLEAR);

    if(params.flags & (PERF_T_COUNT|PERF_T_ALL|PERF_T_COMMON))
        print("  %scount%s: %s%u%s", ry, CLEAR, GREEN, params.t->count, CLEAR);
    if(params.flags & (PERF_T_ACCUM|PERF_T_ALL))
        print("  %saccum%s: %s%llu%s", ry, CLEAR, GREEN, params.t->accum, CLEAR);
    if(params.flags & (PERF_T_DELTA|PERF_T_ALL))
        print("  %sdelta%s: %s%u%s", ry, CLEAR, GREEN, params.t->delta, CLEAR);
    if(!(params.flags & PERF_T_INLINE) &&
        params.flags & (PERF_T_COUNT|PERF_T_ACCUM|PERF_T_DELTA|(PERF_T_ALL|PERF_T_COMMON)))
        print("\n");


    if(params.flags & (PERF_T_REPEAT|PERF_T_ALL))
        print("  %srepeat%s: %s%u%s", br2, CLEAR, GREEN, params.repeat, CLEAR);
    if(params.flags & (PERF_T_BUF_CAPACITY|PERF_T_ALL))
        print("  %sbuf_capacity%s: %s%u%s\n", br2, CLEAR, GREEN, params.buf_capacity, CLEAR);

    if(params.flags & (PERF_T_FPS|PERF_T_ALL|PERF_T_COMMON)) {
        uint32_t fps = 0;
        if(avg!=0)
            fps = perf_timer_calc_fps_from_nano(avg);
        print("  %sfps%s: %s%d%s\n", ry, CLEAR, YELLOW, fps, CLEAR);
    }


    if(params.flags & (PERF_T_AVG_NANO|PERF_T_ALL|PERF_T_COMMON))
        print("  %savg%s: %s%d%s nano", ry, CLEAR, YELLOW, avg, CLEAR);
    if(params.flags & (PERF_T_AVG_MICRO|PERF_T_ALL|PERF_T_COMMON))
        print("  %savg%s: %s%ld%s micro", ry, CLEAR, YELLOW, avg / NANO_TO_MICRO, CLEAR);
    if(params.flags & (PERF_T_AVG_MILI|PERF_T_ALL))
        print("  %savg%s: %s%ld%s micro", ry, CLEAR, YELLOW, avg / NANO_TO_MILI, CLEAR);
    if(params.flags & (PERF_T_AVG_NANO|PERF_T_AVG_MICRO|PERF_T_AVG_MILI | (PERF_T_ALL|PERF_T_COMMON)))
        print("\n");


    if(!(params.flags & PERF_T_IN_CONTINUATION)) print("\n");
}

void
perf_timer_print_simple(struct timer* t, const uint32 repeat, const uint32 buf_capacity)
{
    const uint32_t avg = perf_timer_get_avg(t);
    print("\n%s-PerfTimer%s: %savg%s: %s%d%s nano", YELLOW, CLEAR, ry, CLEAR, YELLOW, avg, CLEAR);
    // print("  %savg%s: %s%ld%s micro", ry, CLEAR, YELLOW, avg / NANO_TO_MICRO, CLEAR);
    const uint32_t fps = perf_timer_get_fps(t);
    print(" %sfps%s: %s%d%s\n", ry, CLEAR, YELLOW, fps, CLEAR);

    print(
        "  %srepeat%s: %s%u%s"
        "  %sbuf_capacity%s: %s%u%s\n",
        br2, CLEAR, GREEN, repeat, CLEAR,
        br2, CLEAR, GREEN, buf_capacity, CLEAR
    );

    print("\n");
}

void
perf_timer_print_simple_with_repeat(struct timer* t, const uint32 repeat, const uint32 buf_capacity)
{

    print(
        "\n%s-PerfTimer%s: %srepeat%s: %s%u%s"
        "  %sbuf_capacity%s: %s%u%s\n",
        YELLOW, CLEAR,
        br2, CLEAR, GREEN, repeat, CLEAR,
        br2, CLEAR, GREEN, buf_capacity, CLEAR
    );



    const uint32_t avg = perf_timer_get_avg(t);
    print("%savg%s: %s%d%s nano", ry, CLEAR, YELLOW, avg/repeat, CLEAR);
    // print("  %savg%s: %s%ld%s micro", ry, CLEAR, YELLOW, avg / NANO_TO_MICRO, CLEAR);
    const uint32_t fps = perf_timer_calc_fps_from_nano(avg/repeat);
    print(" %sfps%s: %s%d%s\n", ry, CLEAR, YELLOW, fps, CLEAR);


    print("\n");
}

// void
// perf_timer_print(struct timer* t, const uint32 repeat, const uint32 buf_capacity)
// {
//     print(
//         "\n%s-- Perf Timer --%s\n"
//         "  %scount%s: %s%u%s"
//         "  %saccum%s: %s%llu%s"
//         "  %sdelta%s: %s%u%s\n"
//         "  %srepeat%s: %s%u%s"
//         "  %sbuf_capacity%s: %s%u%s\n",
//         YELLOW, CLEAR,
//         ry, CLEAR, GREEN, t->count, CLEAR,
//         ry, CLEAR, GREEN, t->accum, CLEAR,
//         ry, CLEAR, GREEN, t->delta, CLEAR,
//         br2, CLEAR, GREEN, repeat, CLEAR,
//         br2, CLEAR, GREEN, buf_capacity, CLEAR
//     );
//
//     const uint32_t avg = perf_timer_get_avg(t);
//     print("  %savg%s: %s%d%s nano", ry, CLEAR, YELLOW, avg, CLEAR);
//     print("  %savg%s: %s%ld%s micro\n", ry, CLEAR, YELLOW, avg / NANO_TO_MICRO, CLEAR);
//     const uint32_t fps = perf_timer_get_fps(t);
//     print("  %sfps%s: %s%d%s\n", ry, CLEAR, YELLOW, fps, CLEAR);
//
//     print("\n");
// }
