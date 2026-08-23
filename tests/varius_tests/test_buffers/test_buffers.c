/*
 * Created by ckalo on 8/18/26.
 */




#include "cpu.h"
#include "test_buffers.h"

#include <assert.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <time.h>

#include "logger.h"
#include "my_types.h"
#include "precision_timer.h"
#include "precision_timer_logger.h"
#include "unrolled_loop_access.h"


#define num_elems   1024
// #define num_elems   2048*16
#define scale_count 2
#define wpos_count  3
#define col_count   4
#define ibuf_stride (scale_count+wpos_count+col_count)
#define rep 4000000
// #define rep 1
#define num_of_rand_idxs num_elems
#define rand_buffer_count num_elems
#define rand_max_index_cap num_elems /// The maximum index the random generator will generate. MUST BE <= num_elems
#define total_vector_elems ibuf_stride+scale_count+wpos_count+col_count
#define total_elements total_vector_elems*num_elems


// #define unrolled_cap 64
// #define random_cap num_elems / unrolled_cap

volatile int r_i[rand_buffer_count];

static uint32 obj_id = 0;
static uint32 Iobj_id = 0;


volatile float* storage = NULL;
volatile float Ibuffer  [num_elems*ibuf_stride] = {0};
volatile float wpos_buf [num_elems*wpos_count]  = {0};
volatile float col_buf  [num_elems*col_count]   = {0};
volatile float scale_buf[num_elems*scale_count] = {0};

uint32 Ibuf_idx  = 0;
uint32 scale_idx = 0;
uint32 wpos_idx  = 0;
uint32 col_idx   = 0;

struct object {
    int32 id;
};
struct Iobject {
    int32 id;
};

int32 create_object         (float scale[2], float wpos[3], float col[4]);
int32 create_Iobject        (float scale[2], float wpos[3], float col[4]);
int32 buffer_add_interleaved(float scale[2], float wpos[3], float col[4]);
int32 buffer_scale_add      (float scale[3]);
int32 buffer_wpos_add       (float wpos[3]);
int32 buffer_col_add        (float col[4]);

void Ibuffer_set            (uint32 obj_idx, uint32 elem_count, uint32 elem_offset, float* src, struct timer* t);
void buffer_set_scale       (uint32 obj_idx, uint32 elem_count, float* src, struct timer* t);
void buffer_set_wpos        (uint32 obj_idx, uint32 elem_count, float* src, struct timer* t);
void buffer_set_col         (uint32 obj_idx, uint32 elem_count, float* src, struct timer* t);
void access();

void Ibuffer_set_obj_unroll(uint32 obj_idx, float scale[2], float wpos[3], float col[4], struct timer* t);
void buffer_set_obj_unroll(uint32 obj_idx, float scale[2], float wpos[3], float col[4], struct timer* t);


void perf_timer_measures_the_whole_loop();
void perf_timer_measures_the_access();
void perf_timer_color_vector();
void perf_timer_measures_unrolled();
void perf_timer_measures_complete_unrolled();

int32 some_storage[num_elems];

void print_Ibuffer();
void print_buffers();
void print_timer(const uint64 t1_avg, const uint64 t2_avg);





/*****************************************************************************************/
/// create random indices for buffer random access
int random_int() {
    // Better distribution (avoids modulo bias)
    int random = (int)((double)rand() / RAND_MAX * rand_max_index_cap);
    return random;
}

