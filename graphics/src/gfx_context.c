/*
 * Created by ckalo on 4/8/26.
 */


#include "gfx_buffers.h"
#include "shader_id.h"


/***************************************************************************************************
 * find match: vertex buffer
 **************************************************************************************************/
uint32
gfx_vertex_buffer_find_match(const struct gfx_sid sid)
{
    struct vertex_buffer_object* vbtable = vertex_buffer_get_table();
    uint32 count = vertex_buffer_get_table_count();
    for(uint32 i = 0; i < count; i++) {
        struct vertex_buffer_object* vb = &vbtable[i];
        if(sid_check_match(sid, vb->sid)) {
            return i;
        }
    }

    return INT_NULL;
}
