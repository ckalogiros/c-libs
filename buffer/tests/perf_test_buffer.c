/*
 * Created by ckalo on 1/8/26.
 */

#include "perf_test_buffer.h"
#include "buffer_internals.h"
#include "buffer_log.h"
#include "logger.h"
#include "mem_man.h"
#include "precision_timer.h"
#include "precision_timer_logger.h"


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
uint64_t
division_heavy_hash(uint32 seed, uint32 i);


/*******************************************************************************
 * test: measure performance of:
 * remove buffer element
 * insert buffer element
 ******************************************************************************/
void
perf_test_buffer_create_generic_vs_uint32()
{
    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%stest performance of allocating and releasing\n"
          "uint32 buffer versus generic buffer.%s\n\n", CYAN, CLEAR);

    const uint32_t num_pages = 16;
    memory_manager_init(num_pages);

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

    memory_manager_terminate();
}

/*******************************************************************************
 * test: measure performance of:
 * remove buffer element
 * insert buffer element
 ******************************************************************************/
void
perf_test_buffer_remove_insert()
{
    const uint32_t num_pages = 10;
    memory_manager_init(num_pages);

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
        buffer->tail = capacity-1;

        for(uint32 j = 0; j < buffer->capacity; j++) {
            remove_from(buffer, j);
        }
    }
    perf_timer_stop_with_count(repeat * buffer->capacity, t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat, .buf_capacity = buffer->capacity,
        .flags = PERF_T_COMMON|PERF_T_INLINE }));
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
        .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_IN_CONTINUATION }));
    perf_timer_reset(t);

    // buffer_print_uint32((buffer_print_config((buffer_print_p){
    //     .buffer = buffer, .print_num_elems = buffer->capacity
    // })));

    memory_manager_terminate();
}


/*******************************************************************************
 * test: measure performance overhead of pushing to buffer
 * inside the loop compare to push via function overhead (using the buffer id)
 ******************************************************************************/
void
push_abstructed(const uint16 id, const uint32 index, uint32 val)
{
    void* address = Temp_get_address_by_id(id); // Get memory directly from mem manager
    buffer_t* buffer = (buffer_t*) address;
    uint32* data = buffer->data;
    data[index] = val;
    buffer->tail++;
    buffer->count++;
}

void
perf_test_insert_abstruct_call()
{
    print("%s-- TEST: --\n", ry, CLEAR);
    print("%s measure performance overhead\n of pushing to buffer\n"
          " from inside the loop\n compare to push via function\n using  the buffer id%s\n\n",
          ry, CLEAR);

    const uint32_t num_pages = 8;
    memory_manager_init(num_pages);


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

    memory_manager_terminate();
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
perf_test_buffer_find_tail()
{
    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s test_buffer_find_tail%s\n\n", CYAN, CLEAR);

    const uint32_t num_pages = 15;
    memory_manager_init(num_pages);

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
            buffer->tail = capacity-1;

            for(uint32 i = 0; i < buffer->capacity; i++) {
                buffer_free_elem(buffer, i);
            }
        }
        perf_timer_stop_with_count(repeat*capacity, t);
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
            buffer->tail = capacity-1;

            for(uint32 i = 0; i < buffer->capacity; i++) {
                buffer_free_elem(buffer, i);
            }
        }
        perf_timer_stop_with_count(repeat*capacity, t);
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
            buffer->tail = capacity-1;

            for(uint32 i = 0; i < buffer->capacity; i++) {
                buffer_free_elem(buffer, i);
            }
        }
        perf_timer_stop_with_count(repeat*capacity, t);
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
            buffer->tail = capacity-1;

            for(uint32 i = 0; i < buffer->capacity; i++) {
                buffer_free_elem(buffer, i);
            }
        }
        perf_timer_stop_with_count(repeat*capacity, t);
        timer_print(t, repeat, capacity, sizeof(uint32));
    }


    memory_manager_terminate();
}


/***************************************************************************************************
 * buffers inside of buffers !
 */