float g_scale[scale_count]    = {1, 2};
float g_wpos [wpos_count]     = {1, 2, 3};
float g_col  [col_count]      = {1, 2, 3, 4};
/*****************************************************************************************/
void test_buffers_access()
{
    pin_to_cpu(CPU_THREAD_6);
    // set_high_priority();

    assert(rand_max_index_cap <= num_elems);

    /* initialization */
    for(uint32 i = 0; i < num_elems; i++) {
        int32 id = create_object(g_scale, g_wpos, g_col);
    }
    // print_buffers();
    for(uint32 i = 0; i < num_elems; i++) {
        int32 id = create_Iobject(g_scale, g_wpos, g_col);
    }
    // print_Ibuffer();

    /// create random indices for random access
    srand(time(NULL));
    for(uint32 i = 0; i < num_of_rand_idxs; i++) r_i[i] = random_int(); /// random access into buffers
    // for(uint32 i = 0; i < num_of_rand_idxs; i++) r_i[i] = i; /// contiguous access into buffers
    // for(uint32 i = 0; i < num_elems; i++) print("Random number: %d\n", r_i[i]);

    timer_init_table();

    storage = (float*)malloc(total_elements*SIZE_F32);

    /* measure performance */
    // perf_timer_measures_the_whole_loop();
    // perf_timer_color_vector();
    // perf_timer_measures_unrolled();
    perf_timer_measures_complete_unrolled();


    timer_destroy_table();
}
void perf_timer_measures_the_whole_loop()
{
    struct timer* t = timer_create_perf_timer();

    print("-- TEST 1: MEASURE THE LOOP OF ACCESSING ALL ELEMENTS --\n "
          "Update scale, wpos and col vectors to the two buffer structures.\n "
          "1. contiguous buffer of type: { [scale] [wpos] [col] },\n "
          "2. 3 separate buffers of type: buf[scale] buf[wpos] buf[col] },\n "
          "Calling 3 separate functions, setting the 3 vectors.\n "
          "The former loop repeats: %u times.\n "
          "The time measurment happens in the inner loop,\n "
          "the one that runs for num objects.\n "
          "Number of objects: %u.\n "
          "Repeat: %u.\n "
          "Random indices generated: %u.\n "
          "Random generator max index cap: %u.\n "
          "Random generator buffer count: %u.\n "
          "",
          rep, num_elems, rep, num_of_rand_idxs, rand_max_index_cap, rand_buffer_count
          );


    /// test: interleaved buffer
    perf_timer_start_accum(t);
    for(uint32 k = 0; k < rep; k++) {
        for(uint32 i = 0; i < num_elems; i++) {
            Ibuffer_set(r_i[i], scale_count, 0, g_scale, NULL);
            atomic_signal_fence(memory_order_release);
            Ibuffer_set(r_i[i], wpos_count, scale_count, g_wpos, NULL);
            atomic_signal_fence(memory_order_release);
            Ibuffer_set(r_i[i], col_count, scale_count+wpos_count, g_col, NULL);
            atomic_signal_fence(memory_order_release);
        }
    }
    perf_timer_stop_accum(t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = rep, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
    }));
    const uint64 t1_accum = t->accum; /// store average timer result

    perf_timer_reset(t);

    /// test: seperated buffers
    perf_timer_start_accum(t);
    for(uint32 k = 0; k < rep; k++) {
        for(uint32 i = 0; i < num_elems; i++) {
            atomic_signal_fence(memory_order_release);
            buffer_set_scale(r_i[i], scale_count, g_scale, NULL);
            atomic_signal_fence(memory_order_release);
            buffer_set_wpos (r_i[i], wpos_count, g_wpos, NULL);
            atomic_signal_fence(memory_order_release);
            buffer_set_col  (r_i[i], col_count, g_col, NULL);
        }
    }
    perf_timer_stop_accum(t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = rep, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
    }));
    const uint64 t2_accum = t->accum; /// store average timer result


    print_timer(t1_accum, t2_accum);
}
void perf_timer_color_vector()
{
    struct timer* t = timer_create_perf_timer();

    print("-- TEST 1: MEASURE THE LOOP OF ACCESSING ALL ELEMENTS --\n "
          "Update scale, wpos and col vectors to the two buffer structures.\n "
          "1. contiguous buffer of type: { [scale] [wpos] [col] },\n "
          "2. 3 separate buffers of type: buf[scale] buf[wpos] buf[col] },\n "
          "Calling 3 separate functions, setting the 3 vectors.\n "
          "The former loop repeats: %u times.\n "
          "The time measurment happens in the inner loop,\n "
          "the one that runs for num objects.\n "
          "Number of objects: %u.\n "
          "Repeat: %u.\n "
          "Random indices generated: %u.\n "
          "Random generator max index cap: %u.\n "
          "Random generator buffer count: %u.\n "
          "",
          rep, num_elems, rep, num_of_rand_idxs, rand_max_index_cap, rand_buffer_count
          );
    /// test: interleaved buffer
    perf_timer_start_accum(t);
    for(uint32 k = 0; k < rep; k++) {
        for(uint32 i = 0; i < num_elems; i++) {
            Ibuffer_set(r_i[i], col_count, scale_count+wpos_count, g_col, NULL);
            // Compiler must not reorder these stores
            atomic_signal_fence(memory_order_release);
        }
    }
    perf_timer_stop_accum(t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = rep, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
    }));
    const uint64 t1_accum = t->accum; /// store average timer result

    perf_timer_reset(t);

    /// test: seperated buffers
    perf_timer_start_accum(t);
    for(uint32 k = 0; k < rep; k++) {
        for(uint32 i = 0; i < num_elems; i++) {
            buffer_set_col(r_i[i], col_count, g_col, NULL);
            // Compiler must not reorder these stores
            atomic_signal_fence(memory_order_release);
        }
    }
    perf_timer_stop_accum(t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = rep, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
    }));
    const uint64 t2_accum = t->accum; /// store average timer result

    print_timer(t1_accum, t2_accum);

}
void perf_timer_measures_unrolled()
{
    struct timer* t = timer_create_perf_timer();

    print("-- TEST 1: perf_timer_measures_unrolled --\n "
          "Update scale, wpos and col vectors to the two buffer structures.\n "
          "1. contiguous buffer of type: { [scale] [wpos] [col] },\n "
          "2. 3 separate buffers of type: buf[scale] buf[wpos] buf[col] },\n "
          "Calling 3 separate functions, setting the 3 vectors.\n "
          "The former loop repeats: %u times.\n "
          "The time measurment happens in the inner loop,\n "
          "the one that runs for num objects.\n "
          "Number of objects: %u.\n "
          "Repeat: %u.\n "
          "Random indices generated: %u.\n "
          "Random generator max index cap: %u.\n "
          "Random generator buffer count: %u.\n "
          "",
          rep, num_elems, rep, num_of_rand_idxs, rand_max_index_cap, rand_buffer_count
          );


    /// test: interleaved buffer
    perf_timer_start_accum(t);
    for(uint32 k = 0; k < rep; k++) {
        for(uint32 i = 0; i < num_elems; i++) {
            Ibuffer_set_obj_unroll(r_i[i], g_scale, g_wpos, g_col, t);
            // Compiler must not reorder these stores
            atomic_signal_fence(memory_order_release);
        }
    }
    perf_timer_stop_accum(t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = rep, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
    }));
    const uint64 t1_accum = t->accum; /// store average timer result

    perf_timer_reset(t);

    /// test: seperated buffers
    perf_timer_start_accum(t);
    for(uint32 k = 0; k < rep; k++) {
        for(uint32 i = 0; i < num_elems; i++) {
            buffer_set_obj_unroll(r_i[i], g_scale, g_wpos, g_col, t);
            // Compiler must not reorder these stores
            atomic_signal_fence(memory_order_release);
        }
    }
    perf_timer_stop_accum(t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = rep, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
    }));
    const uint64 t2_accum = t->accum; /// store average timer result


    print_timer(t1_accum, t2_accum);
}
void perf_timer_measures_complete_unrolled()
{
    struct timer* t = timer_create_perf_timer();

    print("-- TEST: perf_timer_measures_complete_unrolled--\n "
          "Update scale, wpos and col vectors to the two buffer structures.\n "
          "1. contiguous buffer of type: { [scale] [wpos] [col] },\n "
          "2. 3 separate buffers of type: buf[scale] buf[wpos] buf[col] },\n "
          "Calling 3 separate functions, setting the 3 vectors.\n "
          "The former loop repeats: %u times.\n "
          "The time measurment happens in the inner loop,\n "
          "the one that runs for num objects.\n "
          "Number of objects: %u.\n "
          "Repeat: %u.\n "
          "Random indices generated: %u.\n "
          "Random generator max index cap: %u.\n "
          "Random generator buffer count: %u.\n "
          "",
          rep, num_elems, rep, num_of_rand_idxs, rand_max_index_cap, rand_buffer_count
          );

    /* the unrolled functions use "random_index_buf_cap" number of random indexes.
     * in order to use as many random indexes(from r_i),
     * we use a stride to index the r_i buffer to continue indexing
     * from the last index of the previous call to unrolled(),
     * The instructions:
     *         r_ii+=r_i_used_indexes;
     *         if((r_ii+r_i_used_indexes) >= num_elems) r_ii = 0;
     *
     * MAKE sure the r_ii do NOT exceed the r_i buffer capacity.
     */
    uint32 r_ii = 0;

    /// test: interleaved buffer
    r_ii = 0;
    {
        perf_timer_start_accum(t);
        for(uint32 k = 0; k < rep; k++) {

            uint32 r_i_used_indexes = unrolled_interleaved(
                Ibuffer, ibuf_stride,
                g_scale, g_wpos, g_col,
                r_i, r_ii);

            r_ii=r_i_used_indexes;
            if((r_ii+random_index_buf_cap) >= num_elems) r_ii = 0;
            // print("index_buf_stride=%u\n", r_ii);

            /// check if the unrolled function indexed the random buffer inside its elements count
            assert(r_i_used_indexes < rand_buffer_count);
        }
        perf_timer_stop_accum(t);
        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = rep, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
        }));
    }
    const uint64 t1_accum = t->accum; /// store average timer result

    perf_timer_reset(t);

    /// test: seperated buffers
    r_ii = 0;
    {
        perf_timer_start_accum(t);
        for(uint32 k = 0; k < rep; k++) {
            uint32 r_i_used_indexes = unrolled_separate(
                scale_buf, scale_count,
                wpos_buf,  wpos_count,
                col_buf,   col_count,
                g_scale, g_wpos, g_col,
                r_i, r_ii);

            r_ii=r_i_used_indexes;
            if((r_ii+random_index_buf_cap) >= num_elems) r_ii = 0;
            // print("index_buf_stride=%u\n", r_ii);

            /// check if the unrolled function indexed the random buffer inside its elements count
            assert(r_i_used_indexes < rand_buffer_count);
        }
        perf_timer_stop_accum(t);
        perf_timer_print(timer_print_config((timer_print_params){
            .t = t, .repeat = rep, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
        }));
    }
    const uint64 t2_accum = t->accum; /// store average timer result


    // print_timer(t2_accum, t1_accum);
    print_timer(t1_accum, t2_accum);
}


