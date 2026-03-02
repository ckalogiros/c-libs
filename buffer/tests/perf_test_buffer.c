/*
 * Created by ckalo on 1/8/26.
 */

#include "perf_test_buffer.h"

#include <ctype.h>
#include <string.h>

#include "buffer_internals.h"
#include "buffer_log.h"
#include "file_sys_interface.h"
#include "logger.h"
#include "mem_man_internals.h"
#include "mem_structs.h"
#include "precision_timer.h"
#include "precision_timer_logger.h"
#include "slab_allocator_internals.h"
#include "str_utils.h"


/*
 * operations-helpers
 */

struct perf_results {
    uint32 fps;
    uint32 nanosec;
    uint32 microsec;
    uint32 buffer_size;
    uint32 type_size;
    char* name;
}
static perf_results_buf[16];

static uint32 perf_results_buf_cnt = 0;

/*******************************************************************************
 * private
 ******************************************************************************/

void
perf_results_set(struct timer* t, const uint32 buffer_size, const char* name)
{
    perf_results_buf[perf_results_buf_cnt].fps = perf_timer_get_fps(t);
    perf_results_buf[perf_results_buf_cnt].nanosec = perf_timer_get_avg(t);
    perf_results_buf[perf_results_buf_cnt].microsec = perf_timer_get_avg(t);
    perf_results_buf[perf_results_buf_cnt].buffer_size = buffer_size;
    perf_results_buf[perf_results_buf_cnt].name = name;
    perf_results_buf_cnt++;
}

void
perf_print_timer_resuls(const struct perf_results* results,
                        const uint32 results_cnt, const uint32 repeat)
{
    print("%s--- Perf Timer Results ---%s\n", CYAN, CLEAR);
    print("repeat:%s%u%s", br2, repeat, CLEAR);

    for(uint32 i = 0; i < results_cnt; i++) {
        if(i % 2 != 0) {
            print("test:%s[%u]%s", YELLOW, i, CLEAR);
            print(
                " buffer_size:%s%u%s"
                " type_size:%s%u%s\n",
                CYAN, results[i].buffer_size, CLEAR,
                CYAN, results[i].type_size, CLEAR
            );

            print(" %s%s%s", GREEN, results[i - 1].name, CLEAR);
            print(" vs %s%s%s\n", GREEN, results[i].name, CLEAR);

            print(" avg: %d nano", results[i - 1].nanosec);
            print(" avg: %ld micro\n", results[i - 1].nanosec / NANO_TO_MICRO);
            print(" avg: %d nano", results[i].nanosec);
            print(" avg: %ld micro\n", results[i].nanosec / NANO_TO_MICRO);
        }
    }
}

void
timer_print(struct timer* t, const uint32 repeat,
            const uint32 buffer_size, const uint32 type_size)
{
    print("%s--- Timer ---%s\n", CYAN, CLEAR);
    const uint32_t fps = perf_timer_get_fps(t);
    print("  fps: %d\n", fps);
    const uint32_t avg = perf_timer_get_avg(t);
    print("  avg: %d nanosec", avg);
    print("  avg: %ld microsec\n", avg / NANO_TO_MICRO);
    // perf_timer_reset(t);

    print(
        "  repeat: %s%u%s"
        "  buffer_size: %s%u%s"
        "  type_size: %s%u%s\n",
        CYAN, repeat, CLEAR,
        CYAN, buffer_size, CLEAR,
        CYAN, type_size, CLEAR
    );

    print("%s--- Timer ---%s\n", CYAN, CLEAR);
}

static void
fill(buffer_t* const buffer)
{
    for(uint32 i = 0; i < buffer->capacity; i++) {
        buffer_set_for_push(buffer);
        uint32* data = buffer->data;
        data[i] = i + 1;
    }
}

static void
insert_at(const uint32 val, buffer_t* const buffer, const uint32 index)
{
    if(index != INT_NULL)
        buffer_insert_uint32(val, buffer, index);
}

static void
remove_from(buffer_t* const buffer, const uint32 index)
{
    buffer_free_elem(buffer, index);
}

// A function heavy on divisions that transforms a seed
// This is intentionally inefficient with many division operations
uint64_t division_heavy_hash(uint32 seed, uint32 i);


/*******************************************************************************
 * test: measure performance of:
 * remove buffer element
 * insert buffer element
 ******************************************************************************/
void
perf_test_buffer_create_generic_vs_uint32(void)
{
    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%stest performance of allocating and releasing\n"
          "uint32 buffer versus generic buffer.%s\n\n", CYAN, CLEAR);

    const uint32_t num_pages = 16;
    memman_init();

    buffer_init_table();


    timer_init_table();
    struct timer* t = timer_create_perf_timer();

    const uint32 repeat = 5000000;
    // const uint32 repeat = 5000;
    const uint32 type_size = sizeof(uint32);


    /*****************************************************************************
     *  buffer capacity: 32
     ****************************************************************************/
    { // test: buffer capacity 32
        const uint32 capacity = 32;

        perf_timer_start(t);
        for(uint32 i = 0; i < repeat; i++) {
            buffer_t* buffer = buffer_create_uint32(capacity);
            buffer_destroy(buffer);
        }
        perf_timer_stop_with_count(repeat, t);
        perf_results_set(t, capacity, "buffer uint32");
        // timer_print(t, repeat, capacity, sizeof(uint32));
        perf_timer_reset(t);
    }
    { // test: generic
        const uint32 capacity = 32;

        perf_timer_start(t);
        for(uint32 i = 0; i < repeat; i++) {
            buffer_t* buffer = buffer_create_gentype(capacity, type_size);
            buffer_destroy(buffer);
        }
        perf_timer_stop_with_count(repeat, t);
        perf_results_set(t, capacity, "buffer generic");
        // timer_print(t, repeat, capacity, sizeof(uint32));
        perf_timer_reset(t);
    }


    /*****************************************************************************
    *  buffer capacity: 64
    ****************************************************************************/
    { // test: uint32
        const uint32 capacity = 64;

        perf_timer_start(t);
        for(uint32 i = 0; i < repeat; i++) {
            buffer_t* buffer = buffer_create_uint32(capacity);
            buffer_destroy(buffer);
        }
        perf_timer_stop_with_count(repeat, t);
        perf_results_set(t, capacity, "buffer uint32");
        // timer_print(t, repeat, capacity, sizeof(uint32));
        perf_timer_reset(t);
    }
    { // test: buffer capacity 64
        // allocate new buffer
        const uint32 capacity = 64;

        perf_timer_start(t);
        for(uint32 i = 0; i < repeat; i++) {
            buffer_t* buffer = buffer_create_uint32(capacity);
            buffer_destroy(buffer);
        }
        perf_timer_stop_with_count(repeat, t);
        perf_results_set(t, capacity, "buffer generic");
        // timer_print(t, repeat, capacity, type_size);
        perf_timer_reset(t);
    }


    /*****************************************************************************
    *  buffer capacity: 128
    ****************************************************************************/
    { // test: uint32
        const uint32 capacity = 128;

        perf_timer_start(t);
        for(uint32 i = 0; i < repeat; i++) {
            buffer_t* buffer = buffer_create_uint32(capacity);
            buffer_destroy(buffer);
        }
        perf_timer_stop_with_count(repeat, t);
        perf_results_set(t, capacity, "buffer uint32");
        // timer_print(t, repeat, capacity, sizeof(uint32));
        perf_timer_reset(t);
    }
    { // test: buffer capacity 128
        // allocate new buffer
        const uint32 capacity = 128;

        perf_timer_start(t);
        for(uint32 i = 0; i < repeat; i++) {
            buffer_t* buffer = buffer_create_uint32(capacity);
            buffer_destroy(buffer);
        }
        perf_timer_stop_with_count(repeat, t);
        perf_results_set(t, capacity, "buffer generic");
        // timer_print(t, repeat, capacity, type_size);
        perf_timer_reset(t);
    }


    /*****************************************************************************
    *  buffer capacity: 256
    ****************************************************************************/
    { // test: uint32
        // allocate new buffer
        const uint32 capacity = 256;

        perf_timer_start(t);
        for(uint32 i = 0; i < repeat; i++) {
            buffer_t* buffer = buffer_create_uint32(capacity);
            buffer_destroy(buffer);
        }
        perf_timer_stop_with_count(repeat, t);
        perf_results_set(t, capacity, "buffer uint32");
        // timer_print(t, repeat, capacity, sizeof(uint32));
        perf_timer_reset(t);
    }
    { // test: buffer capacity 256
        // allocate new buffer
        const uint32 capacity = 256;

        perf_timer_start(t);
        for(uint32 i = 0; i < repeat; i++) {
            buffer_t* buffer = buffer_create_uint32(capacity);
            buffer_destroy(buffer);
        }
        perf_timer_stop_with_count(repeat, t);
        perf_results_set(t, capacity, "buffer generic");
        // timer_print(t, repeat, capacity, type_size);
        perf_timer_reset(t);
    }


    perf_print_timer_resuls(perf_results_buf, perf_results_buf_cnt, repeat);

    memman_terminate();
}

