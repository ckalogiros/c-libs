#include "buffer_internals.h"
#include "mem_man.h"
#include "precision_timer_logger.h"
#include "src/precision_timer.h"
#include <stdio.h>
#include "src/cpu_time_stamp.h"

#include <stdint.h>

void test_perf_timer(struct timer* t, int num);
void test_cpu_time_stamps(int num);
void test_timer_table_impl();


int main(void) {

    // const int num = 100000;
    // test_perf_timer(num);
    // test_cpu_time_stamps(num);

    test_timer_table_impl();

    return 0;
}

void test_perf_timer(struct timer* t, const int num) {

    for (int i = 0; i < num; i++) {
        perf_timer_start(t);
        // printf(".");
        // sleep(0);
        perf_timer_stop(t);
    }

    printf("--- Timer\n");
    const uint32 fps = perf_timer_get_fps(t);
    printf("fps: %d\n", fps);
    const uint32 avg = perf_timer_get_avg(t);
    printf("avg: %d nanosec\n", avg);
    const uint64_t accum = perf_timer_get_accum(t);
    printf("accum: %ld nanosec\n", accum);
}

void test_cpu_time_stamps(const int num) {

    for (int i = 0; i < num; i++) {
        cycle_counter_start();
        // printf(".");
        // sleep(0);
        cycle_counter_stop();
        // const uint32_t dt = perf_timer_get_delta();
        // printf("delta: %ld\n", dt);
    }

    printf("--- CPU cycles\n");
    const uint32_t avg = cycle_counter_get_avg();
    printf("avg: %d cpu cycles\n", avg);
    // const uint32_t fps = cycle_counter_get_fps();
    // printf("fps: %d\n", fps);
    const uint64_t accum = cycle_counter_get_accum();
    printf("accum: %ld total cpu cycles\n", accum);
}


void test_timer_table_impl() {

  memory_manager_init(10);
  buffer_init_table();

  timer_init_table();


  struct timer* t = timer_create_timer();

  temp_debug_print_timer(t);

  perf_timer_start(t);
  perf_timer_stop(t);

  temp_debug_print_timer(t);

  memory_manager_terminate();

}