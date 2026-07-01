/*
 * Created by ckalo on 4/8/26.
 */

#include "gfx_buffers.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "fdebug_test.h"
#include "mesh.h"
#include "object.h"
#include "opengl.h"
#include "opengl_debug.h"
#include "shader_builder.h"
#include "object_components.h"


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

    return vaidx;
}
void gfx_vertex_array_delete(void){}



/***************************************************************************************************
 * vertex buffer
 **************************************************************************************************/
struct vertex_buffer_object vertex_buffer_table[4];
uint32 vertex_buffer_table_count = 0;

uint32 vertex_buffer_get_table_count(void){ return vertex_buffer_table_count; }
struct vertex_buffer_object* vertex_buffer_get_vbo(vbidx_t vbidx){ return &vertex_buffer_table[vbidx]; }

uint32 vertex_buffer_get_vertex_count(vbidx_t vbidx){ return vertex_buffer_table[vbidx].count; }

uint32 gfx_vertex_buffer_create(const uint32 data_size, const uint32 stride)
{
    GLuint vbo;
    gl_gen_buffers(1, &vbo);

    const vbidx_t vbidx = vertex_buffer_table_count;
    vertex_buffer_table_count++;

    struct vertex_buffer_object* vb = &vertex_buffer_table[vbidx];
    vb->idx = vbidx;
    vb->vbo = vbo;
    vb->data = aligned_alloc(1024, data_size);
    memset(vb->data, 0xCD, data_size);
    vb->size = data_size;
    vb->count = 0;
    vb->stride = stride;

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

void HHH(enum ENGINE_FDEBUG_FILE_NAMES type, const char* format, ...)
{

}

/// TODO: (SET VERTEX BUFFER)-(SET VERTEX BUFFER REGION)
/// TODO: eventualy this function will be implemented for "per vertex" and "per instance" attributes
float* gfx_vertex_buffer_set_data(struct vb_cmd_params* params)
{

    struct vertex_buffer_object* vb = &vertex_buffer_table[params->vbidx];

    struct comp_per_group_descriptor* comp_desc = obj_component_get_comp_desc(params->vbidx);
    float* obj_attr_data = obj_component_get_obj_attribs_data(params->comp_idx);
    uint32 cnt1 = 0, vcnt = 0;

    uint32 stride = vb->stride-params->vpos_num_elems; // stride becomes minus the vertex position's number of elements
    uint32 comp_offset = 0; // the next object's components/attributes in the "obj_attr_data"

    print("\n\n%s- ADD OBJ TO VB vb:%u -",  YELLOW, params->vbidx);
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_VERTEX_BUFFER, "\n\n- ADD OBJ TO VB vb:%u -", params->vbidx);

    for(uint32 i = 0; i < comp_desc->per_obj_desc_counter; i++) {
        enum MESH_TYPE mesh_type = comp_desc->per_obj_desc[i].mesh_type;
        // enum MESH_TYPE mesh_type = object_get_vertex_mesh_type(i+1);
        float* vpos = mesh_get_mesh_from_type(mesh_type);
        uint16 vpos_count = comp_desc->per_obj_desc[i].vertex_count * params->vpos_num_elems;
        cnt1 = 0;

        for(uint32 j = 0; j < comp_desc->per_obj_desc[i].vertex_count; j++) {

            print("\n[%2u][%2u]: ", i, vcnt++);
            engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_VERTEX_BUFFER, "\n[%2u][%3u]: ", i, vcnt-1);

            for(uint8 k = 0; k < params->vpos_num_elems; k++) {
                if(cnt1 >= vpos_count) cnt1 = 0;
                vb->data[vb->count++] = vpos[cnt1++];
                print("%5.2f, ", vb->data[vb->count-1]);
                engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_VERTEX_BUFFER, "%2.2f, ", vb->data[vb->count-1]);

            }

            for(uint32 k = comp_offset; k < comp_offset+stride; k++) {

                if(k >= comp_offset+stride-4) {
                    vb->data[vb->count++] = gfx_color_array[color_cnt++];
                    print("%4.1f, ", vb->data[vb->count-1]);
                    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_VERTEX_BUFFER, "%2.2f, ", vb->data[vb->count-1]);
                    if(color_cnt >= COLORS_COUNT) color_cnt = 0;
                }
                else {
                    vb->data[vb->count++] = obj_attr_data[k];
                    print("%4.1f, ",vb->data[vb->count-1]);
                    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_VERTEX_BUFFER, "%2.2f, ", vb->data[vb->count-1]);
                }
            }
        }

        comp_offset += stride; // go to the next objects start of component's data
    }
    print("%s\n\n", CLEAR);

    engine_fdebug_append_to_file_from_arena(ENGINE_FDEBUG_FILE_VERTEX_BUFFER);
    engine_fdebug_flush_arena(ENGINE_FDEBUG_FILE_VERTEX_BUFFER);


    return vb->data;

}

void gfx_vertex_buffer_set_rotation(const build_t build_idx, const vbidx_t vbidx, vec2f anglvel)
{
    struct vertex_buffer_object* vb = &vertex_buffer_table[vbidx];
    uint8 offset = shader_attrib_get_offset(build_idx,  SID_ROTAT2_ID);
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
    uint8 offset = shader_attrib_get_offset(build_idx,  SID_WPOSI3_ID);
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
    uint8 offset = shader_attrib_get_offset(build_idx,  SID_WPOSI3_ID);
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

    uint8 id = shader_attrib_id_get(build_idx, SID_SCALE2_ID);
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



