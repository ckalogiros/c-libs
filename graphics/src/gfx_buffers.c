/*
 * Created by ckalo on 4/8/26.
 */

#include "gfx_buffers.h"

#include <math.h>
#include <stdlib.h>

#include "opengl.h"
#include "opengl_debug.h"




/***************************************************************************************************
 * vertex array
 **************************************************************************************************/
struct vertex_array_object vertex_array_table[4];
uint32 vertex_array_table_count = 0;
struct vertex_array_object gfx_vertex_array_get_byidx(uint32 idx){ return vertex_array_table[idx]; }
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
struct vertex_buffer_object vertex_buffer_get_vbo(vbidx_t vbidx){ return vertex_buffer_table[vbidx]; }

uint32 vertex_buffer_get_vertex_count(vbidx_t vbidx){ return vertex_buffer_table[vbidx].count; }

uint32 gfx_vertex_buffer_create(const struct gfx_sid sid, const uint32 data_size, const uint32 data_count, const uint32 stride)
{
    GLuint vbo;
    gl_gen_buffers(1, &vbo);

    const vbidx_t vbidx = vertex_buffer_table_count;
    vertex_buffer_table_count++;

    struct vertex_buffer_object* vb = &vertex_buffer_table[vbidx];
    vb->idx = vbidx;
    vb->vbo = vbo;
    vb->sid = sid;
    // vb->data = malloc(sizeof(float) * data_size);
    vb->data = malloc(data_size);
    vb->size = data_size;
    vb->count = data_count;
    vb->stride = stride;

    return vbidx;
}
void gfx_vertex_buffer_delete(uint32 n, vbidx_t* buffer){ gl_delete_buffers(n, buffer); }
void gfx_vertex_buffer_push_data(const vbidx_t vbidx, void* data){}
void gfx_vertex_buffer_update_data(const vbidx_t vbidx)
{
    const vbidx_t vbo = vertex_buffer_table[vbidx].vbo;
    void* const data = vertex_buffer_table[vbidx].data;
    const vbidx_t data_size = vertex_buffer_table[vbidx].size;
    gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo);
    GL_CHECK();
    gl_buffer_data(OPENGL_ARRAY_BUFFER, data_size*SIZE_F32, data, OPENGL_STATIC_DRAW);
    GL_CHECK();

}

void gfx_vertex_buffer_set_data(const vbidx_t vbidx, void* data)
{
    float* float_data = (float*)data;
    struct vertex_buffer_object* vb = &vertex_buffer_table[vbidx];
    for (uint32 i = 0; i < vb->count; i++) {
        vb->data[i] = float_data[i];
        print("vbdata[%u]:%f\n", i, vb->data[i]);
    }
}

void gfx_vertex_buffer_set_rotation(const vbidx_t vbidx, vec2f anglvel)
{
    struct vertex_buffer_object* vb = &vertex_buffer_table[vbidx];
    uint32 idx = 2;
    const uint32 stride = vb->stride;
    while(idx < vb->count){
        vb->data[idx] = anglvel.x;
        // print("vbdata[%u]:%f\n", idx-1, vb->data[idx-1]);
        vb->data[idx+1] = anglvel.y;
        // print("vbdata[%u]:%f\n", idx-1, vb->data[idx-1]);
        idx += stride;
    }
    // print("\n");
}

void gfx_vertex_buffer_set_wpos(const progidx_t progidx, const vbidx_t vbidx, vec3f wpos)
{
    struct vertex_buffer_object* vb = &vertex_buffer_table[vbidx];

    struct vertex_layout* vl = shader_attributes_desc_get_byidx(SID_WPOSI3_IDI);
    uint32 idx = 4;
    const uint32 stride = vb->stride;
    while(idx < vb->count){
        vb->data[idx] = wpos.x;
        // print("vbdata[%u]:%f\n", idx-1, vb->data[idx-1]);
        vb->data[idx+1] = wpos.y;
        // print("vbdata[%u]:%f\n", idx-1, vb->data[idx-1]);
        vb->data[idx+2] = wpos.z;
        idx += stride;
    }
    // print("\n");
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