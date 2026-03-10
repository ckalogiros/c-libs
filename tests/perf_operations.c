/*
 * Created by ckalo on 1/10/26.
 */

#include "perf_operations.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "precision_timer.h"
#include "precision_timer_logger.h"
#include "buffer_internals.h"
#include "mem_man_interface.h"

#define LEN 64

void
init_buffer(const uint32 len, uint32* buffer)
{
    for(uint32 i = 0; i < len; i++) {
        buffer[i] = i;
    }
}

void
init_buffer_float(const uint32 len, float* buffer)
{
    for(uint32 i = 0; i < len; i++) {
        buffer[i] = i;
    }
}

void
test_prerf_basic_operations()
{
    memman_init();
    buffer_init_table();
    timer_init_table();

    volatile uint32 buffer[LEN] = {0};
    init_buffer(LEN, buffer);
    float buffer_float[LEN] = {0};
    init_buffer_float(LEN, buffer_float);


    const uint32 repeat = 1000000;
    // const uint32 repeat = 100;

    struct obj {
        char* name;
        uint32 avg;
    };
    struct obj ti_results[24] = {0};
    uint32 cnt = 0;
    uint repetitions = repeat*(LEN-44); // leave 10x operations as average perf timer



    print("%s-- buffer len: %u values%s\n", ry, LEN, CLEAR);
    print("%s-- test repeat: %u times%s\n\n", ry, repetitions, CLEAR);

    struct timer* t = timer_create_perf_timer();

    // empty for loop
    print("%s-- test: empty%s\n", br2, CLEAR);
    {
        ti_results[cnt].name = "empty";
        perf_timer_start(t);
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {}
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    // add
    print("%s-- test: add%s, 10x per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "add";
        perf_timer_start(t);
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                buffer[i] += i;
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    // sub
    print("%s-- test: sub%s, 10x per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "sub";
        perf_timer_start(t);
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                buffer[i] -= i + 1;
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    // mult
    print("%s-- test: mult%s, 10x per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "mult";
        perf_timer_start(t);
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                buffer[i] *= i + 1;
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    // div
    print("%s-- test: div%s, 10x per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "div";
        perf_timer_start(t);
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                buffer[i] /= i + 1;
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    // mod
    print("%s-- test: mod%s, 10x per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "mod";
        perf_timer_start(t);
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                buffer[i] %= i + 1;
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    // sqrt
    print("%s-- test: sqrt%s, 10x per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "sqrt";
        perf_timer_start(t);
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                buffer[i] = sqrt(buffer_float[i] + i + 1);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    // sqrt
    print("%s-- test: pow%s, 10x per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "pow";
        perf_timer_start(t);
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                buffer[i] = pow(buffer_float[i], 1);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    // memcpy
    print("%s-- test: memcpy%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "memcpy";
        perf_timer_start(t);
        const uint32 value = 333333;
        const uint32 size = sizeof(uint32);
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                memcpy((void*)&value, &buffer_float[i], size);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }


    // sin
    print("%s-- test: sin%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "sin";
        perf_timer_start(t);
        double value = 0.0;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                value += sin((double)i+1);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    // cos
    print("%s-- test: cos%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "cos";
        perf_timer_start(t);
        double value = 0.0;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                value += cos((double)i+1);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    // tan
    print("%s-- test: tan%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "tan";
        perf_timer_start(t);
        double value = 0.0;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                value += tan((double)i+1);;
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    // acos
    print("%s-- test: acos%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "acos";
        perf_timer_start(t);
        double value = 0.0;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                value += acos((double)i+1);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    // asin
    print("%s-- test: asin%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "asin";
        perf_timer_start(t);
        double value = 0.0;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                value += asin((double)i+1);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    // atan
    print("%s-- test: atan%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "atan";
        perf_timer_start(t);
        double value = 0.0;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                value += atan((double)i+1);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    // atan2
    print("%s-- test: atan2%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "atan2";
        perf_timer_start(t);
        double value = 0.0;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                value += atan2((double)i+1, value);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }

    /* Return `sqrt(X*X + Y*Y)'.  */
    // hypot
    print("%s-- test: hypot%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "hypot";
        perf_timer_start(t);
        double value = 0.0;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                value += hypot((double)i+1, value);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }

    /* Exponential function of X.  */
    // exp
    print("%s-- test: exp%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "exp";
        perf_timer_start(t);
        double value = 0.0;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                value += exp(value);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }

    /* Natural logarithm of X.  */
    // log
    print("%s-- test: log%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "log";
        perf_timer_start(t);
        double value = 0.0;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                value += log(value);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }

    /* Base-ten logarithm of X.  */
    // log10
    print("%s-- test: log10%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "log10";
        perf_timer_start(t);
        double value = 0.0;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                value += log10(value);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }

    /* Compute base-2 logarithm of X.  */
    // log2
    print("%s-- test: log2%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "log2";
        perf_timer_start(t);
        double value = 0.0;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                value += log2(value);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    print("%s-- test: log2%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "log2";
        perf_timer_start(t);
        double value = 0.0;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                value += log2(value);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }
    /* Break VALUE into integral and fractional parts.  */
    // modf
    print("%s-- test: modf%s, 10x  per measure\n", br2, CLEAR);
    {
        ti_results[cnt].name = "modf";
        perf_timer_start(t);
        double value = 0.0;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < LEN; i++) {
                value += modf(value, &value);
            }
        }
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repetitions, }));
        ti_results[cnt].avg = t->avg = t->accum / repetitions; cnt++;
        perf_timer_reset(t);
    }


    /* Compute base-2 exponential of X.  */
    // exp2


    /* Smallest integral value not less than X.  */
    // ceil

    /* Absolute value of X.  */
    // fabs

    /* Largest integer not greater than X.  */
    // floor

    /* Floating-point modulo remainder of X/Y.  */
    // fmod

    /* Break VALUE into a normalized fraction and an integral power of 2.  */
    // frexp

    /* X times (two to the EXP power).  */
    // ldexp



    for(uint32 k = 0; k < cnt; k++) {

        print("%s%u%snano %s\n", YELLOW, ti_results[k].avg, CLEAR, ti_results[k].name);
    }
    memman_terminate();
}

void
test_perf_unaligned_memory_buffers()
{
    memman_init();
    buffer_init_table();
    timer_init_table();
    struct timer* t = timer_create_perf_timer();


    //    uint32 alignment = 4096*2;
    // uint32 alignment = 16;
    uint32 alignment = sizeof(uint32);
    uint32 mem_size = 4096 * 1;
    void* mem = aligned_alloc(mem_size, alignment);
    volatile uint32* buffer = (uint32*) mem;
    uint32 capacity = mem_size / sizeof(uint32);
    uint32 repeat = 500000;

    // create random indexes to access buffers
    uint32 random_access_indices[capacity];
    for(uint32 i = 0; i < capacity; i++) {
        random_access_indices[i] = rand() % capacity;
        //        random_access_indices[i] = i;
        //        printf("random_access_indices[%d] = %d\n", i, random_access_indices[i]);
    }

    memset(mem, 0, capacity);

    perf_timer_start(t);
    for(uint32 r = 0; r < repeat; r++) {
        for(uint32 i = 0; i < capacity; i++) {
            buffer[i] = random_access_indices[i];
        }
    }
    perf_timer_stop(t);
    perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repeat, }));
    perf_timer_reset(t);

    //    uintptr_t proper_alignment = sizeof(uint32);
    //    uintptr_t proper_alignment = 13;
    //    void* aligned_mem = (void*)(((uintptr_t)mem + proper_alignment - 1) & ~(proper_alignment - 1));
    uintptr_t missalignment = 0x3c1;
    void* miss_aligned_mem = (void*) (((uintptr_t) mem + missalignment));
    buffer = (uint32*) miss_aligned_mem;
    memset(mem, 0, capacity);


    perf_timer_start(t);
    for(uint32 r = 0; r < repeat; r++) {
        for(uint32 i = 0; i < capacity; i++) {
            buffer[i] = random_access_indices[i];
        }
    }
    perf_timer_stop(t);
    perf_timer_print(timer_print_config((timer_print_params){.t = t, .repeat = repeat, }));
    perf_timer_reset(t);

    print("\n");
    print("mem_size:%u bytes\nbuffer capacity: %u\ntype size:%u\n", mem_size, capacity, sizeof(buffer[0]));
    print("mem:              %p alignment:       %u\n", mem, alignment);
    print("miss_aligned_mem: %p missalignment:%u\n", miss_aligned_mem, missalignment);


    free(mem);
}