/*******************************************************************************
 * test: measure performance of:
 * remove buffer element
 * insert buffer element
 ******************************************************************************/
void
perf_test_buffer_remove_insert(void)
{
    const uint32_t num_pages = 10;
    memman_init();

    // const uint32 repeat = 1000000;
    const uint32 repeat = 2000;

    buffer_init_table();

    timer_init_table();

    // allocate new buffer
    const uint32 capacity = 64;
    buffer_t* buffer = buffer_create_uint32(capacity);


    // fill buffer
    fill(buffer);

    buffer_print_uint32((buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    })));


    const int32 index = 9;
    struct timer* t = timer_create_perf_timer();

    // remove an element, using free buffer to store all free buffer elems
    print("%s-- REMOVE ELEMENTS AT INDEX:%d%s\n\n", gc, index, CLEAR);
    perf_timer_start(t);
    for(uint32 i = 0; i < repeat; i++) {
        /* cheat: */
        // set free list as if the buffer is full
        for(uint32 i = 0; i < buffer->free_list_capacity; i++)
            buffer->free_list[i] = MAX_UINT64;
        // set tail as if the buffer is full
        buffer->tail = capacity - 1;

        for(uint32 j = 0; j < buffer->capacity; j++) {
            remove_from(buffer, j);
        }
    }
    perf_timer_stop_with_count(repeat * buffer->capacity, t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat, .buf_capacity = buffer->capacity,
        .flags = PERF_T_COMMON | PERF_T_INLINE
    }));
    perf_timer_reset(t);


    // insert a value in the buffer at specific index, using the free buffer 'find' a free element
    print("%s-- INSERT ELEMENTS AT INDEX:%d%s\n\n", gc, index, CLEAR);
    perf_timer_start(t);
    for(uint32 i = 0; i < repeat; i++) {
        /* cheat: */
        // set free list as if the buffer is empty
        for(uint32 i = 0; i < buffer->free_list_capacity; i++)
            buffer->free_list[i] = 0;
        // set tail as if the buffer is empty
        buffer->tail = -1;

        for(uint32 j = 0; j < buffer->capacity; j++) {
            const uint32 v = (((i + 1) / 2) * i) / 2;
            // const uint32 v = division_heavy_hash(i, i);
            insert_at(v, buffer, j);
        }
    }
    perf_timer_stop_with_count(repeat * buffer->capacity, t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat, .buf_capacity = buffer->capacity,
        .flags = PERF_T_COMMON | PERF_T_INLINE | PERF_T_IN_CONTINUATION
    }));
    perf_timer_reset(t);

    // buffer_print_uint32((buffer_print_config((buffer_print_p){
    //     .buffer = buffer, .print_num_elems = buffer->capacity
    // })));

    memman_terminate();
}


/*******************************************************************************
 * test: measure performance overhead of pushing to buffer
 * inside the loop compare to push via function overhead (using the buffer id)
 ******************************************************************************/
void
push_abstructed(const uint16 id, const uint32 index, uint32 val)
{
    // void* address = mem_cache_alloc_slab(id); // Get memory directly from mem manager
    // buffer_t* buffer = (buffer_t*) address;
    // uint32* data = buffer->data;
    // data[index] = val;
    // buffer->tail++;
    // buffer->count++;
}

void
perf_test_insert_abstruct_call(void)
{
    print("%s-- TEST: --\n", ry, CLEAR);
    print("%s measure performance overhead\n of pushing to buffer\n"
          " from inside the loop\n compare to push via function\n using  the buffer id%s\n\n",
          ry, CLEAR);

    memman_init();


    buffer_init_table();

    timer_init_table();

    // allocate new buffer
    const uint32 capacity = 16;
    buffer_t* buffer = buffer_create_uint32(capacity);


    // fill buffer
    const uint32 len = 64;
    const uint32 repeat = 1000000;
    print("%s-- buffer push len: %u values%s\n", ry, len, CLEAR);
    print("%s-- test repeat: %u times%s\n\n", ry, repeat, CLEAR);


    print("%s-- measurment outside the loops%s\n", GREEN, CLEAR);
    // measure inside the loop
    {
        struct timer* t = timer_create_perf_timer();
        print("%s-- test: with function call overhead%s\n", BLUE, CLEAR);
        perf_timer_start(t);
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < len; i++) {
                push_abstructed(buffer->id, i, 1);
            }
        }
        perf_timer_stop_with_count(repeat * len, t);
        timer_print(t, repeat, len, sizeof(buffer->data[0]));


        buffer->tail = -1;
        buffer->count = 0;

        print("%s-- test: push inside the loop%s\n", BLUE, CLEAR);
        perf_timer_start(t);
        uint32* data = buffer->data;
        for(uint32 k = 0; k < repeat; k++) {
            for(uint32 i = 0; i < len; i++) {
                data[i] = 0;
                buffer->tail++;
                buffer->count++;
            }
        }
        perf_timer_stop_with_count(repeat * len, t);
        timer_print(t, repeat, len, sizeof(buffer->data[0]));
    }

    print("%s-- TEST: --\n", ry, CLEAR);

    /*
     * Results

     */

    memman_terminate();
}


uint64_t
division_heavy_hash(uint32 seed, uint32 i)
{
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
    if(value == 0) value = seed + i + 1;

    return value;
}


