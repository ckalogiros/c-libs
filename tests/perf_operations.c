/*
 * Created by ckalo on 1/10/26.
 */

#include "perf_operations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "precision_timer.h"

#define LEN 64

void timer_print()
{
    print("--- Timer\n");
    const uint32_t avg = perf_timer_get_avg();
    print("  avg: %d nanosec\n", avg);
    perf_timer_reset();
}

void init_buffer(const uint32 len, uint32 *buffer)
{
    for (uint32 i = 0; i < len; i++) {
        buffer[i] = i;
    }
}

void init_buffer_float(const uint32 len, float *buffer)
{
    for (uint32 i = 0; i < len; i++) {
        buffer[i] = i;
    }
}

void test_prerf_basic_operations()
{
    uint32 buffer[LEN] = {0};
    init_buffer(LEN, buffer);
    float buffer_float[LEN] = {0};
    init_buffer_float(LEN, buffer_float);
    const uint32 repeat = 1000000;
    print("%s-- buffer len: %u values%s\n", ry, LEN, CLEAR);
    print("%s-- test repeat: %u times%s\n\n", ry, repeat, CLEAR);
    // empty for loop
    print("%s-- test: empty%s\n", br2, CLEAR);
    {
        perf_timer_start();
        for (uint32 k = 0; k < repeat; k++) {
            for (uint32 i = 0; i < LEN; i++) {}
        }
        perf_timer_stop_with_count(repeat * LEN);
        timer_print();
    }
    // add
    print("%s-- test: add%s, 10x per measure\n", br2, CLEAR);
    {
        perf_timer_start();
        for (uint32 k = 0; k < repeat; k++) {
            for (uint32 i = 0; i < LEN; i++) {
                buffer[i] += i;
                buffer[i] += i + 1;
                buffer[i] += i + 2;
                buffer[i] += i + 3;
                buffer[i] += i + 4;
                buffer[i] += i + 5;
                buffer[i] += i + i + 1;
                buffer[i] += i + i + 2;
                buffer[i] += i + i + 3;
                buffer[i] += i + i + 4;
            }
        }
        perf_timer_stop_with_count(repeat * LEN);
        timer_print();
    }
    // sub
    print("%s-- test: sub%s, 10x per measure\n", br2, CLEAR);
    {
        perf_timer_start();
        for (uint32 k = 0; k < repeat; k++) {
            for (uint32 i = 0; i < LEN; i++) {
                buffer[i] -= i + 1;
                buffer[i] -= i + 2;
                buffer[i] -= i + 3;
                buffer[i] -= i + 4;
                buffer[i] -= i + 5;
                buffer[i] -= i + i + 1;
                buffer[i] -= i + i + 2;
                buffer[i] -= i + i + 3;
                buffer[i] -= i + i + 4;
                buffer[i] -= i + i + 5;
            }
        }
        perf_timer_stop_with_count(repeat * LEN);
        timer_print();
    }
    // mult
    print("%s-- test: mult%s, 10x per measure\n", br2, CLEAR);
    {
        perf_timer_start();
        for (uint32 k = 0; k < repeat; k++) {
            for (uint32 i = 0; i < LEN; i++) {
                buffer[i] *= i + 1;
                buffer[i] *= i + 2;
                buffer[i] *= i + 3;
                buffer[i] *= i + 4;
                buffer[i] *= i + 5;
                buffer[i] *= i + i + 1;
                buffer[i] *= i + i + 2;
                buffer[i] *= i + i + 3;
                buffer[i] *= i + i + 4;
                buffer[i] *= i + i + 5;
            }
        }
        perf_timer_stop_with_count(repeat * LEN);
        timer_print();
    }
    // div
    print("%s-- test: div%s, 10x per measure\n", br2, CLEAR);
    {
        perf_timer_start();
        for (uint32 k = 0; k < repeat; k++) {
            for (uint32 i = 0; i < LEN; i++) {
                buffer[i] /= i + 1;
                buffer[i] /= i + 2;
                buffer[i] /= i + 3;
                buffer[i] /= i + 4;
                buffer[i] /= i + 5;
                buffer[i] /= i + i + 1;
                buffer[i] /= i + i + 1;
                buffer[i] /= i + i + 1;
                buffer[i] /= i + i + 1;
                buffer[i] /= i + i + 1;
            }
        }
        perf_timer_stop_with_count(repeat * LEN);
        timer_print();
    }
    // mod
    print("%s-- test: mod%s, 10x per measure\n", br2, CLEAR);
    {
        perf_timer_start();
        for (uint32 k = 0; k < repeat; k++) {
            for (uint32 i = 0; i < LEN; i++) {
                buffer[i] %= i + 1;
                buffer[i] %= i + 2;
                buffer[i] %= i + 3;
                buffer[i] %= i + 4;
                buffer[i] %= i + 5;
                buffer[i] %= i + i + 1;
                buffer[i] %= i + i + 1;
                buffer[i] %= i + i + 1;
                buffer[i] %= i + i + 1;
                buffer[i] %= i + i + 1;
            }
        }
        perf_timer_stop_with_count(repeat * LEN);
        timer_print();
    }
    // sqrt
    print("%s-- test: sqrt%s, 10x per measure\n", br2, CLEAR);
    {
        perf_timer_start();
        for (uint32 k = 0; k < repeat; k++) {
            for (uint32 i = 0; i < LEN; i++) {
                buffer[i] = sqrt(buffer_float[i] + i + 1);
                buffer[i] = sqrt(buffer_float[i] + i + 2);
                buffer[i] = sqrt(buffer_float[i] + i + 3);
                buffer[i] = sqrt(buffer_float[i] + i + 4);
                buffer[i] = sqrt(buffer_float[i] + i + 5);
                buffer[i] = sqrt(buffer_float[i] + i + i + 1);
                buffer[i] = sqrt(buffer_float[i] + i + i + 1);
                buffer[i] = sqrt(buffer_float[i] + i + i + 1);
                buffer[i] = sqrt(buffer_float[i] + i + i + 1);
                buffer[i] = sqrt(buffer_float[i] + i + i + 1);
            }
        }
        perf_timer_stop_with_count(repeat * LEN);
        timer_print();
    }
    // sqrt
    print("%s-- test: pow%s, 10x per measure\n", br2, CLEAR);
    {
        perf_timer_start();
        for (uint32 k = 0; k < repeat; k++) {
            for (uint32 i = 0; i < LEN; i++) {
                buffer[i] = pow(buffer_float[i], 1);
                buffer[i] = pow(buffer_float[i], 2);
                buffer[i] = pow(buffer_float[i], 2);
                buffer[i] = pow(buffer_float[i], 2);
                buffer[i] = pow(buffer_float[i], 2);
                buffer[i] = pow(buffer_float[i], 2);
                buffer[i] = pow(buffer_float[i], 2);
                buffer[i] = pow(buffer_float[i], 2);
                buffer[i] = pow(buffer_float[i], 2);
                buffer[i] = pow(buffer_float[i], 2);
            }
        }
        perf_timer_stop_with_count(repeat * LEN);
        timer_print();
    }
  // memcpy
  print("%s-- test: memcpy%s, 10x  per measure\n", br2, CLEAR);
  {
      perf_timer_start();
      const uint32 value = 333333;
      const uint32 size = sizeof(uint32);
      for (uint32 k = 0; k < repeat; k++) {
        for (uint32 i = 0; i < LEN; i++) {
          memcpy(&value, &buffer_float[i], size);
          memcpy(&value, &buffer_float[i], size);
          memcpy(&value, &buffer_float[i], size);
          memcpy(&value, &buffer_float[i], size);
          memcpy(&value, &buffer_float[i], size);
          memcpy(&value, &buffer_float[i], size);
          memcpy(&value, &buffer_float[i], size);
          memcpy(&value, &buffer_float[i], size);
          memcpy(&value, &buffer_float[i], size);
          memcpy(&value, &buffer_float[i], size);
        }
      }
      perf_timer_stop_with_count(repeat * LEN);
      timer_print();
  }
}