void // test buffer with children. All buffers with same (slab) capacity
perf_test_buffer_with_children()
{

    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s test_buffer_with_children%s\n\n", CYAN, CLEAR);
    print("%s SAME BUFFER CAPACITIES FOR CHILDREN%s\n\n", CYAN, CLEAR);

    const uint32_t num_pages = 1000000;
    memory_manager_init(num_pages);

    buffer_init_table();
    timer_init_table();


    struct timer* t = timer_create_perf_timer();

    struct node {
        uint32 nameid;
        buffer_t* children;
    };

    const uint32 repeat = 10000;

    {
        const uint32 capacity = 4;
        const uint32 type_size = sizeof(struct node);
        buffer_t* buffer = buffer_create_gentype(capacity, type_size);

        // fill buffer
        for(uint32 i = 0; i < buffer->capacity; i++) {

            buffer_set_for_insert(buffer, i);
            struct node* data = buffer->data;
            data[i].nameid = i+1;

            data[i].children = buffer_create_gentype(capacity, type_size);
            // print("add child at:%x\n", REDUCE_ADDR(data[i].children->data, F6));
            for(uint32 j = 0; j < data[i].children->capacity; j++) {
                buffer_set_for_insert(data[i].children, j);
                struct node* cdata = data[i].children->data;
                cdata[j].nameid = j+1;
                cdata[j].children = NULL;;

            }
        }


        const uint32 total_buffers_elements = buffer->capacity +
        (buffer->capacity * ((struct node*)buffer->data)[0].children->capacity);

        print("\n\n%s#### TEST:buffer capacities:%u%s\n", GREEN, capacity, CLEAR);

        // perf-access
        perf_timer_start(t);
        for(uint32 r = 0; r < repeat; r++) {
            for(uint32 i = 0; i < buffer->capacity; i++) {

                struct node* data = (struct node*)buffer->data;
                data[i].nameid++;

                for(uint32 j = 0; j < data[i].children->capacity; j++) {
                    struct node* cdata = data[i].children->data;
                    cdata[i].nameid++;
                }
            }
        }

        // perf_timer_stop_with_count(repeat * buffer->capacity, t);
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = 0,
            .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT }));

        print("perf with total buffers accesses:%u\n", total_buffers_elements);

        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = total_buffers_elements,
            .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT|PERF_T_USE_BUF_CAPACITY }));
        perf_timer_reset(t);
    }

    {
        const uint32 capacity = 16;
        const uint32 type_size = sizeof(struct node);
        buffer_t* buffer = buffer_create_gentype(capacity, type_size);
        print("add buffer at:%x\n", REDUCE_ADDR(buffer->data, F6));

        // fill buffer
        for(uint32 i = 0; i < buffer->capacity; i++) {

            buffer_set_for_insert(buffer, i);
            struct node* data = buffer->data;
            data[i].nameid = i+1;

            data[i].children = buffer_create_gentype(capacity, type_size);
            // print("add child at:%x\n", REDUCE_ADDR(data[i].children->data, F6));
            for(uint32 j = 0; j < data[i].children->capacity; j++) {
                buffer_set_for_insert(data[i].children, j);
                struct node* cdata = data[i].children->data;
                cdata[j].nameid = j+1;
                cdata[j].children = NULL;;

            }
        }


        const uint32 total_buffers_elements = buffer->capacity +
        (buffer->capacity * ((struct node*)buffer->data)[0].children->capacity);

        print("\n\n%s#### TEST:buffer capacities:%u%s\n", GREEN, capacity, CLEAR);

        // perf-access
        perf_timer_start(t);
        for(uint32 r = 0; r < repeat; r++) {
            for(uint32 i = 0; i < buffer->capacity; i++) {

                struct node* data = (struct node*)buffer->data;
                data[i].nameid++;

                for(uint32 j = 0; j < data[i].children->capacity; j++) {
                    struct node* cdata = data[i].children->data;
                    cdata[i].nameid++;
                }
            }
        }

        // perf_timer_stop_with_count(repeat * buffer->capacity, t);
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = 0,
            .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT }));

        print("perf with total buffers accesses:%u\n", total_buffers_elements);

        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = total_buffers_elements,
            .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT|PERF_T_USE_BUF_CAPACITY }));
        perf_timer_reset(t);
    }

    {
        const uint32 capacity = 64;
        const uint32 type_size = sizeof(struct node);
        buffer_t* buffer = buffer_create_gentype(capacity, type_size);

        // fill buffer
        for(uint32 i = 0; i < buffer->capacity; i++) {

            buffer_set_for_insert(buffer, i);
            struct node* data = buffer->data;
            data[i].nameid = i+1;

            data[i].children = buffer_create_gentype(capacity, type_size);
            // print("add child at:%x\n", REDUCE_ADDR(data[i].children->data, F6));
            for(uint32 j = 0; j < data[i].children->capacity; j++) {
                buffer_set_for_insert(data[i].children, j);
                struct node* cdata = data[i].children->data;
                cdata[j].nameid = j+1;
                cdata[j].children = NULL;;

            }
        }


        const uint32 total_buffers_elements = buffer->capacity +
        (buffer->capacity * ((struct node*)buffer->data)[0].children->capacity);

        print("\n\n%s#### TEST:buffer capacities:%u%s\n", GREEN, capacity, CLEAR);

        // perf-access
        perf_timer_start(t);
        for(uint32 r = 0; r < repeat; r++) {
            for(uint32 i = 0; i < buffer->capacity; i++) {

                struct node* data = (struct node*)buffer->data;
                data[i].nameid++;

                for(uint32 j = 0; j < data[i].children->capacity; j++) {
                    struct node* cdata = data[i].children->data;
                    cdata[i].nameid++;
                }
            }
        }

        // perf_timer_stop_with_count(repeat * buffer->capacity, t);
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = 0,
            .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT }));

        print("perf with total buffers accesses:%u\n", total_buffers_elements);

        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = total_buffers_elements,
            .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT|PERF_T_USE_BUF_CAPACITY }));
        perf_timer_reset(t);
    }

    {
        const uint32 capacity = 128;
        const uint32 type_size = sizeof(struct node);
        buffer_t* buffer = buffer_create_gentype(capacity, type_size);
        print("add buffer at:%x\n", REDUCE_ADDR(buffer->data, F6));

        // fill buffer
        for(uint32 i = 0; i < buffer->capacity; i++) {

            buffer_set_for_insert(buffer, i);
            struct node* data = buffer->data;
            data[i].nameid = i+1;

            data[i].children = buffer_create_gentype(capacity, type_size);
            // print("add child at:%x\n", REDUCE_ADDR(data[i].children->data, F6));
            for(uint32 j = 0; j < data[i].children->capacity; j++) {
                buffer_set_for_insert(data[i].children, j);
                struct node* cdata = data[i].children->data;
                cdata[j].nameid = j+1;
                cdata[j].children = NULL;;

            }
        }


        const uint32 total_buffers_elements = buffer->capacity +
        (buffer->capacity * ((struct node*)buffer->data)[0].children->capacity);

        print("\n\n%s#### TEST:buffer capacities:%u%s\n", GREEN, capacity, CLEAR);

        // perf-access
        perf_timer_start(t);
        for(uint32 r = 0; r < repeat; r++) {
            for(uint32 i = 0; i < buffer->capacity; i++) {

                struct node* data = (struct node*)buffer->data;
                data[i].nameid++;

                for(uint32 j = 0; j < data[i].children->capacity; j++) {
                    struct node* cdata = data[i].children->data;
                    cdata[i].nameid++;
                }
            }
        }

        // perf_timer_stop_with_count(repeat * buffer->capacity, t);
        perf_timer_stop(t);
        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = 0,
            .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT }));

        print("perf with total buffers accesses:%u\n", total_buffers_elements);

        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = total_buffers_elements,
            .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT|PERF_T_USE_BUF_CAPACITY }));
        perf_timer_reset(t);
    }


    TEMP_PRINT_BUFFER_TABLE_COUNT();

    memory_manager_terminate();
}

