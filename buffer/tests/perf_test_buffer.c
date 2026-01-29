/*
 * Created by ckalo on 1/8/26.
 */

#include "perf_test_buffer.h"
#include "buffer_log.h"
#include "logger.h"
#include "mem_man.h"
#include "precision_timer.h"


/*
 * operations-helpers
 */

void perf_timer_print(const uint32 repeat,
                      const uint32 buffer_size,
                      const uint32 type_size)
{
    print("%s--- Timer ---%s\n", CYAN, CLEAR);
    const uint32_t fps = perf_timer_get_fps();
    print("  fps: %d\n", fps);
    const uint32_t avg = perf_timer_get_avg();
    print("  avg: %d nanosec\n", avg);
    print("  avg: %ld microsec\n", avg / NANO_TO_MICRO);
    perf_timer_reset();

    print(
        "  repeat: %s%u%s\n"
        "  buffer_size: %s%u%s\n"
        "  type_size: %s%u%s\n",
        CYAN, repeat, CLEAR,
        CYAN, buffer_size, CLEAR,
        CYAN, type_size, CLEAR
        );

    print("%s--- Timer ---%s\n", CYAN, CLEAR);
}

static void fill(buffer_t* const buffer)
{
    for (uint32 i = 0; i < buffer->capacity; i++)
    {
        buffer_push_uint32(i+1, buffer);
    }
}
static void insert_at(const uint32 val,
                      buffer_t* const buffer,
                      const uint32 index)
{
    if (index != INT_NULL)
        buffer_insert_uint32(val, buffer, index);
}
static void remove_from(buffer_t* const buffer, const uint32 index)
{
    buffer_remove_uint32(buffer, index);
}

// A function heavy on divisions that transforms a seed
// This is intentionally inefficient with many division operations
uint64_t division_heavy_hash(uint32 seed, uint32 i);


/*******************************************************************************
 * test: measure performance of:
 * remove buffer element
 * insert buffer element
 ******************************************************************************/
void perf_test_buffer_remove_insert()
{
    const uint32_t num_pages = 4;
    memory_manager_init(num_pages);

    const uint32 repeat = 1000000;

    // allocate new buffer
    const uint32 capacity  = 64;
    buffer_t* buffer = buffer_create_uint32(capacity);


    // fill buffer
    fill(buffer);

    buffer_print_uint32((buffer_print_config((buffer_print_p){
        .buffer = buffer, .print_num_elems = buffer->capacity
    })));


    const int32 index = 9;

    // remove an element, using free buffer to store all free buffer elems
    print("%s-- REMOVE ELEMENTS AT INDEX:%d%s\n\n", gc, index, CLEAR);
    perf_timer_start();
    for (uint32 i = 0; i < repeat; i++) {
        for (uint32 j = 0; j < buffer->capacity; j++) {
            remove_from(buffer, j);
        }
    }
    perf_timer_stop_with_count(repeat*buffer->capacity);
    perf_timer_print(repeat, buffer->capacity, sizeof(buffer->data[0]));


    // insert a value in the buffer at specific index, using the free buffer 'find' a free element
    print("%s-- INSERT ELEMENTS AT INDEX:%d%s\n\n", gc, index, CLEAR);
    perf_timer_start();
    for (uint32 i = 0; i < repeat; i++) {
        for (uint32 j = 0; j < buffer->capacity; j++) {
            const uint32 v = (((i+1) / 2) * i) / 2;
            // const uint32 v = division_heavy_hash(i, i);
            insert_at(v, buffer, j);
        }
    }
    perf_timer_stop_with_count(repeat*buffer->capacity);
    perf_timer_print(repeat, buffer->capacity, sizeof(buffer->data[0]));

    // buffer_log_print_uint32((buffer_print_config((buffer_print_p){
    //     .buffer = buffer, .print_num_elems = buffer->capacity
    // })));

    memory_manager_terminate();
}



/*******************************************************************************
 * test: measure performance overhead of pushing to buffer
 * inside the loop compare to push via function overhead (using the buffer id)
 ******************************************************************************/
void push_abstructed(const uint16 id, const uint32 index, uint32 val)
{
    void* address         = Temp_get_address_by_id(id); // Get memory directly from mem manager
    buffer_t* buffer      = (buffer_t*)address;
    uint32* data = buffer->data;
    data[index]   = val;
    buffer->tail++;
    buffer->count++;
}

void perf_test_insert_abstruct_call()
{
    print("%s-- TEST: --\n", ry, CLEAR);
    print("%s measure performance overhead\n of pushing to buffer\n"
    " from inside the loop\n compare to push via function\n using  the buffer id%s\n\n",
    ry, CLEAR);

    const uint32_t num_pages = 4;
    memory_manager_init(num_pages);


    // allocate new buffer
    const uint32 capacity  = 16;
    buffer_t* buffer = buffer_create_uint32(capacity);


    // fill buffer
    const uint32 len = 64;
    const uint32 repeat = 1000000;
    print("%s-- buffer push len: %u values%s\n", ry, len, CLEAR);
    print("%s-- test repeat: %u times%s\n\n", ry, repeat, CLEAR);


    print("%s-- measurment outside the loops%s\n", GREEN, CLEAR);
    // measure inside the loop
    {
        print("%s-- test: with function call overhead%s\n", BLUE, CLEAR);
        perf_timer_start();
        for (uint32 k = 0; k < repeat; k++)
        {
            for (uint32 i = 0; i < len; i++)
            {
                push_abstructed(buffer->id, i, 1);
            }
        }
        perf_timer_stop_with_count(repeat*len);
        perf_timer_print(repeat, len, sizeof(buffer->data[0]));


        buffer->tail = -1;
        buffer->count = 0;

        print("%s-- test: push inside the loop%s\n", BLUE, CLEAR);
        perf_timer_start();
        uint32* data = buffer->data;
        for (uint32 k = 0; k < repeat; k++)
        {
            for (uint32 i = 0; i < len; i++)
            {
                data[i] = 0;
                buffer->tail++;
                buffer->count++;
            }
        }
        perf_timer_stop_with_count(repeat*len);
        perf_timer_print(repeat, len, sizeof(buffer->data[0]));
    }

    print("%s-- TEST: --\n", ry, CLEAR);

    /*
     * Results

     */

  memory_manager_terminate();
}


uint64_t division_heavy_hash(uint32 seed, uint32 i) {
    uint64_t value = seed;

    // Chain multiple division operations with varying divisors
    // Using primes and odd numbers as divisors

    // First series of divisions
    value = (value / 3) ^ (value % 7);
    value = value / 5 + (value % 11) * 13;
    value = (value / 17) | (value % 19);

    // More divisions with mixing
    value = ((value / 23) + (value % 29)) * ((value / 31) - (value % 37));
    value = (value / 41) ^ (value / 43) ^ (value % 47);

    // Nested division expressions
    value = ((value / 53) * (value % 59)) / ((value % 61) + 1);
    value = (value / 67) + ((value % 71) / 73) * 79;

    // // Division chains
    // value = (((value / 83) % 89) / 97) * 101;
    // value = (value / 103) + (value / 107) - (value / 109);
    //
    // // More complex division patterns
    // uint64_t temp = value;
    // value = (temp / 113) * (temp % 127);
    // value = value / ((temp % 131) + 137) + (temp / 139);
    // //
    // // Mix with iteration counter
    // value = value / (i + 151) + (value % (i + 157)) * 163;
    // value = ((value / 167) + i) * ((value % 173) - i);

    // Ensure value doesn't become zero (avoid division by zero later)
    if (value == 0) value = seed + i + 1;

    return value;
}