void
perf_test_buffer_find_tail(void)
{
    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s test_buffer_find_tail%s\n\n", CYAN, CLEAR);

    const uint32_t num_pages = 15;
    memman_init();

    buffer_init_table();

    timer_init_table();

    const uint32 repeat = 1000000;
    const uint32 type_size = sizeof(uint32);

    struct timer* t = timer_create_perf_timer();

    // capacity 64
    {
        const uint32 capacity = 64;
        buffer_t* buffer = buffer_create_gentype(capacity, type_size);

        perf_timer_start(t);
        for(uint32 r = 0; r < repeat; r++) {
            /* cheat: */
            // set free list as if the buffer is full
            for(uint32 i = 0; i < buffer->free_list_capacity; i++)
                buffer->free_list[i] = 0;
            // set tail as if the buffer is full
            buffer->tail = capacity - 1;

            for(uint32 i = 0; i < buffer->capacity; i++) {
                buffer_free_elem(buffer, i);
            }
        }
        perf_timer_stop_with_count(repeat * capacity, t);
        timer_print(t, repeat, capacity, sizeof(uint32));
    }
    // capacity 128
    {
        const uint32 capacity = 128;
        buffer_t* buffer = buffer_create_gentype(capacity, type_size);

        perf_timer_start(t);
        for(uint32 r = 0; r < repeat; r++) {
            /* cheat: */
            // set free list as if the buffer is full
            for(uint32 i = 0; i < buffer->free_list_capacity; i++)
                buffer->free_list[i] = 0;
            // set tail as if the buffer is full
            buffer->tail = capacity - 1;

            for(uint32 i = 0; i < buffer->capacity; i++) {
                buffer_free_elem(buffer, i);
            }
        }
        perf_timer_stop_with_count(repeat * capacity, t);
        timer_print(t, repeat, capacity, sizeof(uint32));
    }
    // capacity 256
    {
        const uint32 capacity = 256;
        buffer_t* buffer = buffer_create_gentype(capacity, type_size);

        perf_timer_start(t);
        for(uint32 r = 0; r < repeat; r++) {
            /* cheat: */
            // set free list as if the buffer is full
            for(uint32 i = 0; i < buffer->free_list_capacity; i++)
                buffer->free_list[i] = 0;
            // set tail as if the buffer is full
            buffer->tail = capacity - 1;

            for(uint32 i = 0; i < buffer->capacity; i++) {
                buffer_free_elem(buffer, i);
            }
        }
        perf_timer_stop_with_count(repeat * capacity, t);
        timer_print(t, repeat, capacity, sizeof(uint32));
    }
    // capacity 512
    {
        const uint32 capacity = 512;
        buffer_t* buffer = buffer_create_gentype(capacity, type_size);

        perf_timer_start(t);
        for(uint32 r = 0; r < repeat; r++) {
            /* cheat: */
            // set free list as if the buffer is full
            for(uint32 i = 0; i < buffer->free_list_capacity; i++)
                buffer->free_list[i] = 0;
            // set tail as if the buffer is full
            buffer->tail = capacity - 1;

            for(uint32 i = 0; i < buffer->capacity; i++) {
                buffer_free_elem(buffer, i);
            }
        }
        perf_timer_stop_with_count(repeat * capacity, t);
        timer_print(t, repeat, capacity, sizeof(uint32));
    }


    memman_terminate();
}


/***************************************************************************************************
 * buffers inside of buffers !
 **************************************************************************************************/