void // test buffer with children. All buffers with different (slab) capacities
perf_test_buffer_with_children_in_dif_slabs()
{

    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s test_buffer_with_children%s\n", CYAN, CLEAR);
    print("%s DIFFERENT BUFFER CAPACITIES FOR CHILDREN%s\n\n", CYAN, CLEAR);

    const uint32_t num_pages = 100;
    memory_manager_init(num_pages);

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



         uint32 total_buffers_elements = buffer->capacity;


        print("\n\n%sparent buffer capacity:%u%s\n", GREEN, capacity, CLEAR);

        uint64 sum = 0;
        // fill buffer
        for(uint32 i = 0; i < buffer->capacity; i++) {

            buffer_set_for_insert(buffer, i);
            struct node* data = buffer->data;
            data[i].nameid = i+1;

            dif_capacity *= 4;
            total_buffers_elements += dif_capacity;
            print("child capacity:%u\n", dif_capacity);

            data[i].children = buffer_create_gentype(dif_capacity, type_size);
            // print("add child at:%x\n", REDUCE_ADDR(data[i].children->data, F6));
            for(uint32 j = 0; j < data[i].children->capacity; j++) {
                buffer_set_for_insert(data[i].children, j);
                struct node* cdata = data[i].children->data;
                cdata[j].nameid = j+1;
                cdata[j].children = NULL;;


            }
        }


        // perf-access
        perf_timer_start(t);
        for(uint32 r = 0; r < repeat; r++) {
            for(uint32 i = 0; i < buffer->capacity; i++) {

                struct node* data = (struct node*)buffer->data;
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
            .flags = PERF_T_ALL|PERF_T_USE_REPEAT }));
            // .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT }));

        print("perf with total buffers accesses:%u\n", total_buffers_elements);

        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = total_buffers_elements,
            .flags = PERF_T_ALL|PERF_T_USE_REPEAT|PERF_T_USE_BUF_CAPACITY }));
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
    //     const uint32 total_buffers_elements = buffer->capacity +
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
    //     print("perf with total buffers accesses:%u\n", total_buffers_elements);
    //
    //     perf_timer_print(timer_print_config((timer_print_params){
    //         .t = t, .repeat = repeat, .buf_capacity = total_buffers_elements,
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
    //     const uint32 total_buffers_elements = buffer->capacity +
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
    //     print("perf with total buffers accesses:%u\n", total_buffers_elements);
    //
    //     perf_timer_print(timer_print_config((timer_print_params){
    //         .t = t, .repeat = repeat, .buf_capacity = total_buffers_elements,
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
    //     const uint32 total_buffers_elements = buffer->capacity +
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
    //     print("perf with total buffers accesses:%u\n", total_buffers_elements);
    //
    //     perf_timer_print(timer_print_config((timer_print_params){
    //         .t = t, .repeat = repeat, .buf_capacity = total_buffers_elements,
    //         .flags = PERF_T_COMMON|PERF_T_INLINE|PERF_T_USE_REPEAT|PERF_T_USE_BUF_CAPACITY }));
    //     perf_timer_reset(t);
    // }


    TEMP_PRINT_BUFFER_TABLE_COUNT();

    memory_manager_terminate();
}

