/*
 * Created by ckalo on 4/8/26.
 */


#include "gfx_buffers.h"
#include "shader_descriptor.h"
#include "shader_id.h"

/***************************************************************************************************
 * global buffer: sid
 **************************************************************************************************/
#define SID_MAX_COUNT 32
struct gfx_sid gfx_sid_buffer[32];
sididx_t gfx_sid_buffer_count = 0;
struct gfx_sid gfx_get_sid(const sididx_t sid_idx){ return gfx_sid_buffer[sid_idx]; }
// void gfx_set_sid(const sididx_t sid_idx){ return gfx_sid_buffer[sid_idx]; }
sididx_t gfx_create_sid(struct gfx_sid sid)
{
    assert(gfx_sid_buffer_count < SID_MAX_COUNT);
    sididx_t idx = gfx_sid_buffer_count;
    gfx_sid_buffer[idx] = sid;
    gfx_sid_buffer_count++;
    return idx;
}


/***************************************************************************************************
 * find match: vertex array
 **************************************************************************************************/
vaidx_t
gfx_vertex_array_find_match(const sididx_t sid_idx, const bool attrib_devisor)
{
    uint32 count = vertex_buffer_get_table_count();
    for(uint32 i = 0; i < count; i++) {
        struct vertex_array_object* va = gfx_vertex_array_get_byidx(i);
        struct shader_descriptor* desc = shader_descriptor_get(va->desc_idx);
        if( sid_check_match(sid_idx, desc->sid_idx) && (attrib_devisor == va->attrib_devisor) ) {
            return i; /// both, sid and attrib_devisor, must match
        }
    }

    return INT_NULL;
}



/***************************************************************************************************
 * find match: vertex buffer
 **************************************************************************************************/
vbidx_t
gfx_vertex_buffer_find_match(const sididx_t sid_idx)
{
    uint32 count = vertex_buffer_get_table_count();
    for(uint32 i = 0; i < count; i++) {
        struct vertex_buffer_object* vb = vertex_buffer_get_vbo(i);
        const sididx_t vb_sid = shader_dscriptor_get_sid(vb->build_idx);
        if(sid_check_match(sid_idx, vb_sid)) {
            return i;
        }
    }

    return INT_NULL;
}
