#include <stdio.h>
#include <string.h>

#include "buffer_arr_internals.h"
#include "logger.h"
#include "mem_man_interface.h"
#include "precision_timer.h"
#include "smath.h"


#define LEN 2048*16
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

void test_memcpy_vs_loop();
void test_struct_vs_flat_buffer();

int
main(void)
{
    printf("Hello, World!\n");

    memman_init();
    buffer_arr_init_table(2);
    timer_init_table();

    // test_memcpy_vs_loop();
    test_struct_vs_flat_buffer();


    /* CLOSE APP */
    buffer_arr_destroy_table();
    memman_terminate();
    return 0;
}

void test_memcpy_vs_loop()
{
    float arr1[LEN] = {0.0};
    float arr2[LEN] = {0.0};
    // buffer_t* buf = buffer_arr_constructor_gentype(LEN*SIZE_F32, LEN);

    /* PERF-TIMER */
    struct timer* t = timer_create_perf_timer();


    for (int i = 0; i < LEN; i++) {
        arr1[i] = i;
    }

    uint32 num_times = 10000;
    for (int k = 0; k < num_times; k++) {

        perf_timer_start_accum(t);
        for (int i = 0; i < LEN; i++) {
            arr2[i] = arr1[i];
        }
        perf_timer_stop_accum(t);

    }

    perf_timer_calc_accum(t);
    uint32 fps1 = perf_timer_get_fps(t);
    print("fps: %u\n", fps1);

    perf_timer_reset(t);

    for (int k = 0; k < num_times; k++) {

        perf_timer_start_accum(t);
        memcpy(arr2, arr1, LEN * sizeof(float));
        perf_timer_stop_accum(t);

    }

    perf_timer_calc_accum(t);
    uint32 fps2 = perf_timer_get_fps(t);
    print("fps: %u\n", fps2);

    print("ratio: %.3f\n", MIN((double)fps1, (double)fps2) / MAX((double)fps1, (double)fps2));
}


void test_struct_vs_flat_buffer()
{
    const uint32 elems = 8 * 8;
    struct ss {
        float a1; float a2; float a3; float a4; float a5; float a6; float a7; float a8;
        float b1; float b2; float b3; float b4; float b5; float b6; float b7; float b8;
        float c1; float c2; float c3; float c4; float c5; float c6; float c7; float c8;
        float d1; float d2; float d3; float d4; float d5; float d6; float d7; float d8;
        float e1; float e2; float e3; float e4; float e5; float e6; float e7; float e8;
        float f1; float f2; float f3; float f4; float f5; float f6; float f7; float f8;
        float g1; float g2; float g3; float g4; float g5; float g6; float g7; float g8;
        float h1; float h2; float h3; float h4; float h5; float h6; float h7; float h8;
        float i1; float i2; float i3; float i4; float i5; float i6; float i7; float i8;
        float j1; float j2; float j3; float j4; float j5; float j6; float j7; float j8;
    };

    struct ss s = {0};
    float buf[64] = {0};


    /* PERF-TIMER */
    struct timer* t = timer_create_perf_timer();

    uint32 num_times = 100000;
    for (int k = 0; k < num_times; k++) {
        float val = k;
        perf_timer_start_accum(t);
        s.a1=val; s.a2=val; s.a3=val; s.a4=val; s.a5=val; s.a6=val; s.a7=val; s.a8=val; // 0
        s.b1=val; s.b2=val; s.b3=val; s.b4=val; s.b5=val; s.b6=val; s.b7=val; s.b8=val; // 1
        s.c1=val; s.c2=val; s.c3=val; s.c4=val; s.c5=val; s.c6=val; s.c7=val; s.c8=val; // 2
        s.d1=val; s.d2=val; s.d3=val; s.d4=val; s.d5=val; s.d6=val; s.d7=val; s.d8=val; // 3
        s.e1=val; s.e2=val; s.e3=val; s.e4=val; s.e5=val; s.e6=val; s.e7=val; s.e8=val; // 4
        s.f1=val; s.f2=val; s.f3=val; s.f4=val; s.f5=val; s.f6=val; s.f7=val; s.f8=val; // 5
        s.g1=val; s.g2=val; s.g3=val; s.g4=val; s.g5=val; s.g6=val; s.g7=val; s.g8=val; // 6
        s.h1=val; s.h2=val; s.h3=val; s.h4=val; s.h5=val; s.h6=val; s.h7=val; s.h8=val; // 7
        s.i1=val; s.i2=val; s.i3=val; s.i4=val; s.i5=val; s.i6=val; s.i7=val; s.i8=val; // 8
        s.j1=val; s.j2=val; s.j3=val; s.j4=val; s.j5=val; s.j6=val; s.j7=val; s.j8=val; // 9
        perf_timer_stop_accum(t);

    }

    perf_timer_calc_accum(t);
    uint32 fps1 = perf_timer_get_fps(t);
    print("fps: %u\n", fps1);

    perf_timer_reset(t);

    for (int k = 0; k < num_times; k++) {

        perf_timer_start_accum(t);
        for (int i = 0; i < elems; i++) {
            buf[i] = k;
        }
        perf_timer_stop_accum(t);

    }

    perf_timer_calc_accum(t);
    uint32 fps2 = perf_timer_get_fps(t);
    print("fps: %u\n", fps2);

    print("ratio: %.3f\n", MIN((double)fps1, (double)fps2) / MAX((double)fps1, (double)fps2));
}


#undef LEN
#undef MAX
#undef MIN

