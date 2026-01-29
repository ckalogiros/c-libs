#include <stdio.h>
#include "precision_timer.h"

void test_perf_timer(const int num);

int main(void) {

    const int num = 1000;
    test_perf_timer(num);

    printf("Hello, World!\n");
    return 0;
}

void test_perf_timer(const int num) {

    for (int i = 0; i < num; i++) {
        perf_timer_start();
        perf_timer_stop();
        // const uint32_t dt = perf_timer_get_delta();
        // printf("delta: %ld\n", dt);
    }

    printf("--- Timer\n");
    const uint32_t avg = perf_timer_get_avg();
    printf("avg: %d\n", avg);
    const uint32_t fps = perf_timer_get_fps();
    printf("fps: %d\n", fps);
    const uint32_t accum = perf_timer_get_accum();
    printf("fps: %d\n", accum);
}
