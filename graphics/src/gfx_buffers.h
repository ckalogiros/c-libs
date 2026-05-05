/*
 * Created by ckalo on 4/21/26.
 */
#ifndef GRAPHICS_TEST_OPENGL_BUFFERS_H
#define GRAPHICS_TEST_OPENGL_BUFFERS_H
#include "my_types.h"
#include "gfx_interface.h"
#include "shader_id.h"


/***************************************************************************************************
 * vertex array object
 **************************************************************************************************/
struct vertex_array_object {
    vbidx_t idx;
    vbidx_t vao;
};

struct vertex_array_object gfx_vertex_array_get_byidx(uint32 idx);

uint32 gfx_vertex_array_create(void);

void gfx_vertex_array_delete(void);


/***************************************************************************************************
 * vertex buffer object
 **************************************************************************************************/
struct vertex_buffer_object {
    struct gfx_sid sid;
    vbidx_t idx;
    vbidx_t vbo;
    float* data;
    uint32 size;
    uint16 count;
    uint8 stride;
};

struct vertex_buffer_object* vertex_buffer_get_table(void);

uint32 vertex_buffer_get_table_count(void);

struct vertex_buffer_object vertex_buffer_get_vbo(vbidx_t vbidx);

uint32 vertex_buffer_get_vertex_count(vbidx_t vbidx);

uint32 gfx_vertex_buffer_create(const struct gfx_sid sid, const uint32 size, const uint32 data_count, const uint32 stride);

void gfx_vertex_buffer_delete();

void gfx_vertex_buffer_push_data(const vbidx_t vbidx, void* data);

void gfx_vertex_buffer_update_data(const vbidx_t vbidx);

void gfx_vertex_buffer_set_data(const vbidx_t vbidx, void* data);

void gfx_vertex_buffer_set_rotation(const vbidx_t vbidx, vec2f anglvel);

void gfx_vertex_buffer_set_wpos(const progidx_t progidx, const vbidx_t vbidx, vec3f wpos);


/***************************************************************************************************
 * vertex index buffer object
 **************************************************************************************************/
struct vertex_index_buffer_object {
    struct gfx_sid sid;
    vbidx_t idx;
    vbidx_t vbo;
    float* data;
    uint32 size;
    uint16 count;
    uint8 stride;
};

struct vertex_index_buffer_object* vertex_index_buffer_get_table(void);

uint32 vertex_index_buffer_get_table_count(void);

struct vertex_index_buffer_object vertex_index_buffer_get_vbo(vbidx_t vbidx);

uint32 vertex_index_buffer_get_vertex_count(vbidx_t vbidx);

uint32 gfx_vertex_index_buffer_create(const struct gfx_sid sid, const uint32 size, const uint32 data_count, const uint32 stride);

void gfx_vertex_index_buffer_delete();

void gfx_vertex_index_buffer_push_data(const vbidx_t vbidx, void* data);

void gfx_vertex_index_buffer_update_data(const vbidx_t vbidx);

void gfx_vertex_index_buffer_set_data(const vbidx_t vbidx, void* data);
#endif //GRAPHICS_TEST_OPENGL_BUFFERS_H
