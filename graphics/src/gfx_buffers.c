/*
 * Created by ckalo on 4/8/26.
 */

#include "gfx_buffers.h"

#include <stdlib.h>

#include "opengl.h"


struct vertex_buffer vertex_buffer_table[4];
uint32 vertex_buffer_table_count = 0;

struct vertex_buffer* vertex_buffer_get_table(){ return vertex_buffer_table; }
uint32 vertex_buffer_get_table_count(){ return vertex_buffer_table_count; }
vbidx_t vertex_buffer_get_vbo(vbidx_t vbidx){ return vertex_buffer_table[vbidx].vbidx; }

uint32 gfx_vertex_buffer_create(const struct gfx_sid sid, const uint32 data_size)
{
    GLuint vbo;
    gl_gen_buffers(1, &vbo);

    const vbidx_t vbidx = vertex_buffer_table_count;
    vertex_buffer_table_count++;

    struct vertex_buffer* vb = &vertex_buffer_table[vbidx];
    vb->vbidx = vbidx;
    vb->vbo = vbo;
    vb->sid = sid;
    vb->data = malloc(sizeof(float) * data_size);
    vb->data_size = data_size;

    /// create VAO: check if
    /// Shader_attributes_enable(attribsidx, attrib_divisor, vbinfo.attribs_info, vbidx);

    return vbidx;
}
void gfx_vertex_buffer_delete(uint32 n, vbidx_t* buffer){ gl_delete_buffers(n, buffer); }
void gfx_vertex_buffer_push_data(const vbidx_t vbidx, void* data){}
void gfx_vertex_buffer_update_data(const vbidx_t vbidx)
{
    const vbidx_t vbo = vertex_buffer_table[vbidx].vbo;
    void* const data = vertex_buffer_table[vbidx].data;
    const vbidx_t data_size = vertex_buffer_table[vbidx].data_size;
    gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo);
    gl_buffer_data(OPENGL_ARRAY_BUFFER, data_size, data, OPENGL_STATIC_DRAW);

}

void gfx_vertex_buffer_set_data(const vbidx_t vbidx, void* data)
{
    float* float_data = (float*)data;
    struct vertex_buffer vb = vertex_buffer_table[vbidx];
    for (uint32 i = 0; i < 21; i++) {
        vb.data[i] = float_data[i];
    }
}


uint32 gfx_vertex_array_create(){}
void gfx_vertex_array_delete(){}