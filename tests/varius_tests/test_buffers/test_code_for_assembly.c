/*
 * Created by ckalo on 8/20/26.
 */


#include <stdatomic.h>

#include "my_types.h"

void Ibuffer_set(float* Ibuffer, uint32 obj_idx, uint32 elem_count, uint32 elem_offset, float* src, uint32 ibuf_stride)
{
    const uint32 idx = (obj_idx*ibuf_stride)+elem_offset;
    for(uint32 i = 0; i < elem_count; i++) {
        Ibuffer[idx + i] = src[i];
    }
}

void AAAAA(float* Ibuffer, uint32 rep, uint32 num_elems, uint32* r_i, uint32 col_count, uint32 scale_count, uint32 wpos_count, float g_col[4])
{
    for(uint32 k = 0; k < rep; k++) {
        for(uint32 i = 0; i < num_elems; i++) {
            Ibuffer_set(Ibuffer, r_i[i], col_count, scale_count+wpos_count, g_col, 0);
            // Compiler must not reorder these stores
            atomic_signal_fence(memory_order_release);
        }
    }
}
