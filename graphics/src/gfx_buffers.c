/*
 * Created by ckalo on 4/8/26.
 */

#include "gfx_buffers.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "mesh.h"
#include "opengl.h"
#include "opengl_debug.h"
#include "shader_builder.h"
#include "test.h"


/***************************************************************************************************
 * vertex array
 **************************************************************************************************/
struct vertex_array_object vertex_array_table[4];
uint32 vertex_array_table_count = 0;
struct vertex_array_object* gfx_vertex_array_get_byidx(uint32 idx){ return &vertex_array_table[idx]; }
uint32 gfx_vertex_array_create(const desc_t desc_idx)
{
    GLuint vao;
    gl_gen_vertex_array(1, &vao);
    GL_CHECK();
    gl_bind_vertex_array(vao);
    GL_CHECK();

    const vbidx_t vaidx = vertex_array_table_count;
    vertex_array_table_count++;

    struct vertex_array_object* va = &vertex_array_table[vaidx];
    va->idx = vaidx;
    va->vao = vao;
    va->desc_idx = desc_idx;

    /// TODO: ADD COMMAND: enable attributes
    // shader_attributes_enable(desc->vshader_attrib_locations_count, desc->vshader_attribs_stride, desc->last_loc_num_elems);
    // opengl_debug_attributes_in(desc->vshader_attrib_locations_count);

    return vaidx;
}
void gfx_vertex_array_delete(void){}



/***************************************************************************************************
 * vertex buffer
 **************************************************************************************************/
struct vertex_buffer_object vertex_buffer_table[4];
uint32 vertex_buffer_table_count = 0;

// struct vertex_buffer_object* vertex_buffer_get_table(void){ return vertex_buffer_table; }
uint32 vertex_buffer_get_table_count(void){ return vertex_buffer_table_count; }
struct vertex_buffer_object* vertex_buffer_get_vbo(vbidx_t vbidx){ return &vertex_buffer_table[vbidx]; }

uint32 vertex_buffer_get_vertex_count(vbidx_t vbidx){ return vertex_buffer_table[vbidx].count; }

uint32 gfx_vertex_buffer_create(const build_t build_idx, const uint32 data_size, const uint32 stride)
{
    GLuint vbo;
    gl_gen_buffers(1, &vbo);

    const vbidx_t vbidx = vertex_buffer_table_count;
    vertex_buffer_table_count++;

    // const uint32 data_size = 4096; // TODO: temp size, use sparse array for dynamically grow vertex buffer

    struct vertex_buffer_object* vb = &vertex_buffer_table[vbidx];
    vb->idx = vbidx;
    vb->vbo = vbo;
    // vb->sid = sid;
    vb->data = malloc(data_size);
    memset(vb->data, 0xCD, data_size);
    vb->size = data_size;
    vb->count = 0;
    vb->stride = stride;

    /// TODO: ADD COMMAND: create vertex array
    // struct va_cmd_params params = { .desc_idx = desc_idx };
    // void* p = &params;
    // cmd_add_command((void*)gfx_vertex_array_create, p, sizeof(struct va_cmd_params));


    return vbidx;
}
void gfx_vertex_buffer_delete(uint32 n, vbidx_t* buffer){ gl_delete_buffers(n, (GLint*)buffer); }
void gfx_vertex_buffer_push_data(const vbidx_t vbidx, void* data){}
void gfx_vertex_buffer_update_data(struct vb_cmd_params* params)
{
    const vbidx_t vbo = vertex_buffer_table[params->vbidx].vbo;
    void* const data = vertex_buffer_table[params->vbidx].data;
    const uint32 data_size = vertex_buffer_table[params->vbidx].count;
    /// check if vertex buffer is large enough
    assert(data_size*SIZE_F32 <= vertex_buffer_table[params->vbidx].size);

    GL_CHECK();
    gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo);
    GL_CHECK();

    // TODO: neither the program or the vertex array are bound
    // gl_buffer_data(OPENGL_ARRAY_BUFFER, data_size*SIZE_F32, &data[0], OPENGL_STATIC_DRAW);
    gl_buffer_data(OPENGL_ARRAY_BUFFER, data_size*SIZE_F32, data, OPENGL_STATIC_DRAW);
    GL_CHECK();

}


uint32 color_cnt = 0;
#define COLORS_COUNT 3*4
static float gfx_color_array[COLORS_COUNT] = {
    /// Basic colors
    0.1, 0.5, 1.0, 1.0,   /// BLUE
    0.9, 0.5, 0.1, 1.0,   ///
    0.6, 0.1, 0.8, 1.0,   ///
};


