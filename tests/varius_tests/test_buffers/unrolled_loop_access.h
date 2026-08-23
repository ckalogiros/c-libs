/*
 * Created by ckalo on 8/20/26.
 */
#ifndef VARIUS_TESTS_UNROLLED_LOOP_ACCESS_H
#define VARIUS_TESTS_UNROLLED_LOOP_ACCESS_H
#include "my_types.h"

/* This MUST be the number of times the following block exists in the unrolled functions
 *     {
 *      const uint32 idx = (index_buf[iidx]*buf_stride);
 *      buf[idx + 0] = scale[0];
 *      buf[idx + 1] = scale[1];
 *      buf[idx + 2] = wpos [0];
 *      buf[idx + 3] = wpos [1];
 *      buf[idx + 4] = wpos [2];
 *      buf[idx + 5] = col  [0];
 *      buf[idx + 6] = col  [1];
 *      buf[idx + 7] = col  [2];
 *      buf[idx + 8] = col  [3];
 *      iidx++;
 *  }
 *
 *  random_index_buf_cap = 64, means we must have exactly 64 of these blocks.
 */
#define random_index_buf_cap 64

uint32 unrolled_interleaved(
    volatile float* buf, uint32 buf_stride,
    float scale[2], float wpos[3], float col[4],
    volatile uint32* index_buf, uint32 index_buf_count);

uint32 unrolled_separate(
    volatile float* scale_buf, uint32 scale_buf_stride,
    volatile float* wpos_buf,  uint32 wpos_buf_stride,
    volatile float* col_buf,   uint32 col_buf_stride,
    float scale[2], float wpos[3], float col[4],
    volatile uint32* index_buf, uint32 index_buf_count);

#endif //VARIUS_TESTS_UNROLLED_LOOP_ACCESS_H