/* MOCK SAVE */
// void
// perf_test_buffer_with_children(void)
// {
//
//     print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
//     print("%s test_buffer_with_children%s\n\n", CYAN, CLEAR);
//     print("%s SAME BUFFER CAPACITIES FOR CHILDREN%s\n\n", CYAN, CLEAR);
//
//     memman_init();
//
//     buffer_init_table();
//     timer_init_table();
//
//
//     struct timer* t = timer_create_perf_timer();
//
//     struct node {
//         buffer_t* children;
//         void* data;
//         void* freelist;
//         void* nameid;
//     };
//
//     // const uint32 repeat = 500000;
//     const uint32 repeat = 1;
//     char* mem_start = 0, *mem_end = 0;
//
//     // capacity = 4
//     {
//         // const uint32 capacity = 4;
//         // const uint32 type_size = sizeof(struct node);
//         // buffer_t* buffer = buffer_create_gentype(capacity, type_size);
//         //
//         // // fill buffer
//         // for(uint32 i = 0; i < buffer->capacity; i++) {
//         //
//         //     buffer_set_for_insert(buffer, i);
//         //     struct node* data = buffer->data;
//         //     data[i].nameid = i+1;
//         //
//         //     data[i].children = buffer_create_gentype(capacity, type_size);
//         //     // print("add child at:%x\n", REDUCE_ADDR(data[i].children->data, F6));
//         //     for(uint32 j = 0; j < data[i].children->capacity; j++) {
//         //         buffer_set_for_insert(data[i].children, j);
//         //         struct node* cdata = data[i].children->data;
//         //         cdata[j].nameid = j+1;
//         //         cdata[j].children = NULL;;
//         //
//         //     }
//         // }
//         //
//         //
//         // const uint32 num_buffer_elems = buffer->capacity +
//         // (buffer->capacity * ((struct node*)buffer->data)[0].children->capacity);
//         //
//         // print("\n\n%s#### TEST:buffer capacities:%u%s\n", GREEN, capacity, CLEAR);
//         //
//         // // perf-access
//         // perf_timer_start(t);
//         // for(uint32 r = 0; r < repeat; r++) {
//         //     for(uint32 i = 0; i < buffer->capacity; i++) {
//         //
//         //         struct node* data = (struct node*)buffer->data;
//         //         data[i].nameid++;
//         //
//         //         for(uint32 j = 0; j < data[i].children->capacity; j++) {
//         //             struct node* cdata = data[i].children->data;
//         //             cdata[i].nameid++;
//         //         }
//         //     }
//         // }
//         //
//         // // perf_timer_stop_with_count(repeat * buffer->capacity, t);
//         // perf_timer_stop(t);
//         // perf_timer_print(timer_print_config((timer_print_params){
//         //     .t = t, .repeat = repeat, .buf_capacity = 0,
//         //     .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT }));
//         //
//         // print("perf with total buffers accesses:%u\n", num_buffer_elems);
//         //
//         // perf_timer_print(timer_print_config((timer_print_params){
//         //     .t = t, .repeat = repeat, .buf_capacity = num_buffer_elems,
//         //     .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT|PERF_T_USE_BUF_CAPACITY }));
//         // perf_timer_reset(t);
//     }
//
//     // capacity = 16
//     {
//         // const uint32 capacity = 16;
//         // const uint32 type_size = sizeof(struct node);
//         // buffer_t* buffer = buffer_create_gentype(capacity, type_size);
//         // print("add buffer at:%x\n", REDUCE_ADDR(buffer->data, F6));
//         //
//         // // fill buffer
//         // for(uint32 i = 0; i < buffer->capacity; i++) {
//         //
//         //     buffer_set_for_insert(buffer, i);
//         //     struct node* data = buffer->data;
//         //     data[i].nameid = i+1;
//         //
//         //     data[i].children = buffer_create_gentype(capacity, type_size);
//         //     // print("add child at:%x\n", REDUCE_ADDR(data[i].children->data, F6));
//         //     for(uint32 j = 0; j < data[i].children->capacity; j++) {
//         //         buffer_set_for_insert(data[i].children, j);
//         //         struct node* cdata = data[i].children->data;
//         //         cdata[j].nameid = j+1;
//         //         cdata[j].children = NULL;;
//         //
//         //     }
//         // }
//         //
//         //
//         // const uint32 num_buffer_elems = buffer->capacity +
//         // (buffer->capacity * ((struct node*)buffer->data)[0].children->capacity);
//         //
//         // print("\n\n%s#### TEST:buffer capacities:%u%s\n", GREEN, capacity, CLEAR);
//         //
//         // // perf-access
//         // perf_timer_start(t);
//         // for(uint32 r = 0; r < repeat; r++) {
//         //     for(uint32 i = 0; i < buffer->capacity; i++) {
//         //
//         //         struct node* data = (struct node*)buffer->data;
//         //         data[i].nameid++;
//         //
//         //         for(uint32 j = 0; j < data[i].children->capacity; j++) {
//         //             struct node* cdata = data[i].children->data;
//         //             cdata[i].nameid++;
//         //         }
//         //     }
//         // }
//         //
//         // // perf_timer_stop_with_count(repeat * buffer->capacity, t);
//         // perf_timer_stop(t);
//         // perf_timer_print(timer_print_config((timer_print_params){
//         //     .t = t, .repeat = repeat, .buf_capacity = 0,
//         //     .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT }));
//         //
//         // print("perf with total buffers accesses:%u\n", num_buffer_elems);
//         //
//         // perf_timer_print(timer_print_config((timer_print_params){
//         //     .t = t, .repeat = repeat, .buf_capacity = num_buffer_elems,
//         //     .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT|PERF_T_USE_BUF_CAPACITY }));
//         // perf_timer_reset(t);
//     }
//
//     // capacity = 64
//     {
//         // const uint32 capacity = 64;
//         // const uint32 type_size = sizeof(struct node);
//         // buffer_t* buffer = buffer_create_gentype(capacity, type_size);
//         //
//         // // fill buffer
//         // for(uint32 i = 0; i < buffer->capacity; i++) {
//         //
//         //     buffer_set_for_insert(buffer, i);
//         //     struct node* data = buffer->data;
//         //     data[i].nameid = i+1;
//         //
//         //     data[i].children = buffer_create_gentype(capacity, type_size);
//         //     // print("add child at:%x\n", REDUCE_ADDR(data[i].children->data, F6));
//         //     for(uint32 j = 0; j < data[i].children->capacity; j++) {
//         //         buffer_set_for_insert(data[i].children, j);
//         //         struct node* cdata = data[i].children->data;
//         //         cdata[j].nameid = j+1;
//         //         cdata[j].children = NULL;;
//         //
//         //     }
//         // }
//         //
//         //
//         // const uint32 num_buffer_elems = buffer->capacity +
//         // (buffer->capacity * ((struct node*)buffer->data)[0].children->capacity);
//         //
//         // print("\n\n%s#### TEST:buffer capacities:%u%s\n", GREEN, capacity, CLEAR);
//         //
//         // // perf-access
//         // perf_timer_start(t);
//         // for(uint32 r = 0; r < repeat; r++) {
//         //     for(uint32 i = 0; i < buffer->capacity; i++) {
//         //
//         //         struct node* data = (struct node*)buffer->data;
//         //         data[i].nameid++;
//         //
//         //         for(uint32 j = 0; j < data[i].children->capacity; j++) {
//         //             struct node* cdata = data[i].children->data;
//         //             cdata[i].nameid++;
//         //         }
//         //     }
//         // }
//         //
//         // // perf_timer_stop_with_count(repeat * buffer->capacity, t);
//         // perf_timer_stop(t);
//         // perf_timer_print(timer_print_config((timer_print_params){
//         //     .t = t, .repeat = repeat, .buf_capacity = 0,
//         //     .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT }));
//         //
//         // print("perf with total buffers accesses:%u\n", num_buffer_elems);
//         //
//         // perf_timer_print(timer_print_config((timer_print_params){
//         //     .t = t, .repeat = repeat, .buf_capacity = num_buffer_elems,
//         //     .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT|PERF_T_USE_BUF_CAPACITY }));
//         // perf_timer_reset(t);
//     }
//
//     // capacity = 128
//     {
//         /*******************************************************************************************
//          * create and fill buffer objects
//          ******************************************************************************************/
//         const uint32 capacity = 2;
//         // const uint32 capacity = 10;
//         // const uint32 capacity = 64;
//         const uint32 children_capacity = 2;
//         const uint32 type_size = sizeof(struct node);
//         buffer_t* buffer = buffer_create_gentype(capacity, type_size);
//         print("add buffer at:%x\n", REDUCE_ADDR(buffer->data, F5));
//
//         mem_start = (char*)buffer->data;
//
//         const char* filename = "buffer/fs_debug_mem_blocks.txt";
//         struct fs_t* fs_mem_blocks = fs_create(filename);
//
//         // fill buffer
//         for(uint32 i = 0; i < buffer->capacity; i++) {
//
//             char* start = flist_peek_next_free_page(0);
//
//             struct node* data = buffer->data;
//             // memcpy(&data[i].children, "AAEEOOYY", sizeof(char*));
//             // data[i].data     = data[i].data;
//             // memcpy(&data[i].freelist, "AAEEOOYY", sizeof(char*));
//             memcpy(&data[i].nameid, "PARENT!", sizeof(char*));
//
//             data[i].children = buffer_create_gentype(children_capacity, type_size);
//
//             for(uint32 j = 0; j < data[i].children->capacity; j++) {
//                 buffer_set_for_insert(data[i].children, j);
//                 struct node* cdata = data[i].children->data;
//                 // memcpy(&cdata[j].children, "ARR[]", sizeof(char*));
//                 // cdata[j].data     = 0x0;
//                 // memcpy(&cdata[j].freelist, "AAEEOOYY", sizeof(char*));
//                 memcpy(&cdata[j].nameid, "CHILDREN", sizeof(char*));
//
//                 mem_end = (char*)cdata + sizeof(struct node);
//             }
//
//             const uint64 size = capacity*sizeof(struct node);
//             fs_append_mem(fs_mem_blocks, start, size, "msg");
//         }
//
//         print("\n\n%s#### TEST:buffer capacities:%u%s\n", GREEN, capacity, CLEAR);
//         fs_fprint_all_allocated_mem("buffer/debug_all_memory.txt");
//
//
//         const uint32 num_buffer_elems = buffer->capacity +
//             (buffer->capacity * ((struct node*)buffer->data)[0].children->capacity);
//
//         /*******************************************************************************************
//          * performance: access and set buffer objects
//          ******************************************************************************************/
//
//         // // perf-access
//         // perf_timer_start(t);
//         // for(uint32 r = 0; r < repeat; r++) {
//         //     for(uint32 i = 0; i < buffer->capacity; i++) {
//         //
//         //         // buffer_set_for_insert(buffer, i);
//         //         struct node* data = buffer->data;
//         //         // memcpy(&data[i].children, "AAEEOOYY", sizeof(char*));
//         //         // data[i].data     = data[i].data;
//         //         memcpy(&data[i].freelist, "AAEEOOYY", sizeof(char*));
//         //         memcpy(&data[i].nameid, "PARENT!", sizeof(char*));
//         //
//         //         for(uint32 j = 0; j < data[i].children->capacity; j++) {
//         //             // buffer_set_for_insert(data[i].children, j);
//         //             struct node* cdata = data[i].children->data;
//         //             memcpy(&cdata[j].children, "ARR[]", sizeof(char*));
//         //             cdata[j].data     = 0x0;
//         //             memcpy(&cdata[j].freelist, "AAEEOOYY", sizeof(char*));
//         //             memcpy(&cdata[j].nameid, "CHILDREN", sizeof(char*));
//         //         }
//         //     }
//         // }
//
//         perf_timer_stop(t);
//         perf_timer_print(timer_print_config((timer_print_params){
//             .t = t, .repeat = repeat, .buf_capacity = 0,
//             .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT }));
//         perf_timer_reset(t);
//
//         print("perf with total buffers accesses:%u\n", num_buffer_elems);
//
//
//         /*******************************************************************************************
//          * destroy buffer objects
//          ******************************************************************************************/
//         for(uint32 i = 0; i < buffer->capacity; i++) {
//
//             struct node* data = (struct node*)buffer->data;
//             buffer_t* child = data[i].children;
//             buffer_destroy(child);
//
//         }
//         buffer_destroy(buffer);
//
//
//         /*******************************************************************************************
//          * repeat: create and fill buffer objects
//          ******************************************************************************************/
//         buffer = buffer_create_gentype(capacity, type_size);
//         print("add buffer at:%x\n", REDUCE_ADDR(buffer->data, F5));
//
//         // fill buffer
//         for(uint32 i = 0; i < buffer->capacity; i++) {
//
//             char* start = flist_peek_next_free_page(0);
//
//             buffer_set_for_insert(buffer, i);
//             struct node* data = buffer->data;
//             memcpy(&data[i].children, "AAEEOOYY", sizeof(char*));
//             memcpy(&data[i].nameid, "PARENT!", sizeof(char*));
//
//             data[i].children = buffer_create_gentype(children_capacity, type_size);
//
//             for(uint32 j = 0; j < data[i].children->capacity; j++) {
//                 buffer_set_for_insert(data[i].children, j);
//                 struct node* cdata = data[i].children->data;
//                 // memcpy(&cdata[j].children, "ARR[]", sizeof(char*));
//                 // cdata[j].data     = 0x0;
//                 memcpy(&cdata[j].nameid, "CHILDREN", sizeof(char*));
//
//                 mem_end = (char*)cdata + sizeof(struct node);
//             }
//
//             const uint64 size = capacity*sizeof(struct node);
//             fs_append_mem(fs_mem_blocks, start, size, "msg");
//         }
//
//         print("\n\n%s#### TEST:buffer capacities:%u%s\n", GREEN, capacity, CLEAR);
//         fs_fprint_all_allocated_mem("buffer/debug_all_memory.txt");
//
//         /*******************************************************************************************
//          * performance: access and set buffer objects
//          ******************************************************************************************/
//
//         // // perf-access
//         // perf_timer_start(t);
//         // for(uint32 r = 0; r < repeat; r++) {
//         //     for(uint32 i = 0; i < buffer->capacity; i++) {
//         //
//         //         struct node* data = (struct node*)buffer->data;
//         //         data[i].nameid++;
//         //
//         //         for(uint32 j = 0; j < data[i].children->capacity; j++) {
//         //             struct node* cdata = data[i].children->data;
//         //             cdata[i].nameid++;
//         //         }
//         //     }
//         // }
//         //
//         // perf_timer_stop(t);
//         // perf_timer_print(timer_print_config((timer_print_params){
//         //     .t = t, .repeat = repeat, .buf_capacity = 0,
//         //     .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT }));
//         // perf_timer_reset(t);
//         //
//         // print("perf with total buffers accesses:%u\n", num_buffer_elems);
//     }
//
//
//     TEMP_PRINT_BUFFER_TABLE_COUNT();
//
//     const char* fname = "/exit_status_ok_debug.txt";
//     struct fs_t* fs = fs_create(fname);
//     fs_fprint_all_allocated_mem(fname);
//     fs_append(fs, "EXIT WITH STATUS OK\n");
//
//     debug_mem_corrupted();
//
//     memman_terminate();
// }