/*****************************************************************************************/
int32 create_object (float scale[2], float wpos[3], float col[4])
{
    struct object obj = {
        .id = obj_id++,
    };
    
    buffer_scale_add(scale);
    buffer_wpos_add(wpos);
    buffer_col_add(col);
    return obj.id;
}
int32 create_Iobject(float scale[2], float wpos[3], float col[4])
{
    struct Iobject obj = {
        .id = Iobj_id++,
    };

    buffer_add_interleaved(scale, wpos, col);
    return obj.id;
}



/*****************************************************************************************/
int32 buffer_add_interleaved(float scale[2], float wpos[3], float col[4])
{
    for(uint32 i = 0; i < scale_count; i++) {
        Ibuffer[Ibuf_idx++] = scale[i];
    }
    for(uint32 i = 0; i < wpos_count; i++) {
        Ibuffer[Ibuf_idx++] = wpos[i];
    }
    for(uint32 i = 0; i < col_count; i++) {
        Ibuffer[Ibuf_idx++] = col[i];
    }
    return Ibuf_idx;
}
int32 buffer_scale_add(float scale[2])
{
    for(uint32 i = 0; i < scale_count; i++) {
        scale_buf[scale_idx++] = scale[i];
    }
    return scale_idx;
}
int32 buffer_wpos_add (float wpos[3])
{
    for(uint32 i = 0; i < wpos_count; i++) {
        wpos_buf[wpos_idx++] = wpos[i];
    }
    return wpos_idx;
}
int32 buffer_col_add  (float col[4])
{
    for(uint32 i = 0; i < col_count; i++) {
        col_buf[col_idx++] = col[i];
    }
    return col_idx;
}