void test_perf_unaligned_memory_buffers()
{


//    uint32 alignment = 4096*2;
    uint32 alignment = sizeof(uint32);
    uint32 mem_size  = 4096*8;
    void *mem        = aligned_alloc(mem_size, alignment);
    volatile uint32 *buffer   = (uint32 *) mem;
    uint32 capacity  = mem_size / sizeof(uint32);
    uint32 repeat    = 100000;

    // create random indexes to access buffers
    uint32 random_access_indices[capacity];
    for (uint32 i = 0; i < capacity; i++) {
        random_access_indices[i] = rand() % capacity;
//        random_access_indices[i] = i;
//        printf("random_access_indices[%d] = %d\n", i, random_access_indices[i]);
    }

    memset(mem, 0, capacity);

    perf_timer_start();
    for (uint32 r = 0; r < repeat; r++) {
        for (uint32 i = 0; i < capacity; i++) {
            buffer[i] = random_access_indices[i];
        }
    }
    perf_timer_stop_with_count(repeat);
    timer_print();

    //    uintptr_t proper_alignment = sizeof(uint32);
    //    uintptr_t proper_alignment = 13;
    //    void* aligned_mem = (void*)(((uintptr_t)mem + proper_alignment - 1) & ~(proper_alignment - 1));
    uintptr_t missalignment = 0x3c1;
    void* miss_aligned_mem  = (void*)(((uintptr_t)mem + missalignment));
    buffer                  = (uint32 *) miss_aligned_mem;
    memset(mem, 0, capacity);



    perf_timer_start();
    for (uint32 r = 0; r < repeat; r++) {
        for (uint32 i = 0; i < capacity; i++) {
            buffer[i] = random_access_indices[i];
        }
    }
    perf_timer_stop_with_count(repeat);
    timer_print();

    print("\n");
    print("mem_size:%u bytes\nbuffer capacity: %u\ntype size:%u\n", mem_size, capacity, sizeof(buffer[0]));
    print("mem:              %p alignment:       %u\n", mem, alignment);
    print("miss_aligned_mem: %p missalignment:%u\n", miss_aligned_mem, missalignment);


    free(mem);

}