void // test buffer with children and check memory for correctness. Add remove many buffers repeatedly.
perf_test_buffer_add_remove_with_children(void)
{
#define DISABLE_FPRINT 1
    /* If measuring performance, with a high count of repeats,
    * dissable file print to avoid writing too much data to the file */
    // const uint32 repeat = 100;
    const uint32 repeat = 1;
    assert(repeat < 50 && DISABLE_FPRINT);

#if DISABLE_FPRINT
#define custom_fs_append(fs_debug_mem, start, total_size, message) \
fs_append_mem(fs_debug_mem, start, total_size, message)
#else
#define custom_fs_append(fs_debug_mem, start, total_size, message)
#endif

    const char* description =
            "1.\n"
            "  Create parent objects of type buffer_t.\n"
            "  Create children objects of type buffer_t\n"
            "  to the parent buffer.\n"
            "2.\n"
            "  Destroy children buffers.\n"
            "  Destroy parent buffers\n"
            "3.\n"
            "  repeat proccess in a loop\n"
            "  print memory to file.";

    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s perf_test_buffer_add_remove_with_children%s\n\n", CYAN, CLEAR);
    print("%s description%s\n%s\n\n", ry, CLEAR, description);

    memman_init();

    buffer_init_table();
    timer_init_table();


    struct timer* t_create = timer_create_perf_timer();
    struct timer* t_destroy = timer_create_perf_timer();

    struct node {
        buffer_t* children;
        void* data;
        void* freelist;
        void* nameid;
    };

    const uint32 type_size = sizeof(struct node);
    const uint32 pcapac = 128;
    const uint32 ccapac = 64;


    /* DEBUG-FPRINT */
    const char* filename = "buffer/tests/"
                           "perf_test_buffer_add_remove_with_children.txt";
    struct fs_t* fs_debug_mem = fs_create(filename);
    fs_append(fs_debug_mem,
              "__________________________________________\n"
              "IMPORTANT!\n"
              "Memory is alligned at buffer->freelist\n"
              "OR buffer->data address\n"
              "__________________________________________\n"
    );
    fs_append(fs_debug_mem, description);



    uint32 id = 0;
    char* pfree = NULL;
    char* cfree = NULL;

    for(uint32 r = 0; r < repeat; r++) {

        fs_append(fs_debug_mem,
                  "\n\n**********************************************"
                  "**************************************************"
                  "**************************************************\n"
                  "test:%u\n parents capacity:%u\n childrens capacity:%u*%u=%u\n",
                  id++, pcapac, pcapac, ccapac, pcapac * ccapac
        );

        /*******************************************************************************************
         * create and fill buffer objects
         ******************************************************************************************/

        perf_timer_start_accum(t_create);

        buffer_t* buffer = buffer_create_gentype(pcapac, type_size);
        if((char*)buffer->data > pfree) pfree = (char*)buffer->data ;

        // fill buffer
        for(uint32 i = 0; i < buffer->capacity; i++) {
            struct node* data = buffer->data;

            char pname[16] = "PAR:";
            str_concat_int32_to_str(pname, 16, buffer->id);
            memcpy(&data[i].nameid, pname, sizeof(char*));

            data[i].children = buffer_create_gentype(ccapac, type_size);

            for(uint32 j = 0; j < data[i].children->capacity; j++) {

                buffer_set_for_insert(data[i].children, j);
                struct node* cdata = data[i].children->data;
                char cname[16] = "CHD:";
                str_concat_int32_to_str(cname, 16, data[i].children->id);
                memcpy(&cdata[j].nameid, cname, sizeof(char*));

                if((char*)&cdata[j] > cfree)
                    cfree = (char*)&cdata[j];

            }

        }
        perf_timer_stop_accum(t_create);


        char* start = buffer->data < buffer->free_list ? buffer->data : buffer->free_list;
        const char* end = (pfree > cfree) ? pfree : cfree;
        const uint64 total_size = ((uint64)end - (uint64)start) + PAGE_SIZE;
        custom_fs_append(fs_debug_mem, start, total_size, "CREATE BUFFERS");


        /*******************************************************************************************
         * destroy buffer objects
         ******************************************************************************************/
        perf_timer_start_accum(t_destroy);

        for(uint32 i = 0; i < buffer->capacity; i++) {
            struct node* data = (struct node*) buffer->data;
            buffer_t* child = data[i].children;
            buffer_destroy(child);
        }
        buffer_destroy(buffer);

        perf_timer_stop_accum(t_destroy);

        custom_fs_append(fs_debug_mem, start, total_size, "DESTROY BUFFERS");

    }

    perf_timer_print(timer_print_config((timer_print_params){
        .t = t_create, .repeat = repeat, .buf_capacity = 0, .flags = PERF_T_ALL
    }));
    perf_timer_reset(t_create);

    perf_timer_print(timer_print_config((timer_print_params){
        .t = t_destroy, .repeat = repeat, .buf_capacity = 0, .flags = PERF_T_ALL
    }));
    perf_timer_reset(t_destroy);


    TEMP_PRINT_BUFFER_TABLE_COUNT();

    const char* fname = "/exit_status_ok_debug.txt";
    struct fs_t* fs = fs_create(fname);
    fs_fprint_all_allocated_mem(fname);
    fs_append(fs, "EXIT WITH STATUS OK\n");

    debug_mem_corrupted();

    memman_terminate();

#undef custom_fs_append
}