/*****************************************************************************************/
void Ibuffer_set(uint32 obj_idx, uint32 elem_count, uint32 elem_offset, float* src, struct timer* t)
{
    const uint32 idx = (obj_idx*ibuf_stride)+elem_offset;
    for(uint32 i = 0; i < elem_count; i++) {
        Ibuffer[idx + i] = src[i];
    }
}
void buffer_set_scale(uint32 obj_idx, uint32 elem_count, float* src, struct timer* t)
{
    for(uint32 i = 0; i < elem_count; i++) {
        scale_buf[obj_idx + i] = src[i];
    }
}
void buffer_set_wpos (uint32 obj_idx, uint32 elem_count, float* src, struct timer* t)
{
    for(uint32 i = 0; i < elem_count; i++) {
       wpos_buf[obj_idx + i] = src[i];
    }
}
void buffer_set_col  (uint32 obj_idx, uint32 elem_count, float* src, struct timer* t)
{
    for(uint32 i = 0; i < elem_count; i++) {
        col_buf[obj_idx + i] = src[i];
    }
}
void access()
{
    /// test: interleaved buffer
    for(uint32 i = 0; i < num_elems; i++) {

        int32 idx = r_i[i];
        some_storage[i] = Ibuffer[idx];

    }
    /// test: interleaved buffer
    for(uint32 i = 0; i < num_elems; i++) {

        int32 idx = r_i[i];
        some_storage[i] = Ibuffer[idx];

    }

}



