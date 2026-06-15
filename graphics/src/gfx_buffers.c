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
uint32 gfx_vertex_array_create(void)
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

    return vaidx;
}
void gfx_vertex_array_delete(void){}



/***************************************************************************************************
 * vertex buffer
 **************************************************************************************************/
struct vertex_buffer_object vertex_buffer_table[4];
uint32 vertex_buffer_table_count = 0;

struct vertex_buffer_object* vertex_buffer_get_table(void){ return vertex_buffer_table; }
uint32 vertex_buffer_get_table_count(void){ return vertex_buffer_table_count; }
struct vertex_buffer_object* vertex_buffer_get_vbo(vbidx_t vbidx){ return &vertex_buffer_table[vbidx]; }

uint32 vertex_buffer_get_vertex_count(vbidx_t vbidx){ return vertex_buffer_table[vbidx].count; }

uint32 gfx_vertex_buffer_create(const struct gfx_sid sid, const uint32 data_size, const uint32 stride)
{
    GLuint vbo;
    gl_gen_buffers(1, &vbo);

    const vbidx_t vbidx = vertex_buffer_table_count;
    vertex_buffer_table_count++;

    // const uint32 data_size = 4096; // TODO: temp size, use sparse array for dynamically grow vertex buffer

    struct vertex_buffer_object* vb = &vertex_buffer_table[vbidx];
    vb->idx = vbidx;
    vb->vbo = vbo;
    vb->sid = sid;
    vb->data = malloc(data_size);
    memset(vb->data, 0xCD, data_size);
    vb->size = data_size;
    vb->count = 0;
    vb->stride = stride;

    return vbidx;
}
void gfx_vertex_buffer_delete(uint32 n, vbidx_t* buffer){ gl_delete_buffers(n, (GLint*)buffer); }
void gfx_vertex_buffer_push_data(const vbidx_t vbidx, void* data){}
void gfx_vertex_buffer_update_data(const vbidx_t vbidx)
{
    const vbidx_t vbo = vertex_buffer_table[vbidx].vbo;
    void* const data = vertex_buffer_table[vbidx].data;
    const uint32 data_size = vertex_buffer_table[vbidx].count;
    gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo);
    GL_CHECK();
    gl_buffer_data(OPENGL_ARRAY_BUFFER, data_size*SIZE_F32, &data[0], OPENGL_STATIC_DRAW);
    GL_CHECK();

}


uint32 color_cnt = 0;
#define COLORS_COUNT 3*4
static float color_array[COLORS_COUNT] = {
    /// Basic colors
    0.1, 0.5, 1.0, 1.0,   /// BLUE
    0.9, 0.5, 0.1, 1.0,   ///
    0.6, 0.1, 0.8, 1.0,   ///
};
// #define COLORS_COUNT 16*4
// static float color_array[COLORS] = {
//     /// Basic colors
//     0.9, 0.0, 0.0, 1.0,   /// RED
//     0.0, 0.7, 0.0, 1.0,   /// GREEN
//     0.0, 0.0, 1.0, 1.0,   /// BLUE
//     0.9, 0.7, 0.0, 1.0,   /// YELLOW
//
//     /// Bright/light colors
//     1.0, 0.5, 0.0, 1.0,   /// ORANGE
//     1.0, 0.0, 1.0, 1.0,   /// MAGENTA
//     0.0, 1.0, 1.0, 1.0,   /// CYAN
//     0.8, 0.8, 0.8, 1.0,   /// LIGHT GRAY
//
//     /// Pastel/soft colors
//     1.0, 0.7, 0.8, 1.0,   /// PINK
//     0.7, 1.0, 0.6, 1.0,   /// LIGHT GREEN
//     0.6, 0.8, 1.0, 1.0,   /// LIGHT BLUE
//     1.0, 0.9, 0.6, 1.0,   /// LIGHT YELLOW / CREAM
//
//     /// Dark/rich colors
//     0.5, 0.0, 0.0, 1.0,   /// DARK RED
//     0.0, 0.5, 0.0, 1.0,   /// DARK GREEN
//     0.0, 0.0, 0.5, 1.0,   /// DARK BLUE
//     0.3, 0.2, 0.5, 1.0   /// PURPLE
// };

/// TODO: eventualy this function will be implemented for "per vertex" and "per instance" attributes
float* gfx_vertex_buffer_set_data(const vbidx_t vbidx, const uint32 verts_per_obj,
                                  const uint32 num_objects, const uint32 num_vpos,
                                  const uint32 vpos_count, float* vpos)
{
    float* obj_attr_data = test_get_obj_attribs_data(vbidx);
    struct vertex_buffer_object* vb = &vertex_buffer_table[vbidx];
    uint32 cnt1 = 0;

    uint32 stride = vb->stride - num_vpos; // stride becomes minus the vertex position's number of elements

    print("\n\n%s- ADD OBJ TO VB -",  YELLOW);
    for(uint32 objid = 0; objid < num_objects; objid++)
    {
        for(uint32 vertid = 0; vertid < verts_per_obj; vertid++)
        {
            if(vb->count % (stride+num_vpos) == 0) {
                print("\n[%2u][%3u]: ", vertid+1, vb->count);
            }

            for(uint8 j = 0; j < num_vpos; j++) {
                if(cnt1>=vpos_count) cnt1 = 0;
                vb->data[vb->count++] = vpos[cnt1++];
                // print("%4.1f, ", vb->count-1, vb->data[vb->count-1]);
                print("%4.1f, ", vb->data[vb->count-1]);

            }

            uint32 compid = objid * stride;
            uint32 attribs_count = (objid+1)*stride;
            for(; compid < attribs_count; compid++) {

                // temporary: set color per vertex
                if(compid >= attribs_count-4) {
                    vb->data[vb->count++] = color_array[color_cnt++]; compid++;print("%4.1f, ", vb->count-1, vb->data[vb->count-1]);
                    vb->data[vb->count++] = color_array[color_cnt++]; compid++;print("%4.1f, ", vb->count-1, vb->data[vb->count-1]);
                    vb->data[vb->count++] = color_array[color_cnt++]; compid++;print("%4.1f, ", vb->count-1, vb->data[vb->count-1]);
                    vb->data[vb->count++] = color_array[color_cnt++]; compid;  print("%4.1f, ", vb->count-1, vb->data[vb->count-1]);
                    if(color_cnt >= COLORS_COUNT) color_cnt = 0;
                }
                else {
                    vb->data[vb->count++] = obj_attr_data[compid];
                    print("%4.1f, ", vb->count-1, vb->data[vb->count-1]);
                }

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