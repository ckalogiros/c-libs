/*
 * Created by ckalo on 4/21/26.
 */
#ifndef GRAPHICS_TEST_OPENGL_BUFFERS_H
#define GRAPHICS_TEST_OPENGL_BUFFERS_H
#include "my_types.h"
#include "gfx_interface.h"
#include "shader_id.h"

struct vertex_buffer {
    struct gfx_sid sid;
    // progidx_t progidx;
    vbidx_t vbidx;
    vbidx_t vbo;
    float* data;
    uint32 data_size;
};

struct vertex_buffer* vertex_buffer_get_table();
uint32 vertex_buffer_get_table_count();
vbidx_t vertex_buffer_get_vbo(vbidx_t vbidx);

uint32 gfx_vertex_buffer_create(const struct gfx_sid sid, const uint32 size);
void gfx_vertex_buffer_delete();
void gfx_vertex_buffer_push_data(const vbidx_t vbidx, void* data);
void gfx_vertex_buffer_update_data(const vbidx_t vbidx);

void gfx_vertex_buffer_set_data(const vbidx_t vbidx, void* data);

uint32 gfx_vertex_array_create();
void gfx_vertex_array_delete();

#endif //GRAPHICS_TEST_OPENGL_BUFFERS_H