/*****************************************************************************************/
void Ibuffer_set_obj_unroll(uint32 obj_idx, float scale[2], float wpos[3], float col[4], struct timer* t)
{
    const uint32 idx = (obj_idx*ibuf_stride);
    Ibuffer[idx + 0] = scale[0];
    Ibuffer[idx + 1] = scale[1];
    Ibuffer[idx + 2] = wpos [0];
    Ibuffer[idx + 3] = wpos [1];
    Ibuffer[idx + 4] = wpos [2];
    Ibuffer[idx + 5] = col  [0];
    Ibuffer[idx + 6] = col  [1];
    Ibuffer[idx + 7] = col  [2];
    Ibuffer[idx + 8] = col  [3];
}
void buffer_set_obj_unroll(uint32 obj_idx, float scale[2], float wpos[3], float col[4], struct timer* t)
{
    const uint32 sca_idx = (obj_idx*scale_count);
    const uint32 wpo_idx = (obj_idx*wpos_count);
    const uint32 col_idx = (obj_idx*col_count);
    scale_buf[sca_idx + 0] = scale[0];
    scale_buf[sca_idx + 1] = scale[1];
    wpos_buf [wpo_idx + 0] = wpos [0];
    wpos_buf [wpo_idx + 1] = wpos [1];
    wpos_buf [wpo_idx + 2] = wpos [2];
    col_buf  [col_idx + 0] = col  [0];
    col_buf  [col_idx + 1] = col  [1];
    col_buf  [col_idx + 2] = col  [2];
    col_buf  [col_idx + 3] = col  [3];

}




