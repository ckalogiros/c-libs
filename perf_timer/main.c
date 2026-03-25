#include "logger.h"
#include "sparse_arr_internals.h"
#include "mem_man_interface.h"
#include "precision_timer_logger.h"
#include "src/precision_timer.h"
#include "src/cpu_time_stamp.h"
#include <stdio.h>
#include <stdint.h>


void test_perf_timer();
void test_cpu_time_stamps(int num);
void test_timer_table_impl();


int main(void) {

    // test_perf_timer();

    test_cpu_time_stamps(100000);

    // test_timer_table_impl();

    return 0;
}

void test_perf_timer() {

    memman_init();
    sparse_arr_init_table(1);
    timer_init_table();

    const uint32 repeat = 1000000;
    struct timer* t = timer_create_perf_timer();


    perf_timer_start(t);
    for (int i = 0; i < repeat; i++) {
        // printf(".");
        // usleep(10);
    }
    perf_timer_stop(t);

    // perf_timer_print_simple(t, repeat, 0);
    // perf_timer_print_simple_with_repeat(t, repeat, 0);

    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat,
        .flags = PERF_T_ALL | PERF_T_USE_REPEAT
    }));
    print("ALL-INLINE\n");
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat,
        .flags = PERF_T_ALL|PERF_T_INLINE
    }));
    print("COMON\n");
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat,
        .flags = PERF_T_COMMON|PERF_T_IN_CONTINUATION,
    }));
    print("COMON-INLINE\n");
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat,
        .flags = PERF_T_COMMON|PERF_T_IN_CONTINUATION|PERF_T_INLINE
    }));



    memman_terminate();
}

void test_cpu_time_stamps(const int num) {

    cycle_counter_start();
    for (int i = 0; i < num; i++) {
        // sleep(0);
        // const uint32_t dt = perf_timer_get_delta();
        // printf("delta: %ld\n", dt);
    }
    cycle_counter_stop();

    printf("--- CPU cycles\n");
    const uint32_t avg = cycle_counter_get_avg();
    printf("avg: %d cpu cycles\n", avg);
    // const uint32_t fps = cycle_counter_get_fps();
    // printf("fps: %d\n", fps);
    const uint64_t accum = cycle_counter_get_accum();
    printf("accum: %ld total cpu cycles\n", accum);
}


void test_timer_table_impl() {

  memman_init();
  sparse_arr_init_table(2);

  timer_init_table();


  struct timer* t = timer_create_perf_timer();

  // perf_timer_print(t);

  perf_timer_start(t);
  perf_timer_stop(t);

  // perf_timer_print(t);

  memman_terminate();

}