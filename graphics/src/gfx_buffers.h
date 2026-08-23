/*
 * Created by ckalo on 4/21/26.
 */
#ifndef GRAPHICS_TEST_OPENGL_BUFFERS_H
#define GRAPHICS_TEST_OPENGL_BUFFERS_H
#include "my_types.h"
#include "shader_descriptor.h"


/***************************************************************************************************
 * command buffer argument structs
 **************************************************************************************************/
struct vb_cmd_params {
    /// TODO: flags:{ FORCED_NEW | FORCED_USED | ANY }
    attrid_t shader_attribs_idx;
    attrid_t vao_attribs_idx;
    vaidx_t vaidx;
    vbidx_t vbidx;
    uint8 vpos_num_elems; // TODO: pass SID_VPOS_ID instead which is smaller value
};

struct ib_cmd_params {
    uint16 attribs_idx;
    ibidx_t ibidx;
    uint8 vpos_num_elems; // TODO: pass SID_VPOS_ID instead which is smaller value
};


/***************************************************************************************************
 * vertex buffer object
 **************************************************************************************************/
struct vertex_buffer_object {
    float* data;
    uint32 size;
    uint16 count;
    uint8 stride;
    vbidx_t idx;
    vbidx_t vbo;
    build_t build_idx;
    vbidx_t attrib_count; // number of enabled attributes
    struct attributes_layout v_layout[SHADER_INPUT_ENTRIES_MAX_COUNT]; /// TODO: create dynamic array for this buffer
};

uint32 vertex_buffer_get_table_count(void);

struct vertex_buffer_object* vertex_buffer_get_vbo(vbidx_t vbidx);

uint32 vertex_buffer_get_vertex_count(vbidx_t vbidx);

uint32 gfx_vertex_buffer_create(const uint32 data_size, const uint32 stride);

void gfx_vertex_buffer_delete();

void gfx_vertex_buffer_push_data(const vbidx_t vbidx, void* data);

void gfx_vertex_buffer_update_data(struct vb_cmd_params* params);

void gfx_vertex_buffer_set_data(struct vb_cmd_params* params);

float* gfx_vertex_buffer_set_data2(struct vb_cmd_params* params);

void gfx_vertex_buffer_set_rotation(const build_t build_idx, const vbidx_t vbidx, vec2f anglvel);

void gfx_vertex_buffer_set_wpos(const build_t build_idx, const vbidx_t vbidx, vec3f wpos);

void gfx_vertex_buffer_add_wpos(const build_t build_idx, const vbidx_t vbidx, vec3f wpos);

void gfx_vertex_buffer_set_scale(const build_t build_idx, const vbidx_t vbidx, vec2f scale);


/***************************************************************************************************
 * vertex array object
 **************************************************************************************************/
struct vertex_array_object {
    vbidx_t idx;
    vbidx_t vao;
    desc_t desc_idx;
    bool attrib_devisor;
    vbidx_t vbcount; /// the number of created vbos
    /// TODO: make dynamic buffer
    vbidx_t vbos[8]; /// all the vertex buffer indexes belong to this VAO
};

struct vertex_array_object* gfx_vertex_array_get_byidx(const vaidx_t vaidx);

uint32 gfx_vertex_array_create(const desc_t desc_idx);

void gfx_vertex_array_delete(void);


/***************************************************************************************************
 * vertex index buffer object
 **************************************************************************************************/
struct vertex_index_buffer_object {
    vind_t* data;
    uint32 size;
    uint32 last_index; // stores the index of the last object, in order for the next object to continue the indexing
    uint16 count;
    ibidx_t idx;
    ibidx_t ibo;
};

struct vertex_index_buffer_object* vertex_index_buffer_get_table(void);

uint32 vertex_index_buffer_get_table_count(void);

struct vertex_index_buffer_object* vertex_index_buffer_get_ib(const ibidx_t ibidx);

uint32 vertex_index_buffer_get_vertex_count(const ibidx_t ibidx);

uint32 gfx_vertex_index_buffer_create(const uint32 data_size);

void gfx_vertex_index_buffer_delete(uint32 n, ibidx_t* buffer);

void gfx_vertex_index_buffer_push_data(const ibidx_t ibidx, void* data);

void gfx_vertex_index_buffer_update_data(struct ib_cmd_params* params);

uint32* gfx_vertex_index_buffer_set_data(struct ib_cmd_params* params);

#endif //GRAPHICS_TEST_OPENGL_BUFFERS_H