/*****************************************************************************************/
/// print

void print_buffers()
{
    uint32 i=0,j=0,k=0;
    for(uint32 idx = 0; idx < num_elems; idx++) {
        print("[%3u] ", idx);
        for(uint32 ii = 0; ii < scale_count; ii++) {
            print("s:%.2f, ", scale_buf[i++]);
        }
        for(uint32 ii = 0; ii < wpos_count; ii++) {
            print("w:%.2f, ", wpos_buf[j++]);
        }
        for(uint32 ii = 0; ii < col_count; ii++) {
            print("c:%.2f ", col_buf[k++]);
        }
        print("\n");
    }

}
void print_Ibuffer()
{
    uint32 k = 0;
    for(uint32 i = 0; i < num_elems; i++) {
        print("[%3u] ", i);

        print("s:%.2f, ", Ibuffer[k++]);
        print("%.2f, ", Ibuffer[k++]);

        print("w:%.2f, ", Ibuffer[k++]);
        print("%.2f, ", Ibuffer[k++]);
        print("%.2f, ", Ibuffer[k++]);

        print("c:%.2f, ", Ibuffer[k++]);
        print("%.2f, ", Ibuffer[k++]);
        print("%.2f, ", Ibuffer[k++]);
        print("%.2f\n", Ibuffer[k++]);
    }


}
void print_timer(const uint64 t1_accum, const uint64 t2_accum)
{
    const uint64 t1_avg = t1_accum / rep;
    const uint64 t2_avg = t2_accum / rep;
    // const uint64 t1_avg = t1_accum;
    // const uint64 t2_avg = t2_accum;
    const uint64 larger  = GET_MAX(t1_avg, t2_avg);
    const uint64 smaller = GET_MIN(t1_avg, t2_avg);
    const uint64 dif = larger - smaller;

    // make double type after the division so we dont loose from times being very large integers,
    // which is the cace for time in nanoseconds
    const double faster = (double)dif / (double)larger  * 100;
    const double slower = (double)dif / (double)smaller * 100;
    print("buf interleaved   accum: %llu\nseparated buffers accum: %llu\n", t1_avg, t2_avg);
    print("%s Diff in nanosec: %llu %s\n", YELLOW, dif, CLEAR);
    const char* faster_str = (t1_avg > t2_avg) ? "buf  interleaved" : "separated buffers";
    const char* slower_str = (t1_avg < t2_avg) ? "buf  interleaved" : "separated buffers";
    print("{%s} faster %.2lf%% from {%s}\n", slower_str, faster,  faster_str);
    print("{%s} slower %.2lf%% from {%s}\n", faster_str, slower,  slower_str);
}
// void print_timer(const uint64 t1_avg, const uint64 t2_avg)
// {
//     const uint64 larger  = GET_MAX(t1_avg, t2_avg);
//     const uint64 smaller = GET_MIN(t1_avg, t2_avg);
//     const uint64 dif = larger - smaller;
//     // make double type after the division so we dont loose from times being very large integers,
//     // which is the cace for time in nanoseconds
//     const double faster = (double)dif / (double)larger * 100;
//     const double slower = (double)dif / (double)smaller * 100;
//     print("buf interleaved  accum: %llu\nseparated buffers accum: %llu\n", t1_avg, t2_avg);
//     const char* faster_str = (t1_avg > t2_avg) ? "buf  interleaved" : "separated buffers";
//     const char* slower_str = (t1_avg < t2_avg) ? "buf  interleaved" : "separated buffers";
//     print("{%s} faster %.2lf%% from {%s}\n", slower_str, faster,  faster_str);
//     print("{%s} slower %.2lf%% from {%s}\n", faster_str, slower,  slower_str);
// }























