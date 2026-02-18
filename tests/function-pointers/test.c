/*
 * Created by ckalo on 1/3/26.
 */
#include "test.h"
#include "my_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "buffer.h"
#include "mem_man_interface.h"
#include "precision_timer.h"
#include "precision_timer_logger.h"

#define buffer_len 1

typedef struct extras {
    bool a;
    uint32 b;
    bool c;
    uint64 d;
    bool e;
    uint64 f;
    bool g;
} extras_t;

typedef struct obj {
    uint32 pos[3];
    uint32 col[4];
    float result;
    // extras_t ex;
    // extras_t ex2;
} obj_t;

typedef struct obj2 {
    uint32 pos[3 * buffer_len];
    uint32 col[4 * buffer_len];
    float result[buffer_len];
    // extras_t ex;
    // extras_t ex2;
} obj2_t;

void
initialize_obj1(obj_t* obj, const uint32 len, uint32 random)
{
    uint32 pos_len = sizeof(obj[0].pos) / sizeof(obj[0].pos[0]);
    uint32 col_len = sizeof(obj[0].col) / sizeof(obj[0].col[0]);
    for(uint32 j = 0; j < len; j++) {
        // printf("%u\n", random);
        random = (rand() % 100) + 10;
        for(uint32 k = 0; k < pos_len; k++) obj[j].pos[k] = random;
        for(uint32 k = 0; k < col_len; k++) obj[j].col[k] = random;
        obj[j].result = 0;
    }
}

void
initialize_obj2(obj2_t* obj, const uint32 len, uint32 random)
{
    uint32 pos_len = sizeof(obj->pos) / (sizeof(obj->pos[0]));
    uint32 col_len = sizeof(obj->col) / (sizeof(obj->col[0]));
    uint32 result_len = len;

    for(uint32 k = 0; k < pos_len; k++) {
        random = rand() % 100 + 10;
        obj->pos[k] = random;
    }
    for(uint32 k = 0; k < col_len; k++) {
        random = rand() % 100 + 10;
        obj->col[k] = random;
    }
    for(uint32 k = 0; k < result_len; k++) {
        random = rand() % 100 + 10;
        obj->result[k] = 0.f;
    }
}

void
print_obj(obj_t* obj, const uint32 len)
{
    printf("obj 1 -------------------\n");
    uint32 k = 0;
    for(uint32 j = 0; j < len; j++) {
        // printf("x:%u y:%u result:%.2f\n",
        printf("x:%u y:%u z:%u\n",
               obj[j].pos[k],
               obj[j].pos[k + 1],
               obj[j].pos[k + 2]
        );
    }
}

void
print_obj2(const obj2_t* obj, const uint32 len)
{
    printf("obj 2 -------------------\n");
    uint32 k = 0;
    for(uint32 j = 0; j < len; j++) {
        // printf("x:%u y:%u result:%.2f\n",
        printf("x:%u y:%u z:%u\n",
               obj->pos[k],
               obj->pos[k + 1],
               obj->pos[k + 2]
        );
        k += 3;
    }
}


float
calc_magnitude1(obj_t* obj, const uint32 len, const float f)
{
    for(uint32 j = 0; j < len; j++) {
        // perf_timer_start_accum();
        float mag = (float) sqrt(
            obj[j].pos[0] * obj[j].pos[0] +
            obj[j].pos[1] * obj[j].pos[1]
        );
        obj[j].result = obj[j].result + mag * f;
        // perf_timer_stop_accum();
    }
}

float
calc_magnitude2(obj2_t* obj, const uint32 len, const float f)
{
    uint32 k = 0;
    for(uint32 j = 0; j < len; j++) {
        float mag = (float) sqrt(
            obj->pos[j + k] * obj->pos[j + k++] +
            obj->pos[j + k] * obj->pos[j + k++]
        );
        obj->result[j] = obj->result[j] + mag * f;
        k += 3;
        // perf_timer_stop_accum();
    }
}

void
test()
{
    memman_init();
    buffer_init_table();
    timer_init_table();
    struct timer* t = timer_create_perf_timer();

    const uint32 repeat = 50000000;
    uint32 random = rand() % 100;

    obj_t buffer[buffer_len];
    initialize_obj1(buffer, buffer_len, random);
    printf("-- obj1 --\n buffer size:%lu buffer[0] size:%lu\n",
           sizeof(buffer),
           sizeof(buffer[0]) / sizeof(uint32)
    );
    // print_obj(buffer, buffer_len);

    obj2_t buffer2;
    printf("-- obj2 --\n pos size:%lu pos elem  size:%lu\n",
           sizeof(buffer2.pos) / sizeof(uint32),
           sizeof(buffer2.pos[0])
    );
    initialize_obj2(&buffer2, buffer_len, random);
    // print_obj2(&buffer2, buffer_len);

    { // test array of structures
        perf_timer_start(t);
        for(uint32 i = 0; i < repeat; i++)
            calc_magnitude1(buffer, buffer_len, 1.34f);

        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repeat, }));
        perf_timer_reset(t);
    }

    { // test structure of arrays
        perf_timer_start(t);
        for(uint32 i = 0; i < repeat; i++)
            calc_magnitude2(&buffer2, buffer_len, 1.34f);

        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repeat, }));
        perf_timer_reset(t);
    }


    memory_manager_terminate();
}