void // test buffer with children, and check memory for correctness. Add remove, mixed capacities.
perf_test_buffer_add_with_children_mixed_capacities(void)
{

#define DISABLE_FPRINT 1
    /* If measuring performance, with a high count of repeats,
     * dissable file print to avoid writing too much data to the file */
    // const uint32 repeat = 100;
    const uint32 repeat = 10;
    assert(repeat < 50 && DISABLE_FPRINT);

#if DISABLE_FPRINT
#define custom_fs_append(fs_debug_mem, start, tsize, message) \
fs_append_mem(fs_debug_mem, start, tsize, message)
#else
#define custom_fs_append(fs_debug_mem, start, tsize, message)
#endif


    const char* description =
            "1.\n"
            "  Create parent objects of type buffer_t.\n"
            "  Create children objects of type buffer_t\n"
            "  to the parent buffer.\n"
            "2.\n"
            "  Destroy children buffers.\n"
            "  Destroy parent buffers\n"
            "3.\n"
            "  repeat proccess, each time changing the\n"
            "  capacity of the newly created buffers\n\n";

    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s perf_test_buffer_add_with_children_mixed_capacities%s\n\n", CYAN, CLEAR);
    print("%s description%s\n%s\n\n", ry, CLEAR, description);

    memman_init();

    buffer_init_table();
    timer_init_table();


    struct timer* t_create = timer_create_perf_timer();

    struct node {
        buffer_t* children;
        void* data;
        void* freelist;
        void* nameid;
    };


    const uint32 type_size = sizeof(struct node);
    const uint32 parent_capacity[]   = {16,16,16,16,16};
    const uint32 children_capacity[] = {2, 2, 2, 2, 2};
    // const uint32 parent_capacity[]   = {2, 2, 2, 2, 8, 8, 8, 8};
    // const uint32 children_capacity[] = {4, 4, 4, 4, 1, 1, 1, 1};
    // const uint32 parent_capacity[]   = {3, 8, 14, 3, 6, 7, 5};
    // const uint32 children_capacity[] = {5, 3, 8, 14, 3, 6, 7};
    const uint32 buffer_tests_num = ARRAY_LENGTH(parent_capacity);


    /* DEBUG-FPRINT */
    const char* filename = "buffer/tests/"
                           "perf_test_buffer_add_with_children_mixed_capacities.txt";
    struct fs_t* fs_debug_mem = fs_create(filename);
    fs_append(fs_debug_mem,
              "__________________________________________\n"
              "IMPORTANT!\n"
              "Memory is alligned at buffer->freelist\n"
              "OR buffer->data address\n"
              "__________________________________________\n"
    );
    fs_append(fs_debug_mem, description);



    uint32 id = 0;
    char* pfree = NULL;
    char* cfree = NULL;

    for(uint32 testcnt = 0; testcnt < buffer_tests_num; testcnt++) {

        const uint32 pcapac = parent_capacity[testcnt];
        const uint32 ccapac = children_capacity[testcnt];

        fs_append(fs_debug_mem,
                  "\n\n**********************************************"
                  "**************************************************"
                  "**************************************************\n"
                  "test:%u\n parents capacity:%u\n childrens capacity:%u*%u=%u\n",
                  id++, pcapac, pcapac, ccapac, pcapac * ccapac
        );

        /*******************************************************************************************
         * create and fill buffer objects
         ******************************************************************************************/

        perf_timer_start_accum(t_create);

        buffer_t* buffer = buffer_create_gentype(pcapac, type_size);
        if((char*)buffer->data > pfree) pfree = (char*)buffer->data ;

        // fill buffer
        for(uint32 i = 0; i < buffer->capacity; i++) {
            struct node* data = buffer->data;

            char pname[16] = "PAR:";
            str_concat_int32_to_str(pname, 16, buffer->id);
            memcpy(&data[i].nameid, pname, sizeof(char*));

            data[i].children = buffer_create_gentype(ccapac, type_size);

            for(uint32 j = 0; j < data[i].children->capacity; j++) {

                buffer_set_for_insert(data[i].children, j);
                struct node* cdata = data[i].children->data;
                // const char* childname = memcpy(&cdata[j].nameid, "CHILDREN", sizeof(char*));
                char cname[16] = "CHD:";
                str_concat_int32_to_str(cname, 16, data[i].children->id);
                memcpy(&cdata[j].nameid, cname, sizeof(char*));

                if((char*)&cdata[j] > cfree)
                    cfree = (char*)&cdata[j];

            }

        }
        perf_timer_stop_accum(t_create);


        char* start = buffer->data < buffer->free_list ? buffer->data : buffer->free_list;
        const char* end = (pfree > cfree) ? pfree : cfree;
        const uint64 total_size = ((uint64)end - (uint64)start) + PAGE_SIZE;
        print("1. total_size:%u\n", total_size);
        custom_fs_append(fs_debug_mem, start, total_size, "CREATE BUFFERS");

    }


    TEMP_PRINT_BUFFER_TABLE_COUNT();

    const char* fname = "/exit_status_ok_debug.txt";
    struct fs_t* fs = fs_create(fname);
    fs_fprint_all_allocated_mem(fname);
    fs_append(fs, "EXIT WITH STATUS OK\n");

    debug_mem_corrupted();

    memman_terminate();

#undef custom_fs_append
}


