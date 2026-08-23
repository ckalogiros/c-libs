/*
 * Created by ckalo on 8/20/26.
 */

#include "unrolled_loop_access.h"

#include "logger.h"


/*****************************************************************************************/
uint32 unrolled_interleaved(volatile float* buf, uint32 buf_stride,
    float scale[2], float wpos[3], float col[4],
    volatile uint32* index_buf, uint32 index_buf_stride
    )
{
    uint32 iidx = index_buf_stride;
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }
    {
        const uint32 idx = (index_buf[iidx]*buf_stride);
        buf[idx + 0] = scale[0];
        buf[idx + 1] = scale[1];
        buf[idx + 2] = wpos [0];
        buf[idx + 3] = wpos [1];
        buf[idx + 4] = wpos [2];
        buf[idx + 5] = col  [0];
        buf[idx + 6] = col  [1];
        buf[idx + 7] = col  [2];
        buf[idx + 8] = col  [3];
        iidx++;
    }

    return iidx;
}


uint32 unrolled_separate(
    volatile float* scale_buf, uint32 scale_buf_stride,
    volatile float* wpos_buf,  uint32 wpos_buf_stride,
    volatile float* col_buf,   uint32 col_buf_stride,
    float scale[2], float wpos[3], float col[4],
    volatile uint32* index_buf, uint32 index_buf_stride
    )
{
    uint32 iidx = index_buf_stride;
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }
    {
        const uint32 sca_idx = (index_buf[iidx]*scale_buf_stride);
        const uint32 wpo_idx = (index_buf[iidx]*wpos_buf_stride);
        const uint32 col_idx = (index_buf[iidx]*col_buf_stride);
        scale_buf[sca_idx + 0] = scale[0];
        scale_buf[sca_idx + 1] = scale[1];
        wpos_buf [wpo_idx + 0] = wpos [0];
        wpos_buf [wpo_idx + 1] = wpos [1];
        wpos_buf [wpo_idx + 2] = wpos [2];
        col_buf  [col_idx + 0] = col  [0];
        col_buf  [col_idx + 1] = col  [1];
        col_buf  [col_idx + 2] = col  [2];
        col_buf  [col_idx + 3] = col  [3];
        iidx++;
    }

    return iidx;
}