/*
 * Created by ckalo on 2/18/26.
 */

#include "buffer_internals.h"
#include "logger.h"
#include "mem_man_interface.h"
#include "precision_timer.h"
#include "precision_timer_logger.h"

struct sa {
    uint32 x, y;
};

uint32 result(struct sa* sa, const uint32 vala, const uint32 valb)
{
    sa->x = vala;
    sa->y = valb;
    return sa->x;
}


typedef uint32 (*fnptr)(struct sa*, uint32, uint32);





int main(void)
{
    memman_init();
    buffer_init_table();
    timer_init_table();
    struct timer* t = timer_create_perf_timer();

    const uint32 repeat = 10000000;

    fnptr res = &result;

#define SA_LEN 6
    struct sa sa[SA_LEN] = {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}};

    print("%sTest: performance%s {%sfunction calls VS function pointers%s}\n",
        ry, CLEAR, YELLOW, CLEAR
    );


    uint32 r = 0;

    /**********************************************************************************************/
    print("%s-- FUNCTION CALL --%s\n", ry, CLEAR);
    perf_timer_start(t);
    for(uint32 i = 0; i < repeat; i++) {
        for(uint32 j = 0; j < SA_LEN; j++) {

            uint32 f = result(&sa[j], i, j);
            r += f+sa[j].y;
            // print("%s%u%s\n", ry, r+sa[j].y, CLEAR);
        }
    }
    perf_timer_stop(t);

    perf_timer_print(timer_print_config((timer_print_params){ .t = t, .repeat = repeat }));
    perf_timer_reset(t);
    print("%s-- result = %u --%s\n", ry, r, CLEAR);



    /**********************************************************************************************/
    print("%s-- FUNCTION POINTER --%s\n", ry, CLEAR);
    perf_timer_start(t);
    r = 0;
    for(uint32 i = 0; i < repeat; i++) {
        for(uint32 j = 0; j < SA_LEN; j++) {
            uint32 f = res(&sa[j], i, j);
            r += f + sa[j].y;
        }
    }
    perf_timer_stop(t);

    perf_timer_print(timer_print_config((timer_print_params){ .t = t, .repeat = repeat }));
    perf_timer_reset(t);
    print("%s-- result = %u --%s\n", ry, r, CLEAR);

    buffer_destroy_table();
    memman_terminate();

    return 0;
}