void // test buffer with children, and check memory for correctness. Add remove, mixed capacities.
perf_test_buffer_add_remove_with_children_mixed_capacities(void)
{
#define DISABLE_FPRINT 1
    /* If measuring performance, with a high count of repeats,
    * dissable file print to avoid writing too much data to the file */
    // const uint32 repeat = 100;
    const uint32 repeat = 10;
    assert(repeat < 50 && DISABLE_FPRINT);

#if DISABLE_FPRINT
#define custom_fs_append(fs_debug_mem, start, tsize, message) \
fs_append_mem(fs_debug_mem, start, tsize, message)
#else
#define custom_fs_append(fs_debug_mem, start, tsize, message)
#endif
    const char* description =
            "1.\n"
            "  Create parent objects of type buffer_t.\n"
            "  Create children objects of type buffer_t\n"
            "  to the parent buffer.\n"
            "2.\n"
            "  Destroy children buffers.\n"
            "  Destroy parent buffers\n"
            "3.\n"
            "  repeat proccess, each time changing the\n"
            "  capacity of the newly created buffers\n\n";

    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s perf_test_buffer_add_remove_with_children_mixed_capacities%s\n\n", CYAN, CLEAR);
    print("%s description%s\n%s\n\n", ry, CLEAR, description);

    memman_init();

    buffer_init_table();
    timer_init_table();


    struct timer* t_create = timer_create_perf_timer();
    struct timer* t_destroy = timer_create_perf_timer();

    struct node {
        buffer_t* children;
        void* data;
        void* freelist;
        void* nameid;
    };


    const uint32 type_size = sizeof(struct node);
    // const uint32 parent_capacity[buffer_tests_num]   = {2, 14, 8,  2};
    // const uint32 children_capacity[buffer_tests_num] = {18, 2, 4, 18};
    const uint32 parent_capacity[]   = {3, 8, 14, 3, 6, 7, 5};
    const uint32 children_capacity[] = {5, 3, 8, 14, 3, 6, 7};
    const uint32 buffer_tests_num = ARRAY_LENGTH(parent_capacity);



    /* DEBUG-FPRINT */
    const char* filename = "buffer/tests/"
                           "perf_test_buffer_add_remove_with_children_mixed_capacities.txt";
    struct fs_t* fs_debug_mem = fs_create(filename);
    fs_append(fs_debug_mem,
              "__________________________________________\n"
              "IMPORTANT!\n"
              "Memory is alligned at buffer->freelist\n"
              "OR buffer->data address\n"
              "__________________________________________\n"
    );
    fs_append(fs_debug_mem, description);



    uint32 id = 0;
    char* pfree = NULL;
    char* cfree = NULL;

    for(uint32 testcnt = 0; testcnt < buffer_tests_num; testcnt++) {

        const uint32 pcapac = parent_capacity[testcnt];
        const uint32 ccapac = children_capacity[testcnt];

        fs_append(fs_debug_mem,
                  "\n\n**********************************************"
                  "**************************************************"
                  "**************************************************\n"
                  "test:%u\n parents capacity:%u\n childrens capacity:%u*%u=%u\n",
                  id++, pcapac, pcapac, ccapac, pcapac * ccapac
        );

        /*******************************************************************************************
         * create and fill buffer objects
         ******************************************************************************************/

        perf_timer_start_accum(t_create);

        buffer_t* buffer = buffer_create_gentype(pcapac, type_size);
        if((char*)buffer->data > pfree) pfree = (char*)buffer->data ;

        // fill buffer
        for(uint32 i = 0; i < buffer->capacity; i++) {
            struct node* data = buffer->data;

            char pname[16] = "PAR:";
            str_concat_int32_to_str(pname, 16, buffer->id);
            memcpy(&data[i].nameid, pname, sizeof(char*));

            data[i].children = buffer_create_gentype(ccapac, type_size);

            for(uint32 j = 0; j < data[i].children->capacity; j++) {

                buffer_set_for_insert(data[i].children, j);
                struct node* cdata = data[i].children->data;
                // const char* childname = memcpy(&cdata[j].nameid, "CHILDREN", sizeof(char*));
                char cname[16] = "CHD:";
                str_concat_int32_to_str(cname, 16, data[i].children->id);
                memcpy(&cdata[j].nameid, cname, sizeof(char*));

                if((char*)&cdata[j] > cfree)
                    cfree = (char*)&cdata[j];

            }

        }
        perf_timer_stop_accum(t_create);


        char* start = buffer->data < buffer->free_list ? buffer->data : buffer->free_list;
        const char* end = (pfree > cfree) ? pfree : cfree;
        const uint64 total_size = ((uint64)end - (uint64)start) + PAGE_SIZE;
        print("1. total_size:%u\n", total_size);
        custom_fs_append(fs_debug_mem, start, total_size, "CREATE BUFFERS");


        /*******************************************************************************************
         * destroy buffer objects
         ******************************************************************************************/
        perf_timer_start_accum(t_destroy);

        for(uint32 i = 0; i < buffer->capacity; i++) {
            struct node* data = (struct node*) buffer->data;
            buffer_t* child = data[i].children;
            buffer_destroy(child);
        }
        buffer_destroy(buffer);

        perf_timer_stop_accum(t_destroy);

        print("2. total_size:%u\n", total_size);
        custom_fs_append(fs_debug_mem, start, total_size, "DESTROY BUFFERS");

        perf_timer_print(timer_print_config((timer_print_params){
            .t = t_destroy, .repeat = repeat, .buf_capacity = 0, .flags = PERF_T_ALL
        }));
        perf_timer_reset(t_destroy);

        perf_timer_print(timer_print_config((timer_print_params){
            .t = t_create, .repeat = repeat, .buf_capacity = 0, .flags = PERF_T_ALL
        }));
        perf_timer_reset(t_create);

    }


    TEMP_PRINT_BUFFER_TABLE_COUNT();

    const char* fname = "/exit_status_ok_debug.txt";
    struct fs_t* fs = fs_create(fname);
    fs_fprint_all_allocated_mem(fname);
    fs_append(fs, "EXIT WITH STATUS OK\n");

    debug_mem_corrupted();

    memman_terminate();

#undef custom_fs_append
#undef buffer_tests_num
}


