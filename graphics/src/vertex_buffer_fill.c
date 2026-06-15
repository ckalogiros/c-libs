/*
 * Created by ckalo on 5/20/26.
 */

#include "vertex_buffer_fill.h"

const float mesh_quad[] = {
    -1, -1,
     1, -1,
     1,  1,
    -1, -1,
     1,  1,
    -1,  1,
};

inline void
vertex_buffer_fill_vpos(float* vb, uint32* index, const uint32 start)
{
    uint32 k = 0;
    *index += start;
    vb[*index++] = mesh_quad[k++];
}