/// TODO: (SET VERTEX BUGGER)-(SET VERTEX BUFFER REGION)
/// TODO: eventualy this function will be implemented for "per vertex" and "per instance" attributes
float* gfx_vertex_buffer_set_data(struct vb_cmd_params* params)
{
    struct vertex_buffer_object* vb = &vertex_buffer_table[params->vbidx];

    struct object_components_descriptor* obj_desc = test_get_comp_desc(params->vbidx);
    // uint8 num_vpos = shaderinfo_flag_to_numelems(params->vpos_num_elems);
    uint16 vpos_count = obj_desc->obj_vertex_count * params->vpos_num_elems;
    float* obj_attr_data = test_get_obj_attribs_data(params->vbidx);
    uint32 cnt1 = 0;

    uint32 stride = vb->stride - params->vpos_num_elems; // stride becomes minus the vertex position's number of elements

    print("\n\n%s- ADD OBJ TO VB -",  YELLOW);
    for(uint32 objid = 0; objid < obj_desc->obj_count; objid++)
    {
        for(uint32 vertid = 0; vertid < obj_desc->obj_vertex_count; vertid++)
        {
            if(vb->count % (stride + params->vpos_num_elems) == 0) {
                print("\n[%2u][%3u]: ", vertid+1, vb->count);
            }

            for(uint8 j = 0; j < params->vpos_num_elems; j++) {
                if(cnt1 >= vpos_count) cnt1 = 0;
                vb->data[vb->count++] = params->vpos[cnt1++];
                print("%4.1f, ", vb->data[vb->count-1]);

            }

            uint32 compid = objid * stride;
            uint32 attribs_count = (objid+1)*stride;
            for(; compid < attribs_count; compid++) {

                // temporary: set color per vertex
                if(compid >= attribs_count-4) {
                    vb->data[vb->count++] = gfx_color_array[color_cnt++]; compid++;print("%4.1f, ", vb->count-1, vb->data[vb->count-1]);
                    vb->data[vb->count++] = gfx_color_array[color_cnt++]; compid++;print("%4.1f, ", vb->count-1, vb->data[vb->count-1]);
                    vb->data[vb->count++] = gfx_color_array[color_cnt++]; compid++;print("%4.1f, ", vb->count-1, vb->data[vb->count-1]);
                    vb->data[vb->count++] = gfx_color_array[color_cnt++]; compid;  print("%4.1f, ", vb->count-1, vb->data[vb->count-1]);
                    if(color_cnt >= COLORS_COUNT) color_cnt = 0;
                }
                else {
                    vb->data[vb->count++] = obj_attr_data[compid];
                    print("%4.1f, ", vb->count-1, vb->data[vb->count-1]);
                }

                // set color per object
                // vb->data[vb->count++] = obj_attr_data[compid];
                // print("%4.1f, ", vb->count-1, vb->data[vb->count-1]);

            }
        }
    }
    print("%s\n\n", CLEAR);

    return vb->data;

}

void gfx_vertex_buffer_set_rotation(const build_t build_idx, const vbidx_t vbidx, vec2f anglvel)
{
    struct vertex_buffer_object* vb = &vertex_buffer_table[vbidx];
    uint8 offset = shader_attrib_get_offset(build_idx,  SID_ROTAT2_IDI);
    const uint32 start = 0; /// TODO: get the start of the mesh to update
    uint32 idx = start + offset;
    const uint32 stride = vb->stride;
    while(idx < vb->count){
        vb->data[idx]   = anglvel.x;
        vb->data[idx+1] = anglvel.y;
        idx += stride;
    }
  }

void gfx_vertex_buffer_set_wpos(const build_t build_idx, const vbidx_t vbidx, vec3f wpos)
{
    struct vertex_buffer_object* vb = &vertex_buffer_table[vbidx];
    uint8 offset = shader_attrib_get_offset(build_idx,  SID_WPOSI3_IDI);
    const uint32 start = 0; /// TODO: getthe start of the mesh to update
    uint32 idx = start + offset;
    const uint32 stride = vb->stride;
    while(idx < vb->count){
        vb->data[idx]   = wpos.x;
        vb->data[idx+1] = wpos.y;
        vb->data[idx+2] = -wpos.z;
        idx += stride;
    }
}
void gfx_vertex_buffer_add_wpos(const build_t build_idx, const vbidx_t vbidx, vec3f wpos)
{
    struct vertex_buffer_object* vb = &vertex_buffer_table[vbidx];
    uint8 offset = shader_attrib_get_offset(build_idx,  SID_WPOSI3_IDI);
    const uint32 start = 0; /// TODO: getthe start of the mesh to update
    uint32 idx = start + offset;
    const uint32 stride = vb->stride;
    while(idx < vb->count){
        vb->data[idx]   += wpos.x;
        vb->data[idx+1] += wpos.y;
        vb->data[idx+2] -= wpos.z;
        idx += stride;
    }
}
void gfx_vertex_buffer_set_scale(const build_t build_idx, const vbidx_t vbidx, vec2f scale)
{
    struct vertex_buffer_object* vb = &vertex_buffer_table[vbidx];

    uint8 id = shader_attrib_id_get(build_idx, SID_SCALE2_IDI);
    struct vertex_layout* vl = shader_descriptor_vertex_layout_get(build_idx, id);
    const uint32 start = 0; /// TODO: get the start of the mesh to update
    uint32 idx = start + vl->loc_offset;
    const uint32 stride = vb->stride;
    while(idx < vb->count){
        vb->data[idx]   = scale.x;
        vb->data[idx+1] = scale.y;
        idx += stride;
    }
}



/***************************************************************************************************
 * vertex index buffer object
 **************************************************************************************************/
struct vertex_index_buffer_object* vertex_index_buffer_get_table(void){}
uint32 vertex_index_buffer_get_table_count(void){}
struct vertex_index_buffer_object vertex_index_buffer_get_vbo(vbidx_t vbidx){}

uint32 vertex_index_buffer_get_vertex_count(vbidx_t vbidx){}

uint32 gfx_vertex_index_buffer_create(const struct gfx_sid sid, const uint32 size, const uint32 data_count, const uint32 stride){}
void gfx_vertex_index_buffer_delete(){}
void gfx_vertex_index_buffer_push_data(const vbidx_t vbidx, void* data){}
void gfx_vertex_index_buffer_update_data(const vbidx_t vbidx){}

void gfx_vertex_index_buffer_set_data(const vbidx_t vbidx, void* data){}