void // test buffer with children. All buffers with different (slab) capacities
perf_test_buffer_with_children_in_dif_slabs(void)
{
    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s test_buffer_with_children%s\n", CYAN, CLEAR);
    print("%s DIFFERENT BUFFER CAPACITIES FOR CHILDREN%s\n\n", CYAN, CLEAR);

    const uint32_t num_pages = 100;
    memman_init();

    buffer_init_table();
    timer_init_table();


    struct timer* t = timer_create_perf_timer();

    struct node {
        uint32 nameid;
        buffer_t* children;
    };

    const uint32 repeat = 1000000;

    {
        const uint32 capacity = 4;
        uint32 dif_capacity = capacity;

        const uint32 type_size = sizeof(struct node);
        buffer_t* buffer = buffer_create_gentype(capacity, type_size);


        uint32 num_buffer_elems = buffer->capacity;


        print("\n\n%sparent buffer capacity:%u%s\n", GREEN, capacity, CLEAR);

        uint64 sum = 0;
        // fill buffer
        for(uint32 i = 0; i < buffer->capacity; i++) {
            buffer_set_for_insert(buffer, i);
            struct node* data = buffer->data;
            data[i].nameid = i + 1;

            dif_capacity *= 4;
            num_buffer_elems += dif_capacity;
            print("child capacity:%u\n", dif_capacity);

            data[i].children = buffer_create_gentype(dif_capacity, type_size);
            // print("add child at:%x\n", REDUCE_ADDR(data[i].children->data, F6));
            for(uint32 j = 0; j < data[i].children->capacity; j++) {
                buffer_set_for_insert(data[i].children, j);
                struct node* cdata = data[i].children->data;
                cdata[j].nameid = j + 1;
                cdata[j].children = NULL;;
            }
        }


        // perf-access
        perf_timer_start(t);
        for(uint32 r = 0; r < repeat; r++) {
            for(uint32 i = 0; i < buffer->capacity; i++) {
                struct node* data = (struct node*) buffer->data;
                data[i].nameid++;
                sum += data[i].nameid;

                for(uint32 j = 0; j < data[i].children->capacity; j++) {
                    struct node* cdata = data[i].children->data;
                    cdata[i].nameid++;
                    sum += cdata[j].nameid;
                }
            }
        }

        print("sum:%u\n", sum);


        // perf_timer_stop_with_count(repeat * buffer->capacity, t);
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = 0,
            .flags = PERF_T_ALL | PERF_T_USE_REPEAT
        }));
        // .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT }));

        print("perf with total buffers accesses:%u\n", num_buffer_elems);

        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = num_buffer_elems,
            .flags = PERF_T_ALL | PERF_T_USE_REPEAT | PERF_T_USE_BUF_CAPACITY
        }));
        perf_timer_reset(t);
    }
    //
    // {
    //     const uint32 capacity = 16;
    //     const uint32 type_size = sizeof(struct node);
    //     buffer_t* buffer = buffer_create_gentype(capacity, type_size);
    //     print("add buffer at:%x\n", REDUCE_ADDR(buffer->data, F6));
    //
    //     // fill buffer
    //     for(uint32 i = 0; i < buffer->capacity; i++) {
    //
    //         buffer_set_for_insert(buffer, i);
    //         struct node* data = buffer->data;
    //         data[i].nameid = i+1;
    //
    //         data[i].children = buffer_create_gentype(capacity, type_size);
    //         // print("add child at:%x\n", REDUCE_ADDR(data[i].children->data, F6));
    //         for(uint32 j = 0; j < data[i].children->capacity; j++) {
    //             buffer_set_for_insert(data[i].children, j);
    //             struct node* cdata = data[i].children->data;
    //             cdata[j].nameid = j+1;
    //             cdata[j].children = NULL;;
    //
    //         }
    //     }
    //
    //
    //     const uint32 num_buffer_elems = buffer->capacity +
    //     (buffer->capacity * ((struct node*)buffer->data)[0].children->capacity);
    //
    //     print("\n\n%s#### TEST:buffer capacities:%u%s\n", GREEN, capacity, CLEAR);
    //
    //     // perf-access
    //     perf_timer_start(t);
    //     for(uint32 r = 0; r < repeat; r++) {
    //         for(uint32 i = 0; i < buffer->capacity; i++) {
    //
    //             struct node* data = (struct node*)buffer->data;
    //             data[i].nameid++;
    //
    //             for(uint32 j = 0; j < data[i].children->capacity; j++) {
    //                 struct node* cdata = data[i].children->data;
    //                 cdata[i].nameid++;
    //             }
    //         }
    //     }
    //
    //     // perf_timer_stop_with_count(repeat * buffer->capacity, t);
    //     perf_timer_stop(t);
    //     perf_timer_print(timer_print_config((timer_print_params){
    //         .t = t, .repeat = repeat, .buf_capacity = 0,
    //         .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT }));
    //
    //     print("perf with total buffers accesses:%u\n", num_buffer_elems);
    //
    //     perf_timer_print(timer_print_config((timer_print_params){
    //         .t = t, .repeat = repeat, .buf_capacity = num_buffer_elems,
    //         .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT|PERF_T_USE_BUF_CAPACITY }));
    //     perf_timer_reset(t);
    // }
    //
    // {
    //     const uint32 capacity = 64;
    //     const uint32 type_size = sizeof(struct node);
    //     buffer_t* buffer = buffer_create_gentype(capacity, type_size);
    //
    //     // fill buffer
    //     for(uint32 i = 0; i < buffer->capacity; i++) {
    //
    //         buffer_set_for_insert(buffer, i);
    //         struct node* data = buffer->data;
    //         data[i].nameid = i+1;
    //
    //         data[i].children = buffer_create_gentype(capacity, type_size);
    //         // print("add child at:%x\n", REDUCE_ADDR(data[i].children->data, F6));
    //         for(uint32 j = 0; j < data[i].children->capacity; j++) {
    //             buffer_set_for_insert(data[i].children, j);
    //             struct node* cdata = data[i].children->data;
    //             cdata[j].nameid = j+1;
    //             cdata[j].children = NULL;;
    //
    //         }
    //     }
    //
    //
    //     const uint32 num_buffer_elems = buffer->capacity +
    //     (buffer->capacity * ((struct node*)buffer->data)[0].children->capacity);
    //
    //     print("\n\n%s#### TEST:buffer capacities:%u%s\n", GREEN, capacity, CLEAR);
    //
    //     // perf-access
    //     perf_timer_start(t);
    //     for(uint32 r = 0; r < repeat; r++) {
    //         for(uint32 i = 0; i < buffer->capacity; i++) {
    //
    //             struct node* data = (struct node*)buffer->data;
    //             data[i].nameid++;
    //
    //             for(uint32 j = 0; j < data[i].children->capacity; j++) {
    //                 struct node* cdata = data[i].children->data;
    //                 cdata[i].nameid++;
    //             }
    //         }
    //     }
    //
    //     // perf_timer_stop_with_count(repeat * buffer->capacity, t);
    //     perf_timer_stop(t);
    //     perf_timer_print(timer_print_config((timer_print_params){
    //         .t = t, .repeat = repeat, .buf_capacity = 0,
    //         .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT }));
    //
    //     print("perf with total buffers accesses:%u\n", num_buffer_elems);
    //
    //     perf_timer_print(timer_print_config((timer_print_params){
    //         .t = t, .repeat = repeat, .buf_capacity = num_buffer_elems,
    //         .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT|PERF_T_USE_BUF_CAPACITY }));
    //     perf_timer_reset(t);
    // }
    //
    // {
    //     const uint32 capacity = 128;
    //     const uint32 type_size = sizeof(struct node);
    //     buffer_t* buffer = buffer_create_gentype(capacity, type_size);
    //     print("add buffer at:%x\n", REDUCE_ADDR(buffer->data, F6));
    //
    //     // fill buffer
    //     for(uint32 i = 0; i < buffer->capacity; i++) {
    //
    //         buffer_set_for_insert(buffer, i);
    //         struct node* data = buffer->data;
    //         data[i].nameid = i+1;
    //
    //         data[i].children = buffer_create_gentype(capacity, type_size);
    //         // print("add child at:%x\n", REDUCE_ADDR(data[i].children->data, F6));
    //         for(uint32 j = 0; j < data[i].children->capacity; j++) {
    //             buffer_set_for_insert(data[i].children, j);
    //             struct node* cdata = data[i].children->data;
    //             cdata[j].nameid = j+1;
    //             cdata[j].children = NULL;;
    //
    //         }
    //     }
    //
    //
    //     const uint32 num_buffer_elems = buffer->capacity +
    //     (buffer->capacity * ((struct node*)buffer->data)[0].children->capacity);
    //
    //     print("\n\n%s#### TEST:buffer capacities:%u%s\n", GREEN, capacity, CLEAR);
    //
    //     // perf-access
    //     perf_timer_start(t);
    //     for(uint32 r = 0; r < repeat; r++) {
    //         for(uint32 i = 0; i < buffer->capacity; i++) {
    //
    //             struct node* data = (struct node*)buffer->data;
    //             data[i].nameid++;
    //
    //             for(uint32 j = 0; j < data[i].children->capacity; j++) {
    //                 struct node* cdata = data[i].children->data;
    //                 cdata[i].nameid++;
    //             }
    //         }
    //     }
    //
    //     // perf_timer_stop_with_count(repeat * buffer->capacity, t);
    //     perf_timer_stop(t);
    //     perf_timer_print(timer_print_config((timer_print_params){
    //         .t = t, .repeat = repeat, .buf_capacity = 0,
    //         .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT }));
    //
    //     print("perf with total buffers accesses:%u\n", num_buffer_elems);
    //
    //     perf_timer_print(timer_print_config((timer_print_params){
    //         .t = t, .repeat = repeat, .buf_capacity = num_buffer_elems,
    //         .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT|PERF_T_USE_BUF_CAPACITY }));
    //     perf_timer_reset(t);
    // }


    TEMP_PRINT_BUFFER_TABLE_COUNT();

    memman_terminate();
}
