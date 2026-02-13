/*
 * Created by ckalo on 1/28/26.
 */
#ifndef PERF_TIMER_PRECISION_TIMER_STRUCT_H
#define PERF_TIMER_PRECISION_TIMER_STRUCT_H

#include "my_types.h"

struct timer {
    uint64 start;
    uint64 stop;
    uint64 accum;
    uint32 delta;
    uint32 avg;
    uint32 count;
};

/* https://chat.deepseek.com/a/chat/s/6b0b9513-7909-4d43-afe4-a72bf5e824c6
 *
Option 2: X-Macro pattern (Recommended for automation)
c

// Define all flags in one place
#define FLAG_LIST \
    FLAG_ENTRY(FPS, "fps") \
    FLAG_ENTRY(AVG_NANO, "average nanoseconds") \
    FLAG_ENTRY(AVG_MICRO, "average microseconds") \
    FLAG_ENTRY(AVG_MILI, "average milliseconds") \
    FLAG_ENTRY(MIN_TIME, "minimum time") \
    FLAG_ENTRY(MAX_TIME, "maximum time") \
    FLAG_ENTRY(STD_DEV, "standard deviation")

// Generate the enum with automatic bit positions
enum PERF_TIMER_FLAGS {
    #define FLAG_ENTRY(name, desc) PERF_T_##name,
    FLAG_LIST
    #undef FLAG_ENTRY
    PERF_T_COUNT  // Number of flags
};

// Generate the bitmask constants
#define FLAG_ENTRY(name, desc) \
    static const uint64_t PERF_T_##name##_MASK = 1ULL << PERF_T_##name;
FLAG_LIST
#undef FLAG_ENTRY
||||||||||||||||||| OR
// Or if you prefer #defines:
#define FLAG_ENTRY(name, desc) \
    #define PERF_T_##name (1ULL << PERF_T_##name)
// FLAG_LIST  // Uncomment to use #defines
// #undef FLAG_ENTRY
 */


enum PERF_TIMER_PRINT_FLAGS {
    PERF_T_FPS = BIT_MASK_FROM_UINT(0), // perf timer print: fps
    PERF_T_AVG_NANO = BIT_MASK_FROM_UINT(1), // perf timer print: average in nanoseconds
    PERF_T_AVG_MICRO = BIT_MASK_FROM_UINT(2), // perf timer print: average in microseconds
    PERF_T_AVG_MILI = BIT_MASK_FROM_UINT(3), // perf timer print: average in miliseconds
    PERF_T_COUNT = BIT_MASK_FROM_UINT(4), // perf timer print: count
    PERF_T_ACCUM = BIT_MASK_FROM_UINT(5), // perf timer print: delta accumulator (in nanoseconds)
    PERF_T_DELTA = BIT_MASK_FROM_UINT(6), // perf timer print: delta time (in nanoseconds)
    PERF_T_REPEAT = BIT_MASK_FROM_UINT(7), // perf timer print: how many times the measurment was repeated
    PERF_T_BUF_CAPACITY = BIT_MASK_FROM_UINT(8), // perf timer print: buffer size, if buffer is used for the measurment
    PERF_T_USE_REPEAT = BIT_MASK_FROM_UINT(9), // perf timer print: use repeat in the calculation
    PERF_T_USE_BUF_CAPACITY = BIT_MASK_FROM_UINT(10), // perf timer print: use buffer size in the calculation
    PERF_T_IN_CONTINUATION = BIT_MASK_FROM_UINT(11), // perf timer print: continued timer printings. skip some new lines and printing header


    PERF_T_ALL = BIT_MASK_FROM_UINT(29),
    PERF_T_COMMON = BIT_MASK_FROM_UINT(30),
    PERF_T_INLINE = BIT_MASK_FROM_UINT(31),
    // PERF_T_ = BIT_MASK_FROM_UINT(31),
};

typedef struct {
    struct timer* t;
    uint32 repeat;
    uint32 buf_capacity;
    enum PERF_TIMER_PRINT_FLAGS flags;
} timer_print_params;

static timer_print_params
timer_print_config(timer_print_params in)
{
    timer_print_params out = {
        .t = NULL,
        .repeat = 1,
        .buf_capacity = 1,
        .flags = PERF_T_COMMON | PERF_T_USE_REPEAT | PERF_T_REPEAT | PERF_T_INLINE | PERF_T_IN_CONTINUATION,
    };

    /* override defaults if user provided values */
    if(in.t) out.t = in.t;
    if(in.repeat) out.repeat = in.repeat;
    if(in.buf_capacity) out.buf_capacity = in.buf_capacity;
    if(in.flags) out.flags = in.flags;

    return out;
}


#endif //PERF_TIMER_PRECISION_TIMER_STRUCT_H
