/*
 * Created by ckalo on 4/8/26.
 */

#include "gfx_buffers.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "arena_allocator.h"
#include "fdebug_test.h"
#include "mesh.h"
#include "my_macros.h"
#include "object.h"
#include "opengl.h"
#include "opengl_debug.h"
#include "shader_builder.h"
#include "attributes.h"


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

uint32 vertex_buffer_get_vertex_count(const vbidx_t vbidx){ return vertex_buffer_table[vbidx].count; }

uint32 gfx_vertex_buffer_create(const uint32 data_size, const uint32 stride)
{
    GLuint vbo;
    gl_gen_buffers(1, &vbo);

    const vbidx_t vbidx = vertex_buffer_table_count;
    vertex_buffer_table_count++;

    struct vertex_buffer_object* vb = &vertex_buffer_table[vbidx];
    vb->idx = vbidx;
    vb->vbo = vbo;
    // vb->data = aligned_alloc(1024, data_size);
    memid_t vb_inst_id = arena_allocator_create_new_instance(ROUND_UP(data_size, PAGE_SIZE)/PAGE_SIZE);
    vb->data = arena_allocator_alloc(vb_inst_id, data_size, sizeof(float));
    memset(vb->data, 0xBCD, data_size);
    vb->size = data_size;
    vb->count = 0;
    vb->stride = stride;

    return vbidx;
}
void   gfx_vertex_buffer_delete(const uint32 n, vbidx_t* buffer){ gl_delete_buffers(n, (GLint*)buffer); }
void   gfx_vertex_buffer_push_data(const vbidx_t vbidx, void* data){}
void   gfx_vertex_buffer_update_data(struct vb_cmd_params* params)
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


#define RGBA_WHITE   0.8f, 0.8f, 0.8f, 1.0f
#define RGBA_BLACK   0.1f, 0.1f, 0.1f, 1.0f
#define RGBA_GREY    0.4f, 0.4f, 0.4f, 1.0f
#define RGBA_BLUE    0.1f, 0.5f, 1.0f, 1.0f
#define RGBA_PURPLE  0.5f, 0.1f, 0.8f, 1.0f
#define RGBA_MAGENTA 0.7f, 0.1f, 0.5f, 1.0f
#define RGBA_RED     0.7f, 0.1f, 0.1f, 1.0f
#define RGBA_ORANGE  0.9f, 0.4f, 0.1f, 1.0f
#define RGBA_ORANGE2 0.9f, 0.6f, 0.1f, 1.0f
#define RGBA_GREEN   0.1f, 0.6f, 0.1f, 1.0f
#define RGBA_YELLOW  0.7f, 0.7f, 0.1f, 1.0f
#define RGBA_CYAN    0.1f, 0.8f, 0.8f, 1.0f
#define COLORS_COUNT 12
uint32 color_cnt = 0;
uint32 next_color = 0;
/* 1 color per vertex */
#define COLORS_PER_VERTEX_COUNT (COLORS_COUNT*4)
static float gfx_color_per_vertex[] = {
    RGBA_BLUE, RGBA_PURPLE, RGBA_MAGENTA, RGBA_RED, RGBA_ORANGE, RGBA_ORANGE2,
    RGBA_GREEN, RGBA_YELLOW, RGBA_CYAN, RGBA_BLACK, RGBA_WHITE, RGBA_GREY
};
/* 1 color per triangle */
#define COLORS_PER_TRIANGLE_COUNT (COLORS_COUNT*VERTICES_PER_TRI*4)
static float gfx_color_per_triangle[] = {
    RGBA_BLUE, RGBA_BLUE, RGBA_BLUE,
     RGBA_PURPLE, RGBA_PURPLE, RGBA_PURPLE,
     RGBA_MAGENTA, RGBA_MAGENTA, RGBA_MAGENTA,
     RGBA_RED, RGBA_RED, RGBA_RED,
     RGBA_ORANGE, RGBA_ORANGE, RGBA_ORANGE,
     RGBA_ORANGE2, RGBA_ORANGE2, RGBA_ORANGE2,
     RGBA_GREEN, RGBA_GREEN, RGBA_GREEN,
     RGBA_YELLOW, RGBA_YELLOW, RGBA_YELLOW,
     RGBA_CYAN, RGBA_CYAN, RGBA_CYAN,
     RGBA_BLACK, RGBA_BLACK, RGBA_BLACK,
     RGBA_WHITE, RGBA_WHITE, RGBA_WHITE,
     RGBA_GREY, RGBA_GREY, RGBA_GREY,
};
/* 1 color per pentagon */
#define COLORS_PER_PENTAGON_COUNT (COLORS_COUNT*VERTICES_PER_PENTAGON*4)
static float gfx_color_per_pentagon[COLORS_PER_PENTAGON_COUNT] = {
    /// Basic colors
    RGBA_BLUE, RGBA_BLUE, RGBA_BLUE, RGBA_BLUE, RGBA_BLUE, RGBA_BLUE, RGBA_BLUE, RGBA_BLUE, RGBA_BLUE,
    RGBA_PURPLE, RGBA_PURPLE, RGBA_PURPLE, RGBA_PURPLE, RGBA_PURPLE, RGBA_PURPLE, RGBA_PURPLE, RGBA_PURPLE, RGBA_PURPLE,
    RGBA_MAGENTA, RGBA_MAGENTA, RGBA_MAGENTA, RGBA_MAGENTA, RGBA_MAGENTA, RGBA_MAGENTA, RGBA_MAGENTA, RGBA_MAGENTA, RGBA_MAGENTA,
    RGBA_RED, RGBA_RED, RGBA_RED, RGBA_RED, RGBA_RED, RGBA_RED, RGBA_RED, RGBA_RED, RGBA_RED,
    RGBA_ORANGE, RGBA_ORANGE, RGBA_ORANGE, RGBA_ORANGE, RGBA_ORANGE, RGBA_ORANGE, RGBA_ORANGE, RGBA_ORANGE, RGBA_ORANGE,
    RGBA_ORANGE2, RGBA_ORANGE2, RGBA_ORANGE2, RGBA_ORANGE2, RGBA_ORANGE2, RGBA_ORANGE2, RGBA_ORANGE2, RGBA_ORANGE2, RGBA_ORANGE2,
    RGBA_GREEN, RGBA_GREEN, RGBA_GREEN, RGBA_GREEN, RGBA_GREEN, RGBA_GREEN, RGBA_GREEN, RGBA_GREEN, RGBA_GREEN,
    RGBA_YELLOW, RGBA_YELLOW, RGBA_YELLOW, RGBA_YELLOW, RGBA_YELLOW, RGBA_YELLOW, RGBA_YELLOW, RGBA_YELLOW, RGBA_YELLOW,
    RGBA_CYAN, RGBA_CYAN, RGBA_CYAN, RGBA_CYAN, RGBA_CYAN, RGBA_CYAN, RGBA_CYAN, RGBA_CYAN, RGBA_CYAN,
    RGBA_BLACK, RGBA_BLACK, RGBA_BLACK, RGBA_BLACK, RGBA_BLACK, RGBA_BLACK, RGBA_BLACK, RGBA_BLACK, RGBA_BLACK,
    RGBA_WHITE, RGBA_WHITE, RGBA_WHITE, RGBA_WHITE, RGBA_WHITE, RGBA_WHITE, RGBA_WHITE, RGBA_WHITE, RGBA_WHITE,
    RGBA_GREY, RGBA_GREY, RGBA_GREY, RGBA_GREY, RGBA_GREY, RGBA_GREY, RGBA_GREY, RGBA_GREY, RGBA_GREY

};

/// TODO: (SET VERTEX BUFFER)-(SET VERTEX BUFFER REGION)
/// TODO: eventualy this function will be implemented for "per vertex" and "per instance" attributes
/// If the rendering is indexed geometry, then we run for vertex count, else we run for index count
float* gfx_vertex_buffer_set_data(struct vb_cmd_params* params)
{
    struct vertex_buffer_object* vb = &vertex_buffer_table[params->vbidx];
    buffer_t* obj_group_ids = attribsGroupDescr_get_objects_group(params->attribs_idx);
    float*    obj_attr_data = attribsGroupDescr_get_attribs_data(params->attribs_idx);
    struct attribs_per_attrib_descriptor* attribs_descr = attribsGroupDescr_get_attribs_descr(params->attribs_idx);
    // attribs_descr[attribs_idx].attr_offset
    // attribs_descr[0].stride

    uint32 vcnt = 0;

    bool is_indexed = attribsGroupDescr_is_indexed_geometry(params->attribs_idx);
    vind_t vcount = 0;

    uint32 attribs_stride = vb->stride-params->vpos_num_elems; // attributess stride is the vb attrib stride minus the vertex position's number of elements
    uint32 attribs_offset = 0; // the next object's attributes in the buffer

    print("\n\n%s- ADD OBJ TO VB vb:%u -",  YELLOW, params->vbidx);
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_VERTEX_BUFFER, "\n\n- ADD OBJ TO VB vb:%u -", params->vbidx);

    for(uint32 i = 0; i < obj_group_ids->tail+1; i++) { // per object run

        color_cnt = 0; // reset colors for each object

        struct object* obj = object_get(((objid_t*)obj_group_ids->data)[i]);
        // struct object* obj = object_get(i);
        enum MESH_TYPE mesh_type = obj->mesh_type;

        /// get the object's mesh vertices buffer
        float (*vpos)[params->vpos_num_elems] = (float(*)[params->vpos_num_elems])mesh_get_mesh_vertices_from_type(mesh_type);
        /// get object's indices buffer
        vind_t* ind   = mesh_get_mesh_indices_from_type(mesh_type);

        /// TODO: remove conditional
        if(is_indexed)  vcount = obj->vertex_count;
        else            vcount = mesh_get_mesh_indices_count_from_type(mesh_type);

        obj->vb_start = vb->count; /// set the start index, of the object, in the vertex buffer
        vcnt = 0;


        print("\n%s id:%d, vcount:%u %s\n", YELLOW, obj->id, obj->vertex_count, CLEAR);

        for(uint32 j = 0; j < vcount; j++) { /// per vertex run

            uint32 vertex_index =  ind[vcnt];
            /// TODO: remove conditional
            if(is_indexed) vertex_index = vcnt;

            print("\n[%2u][%2u]: ", i, vcount);
            engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_VERTEX_BUFFER, "\n[%2u][%3u]: ", i, vcount);

            for(uint8 k = 0; k < params->vpos_num_elems; k++) {

                vb->data[vb->count++] = vpos[vertex_index][k];
                print("%5.2f, ", vb->data[vb->count-1]);
                engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_VERTEX_BUFFER, "%2.2f, ", vb->data[vb->count-1]);
            }
            vcnt++;


            for(uint32 k = attribs_offset; k < attribs_offset+attribs_stride; k++) {

                const attr_t attribs_idx = k;
                attribs_descr[attribs_idx].attr_offset;
                // attribs_descr[attribs_idx].shader_attrib_idx;

                vb->data[vb->count++] = obj_attr_data[k];
                print("%4.1f, ",vb->data[vb->count-1]);
                engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_VERTEX_BUFFER, "%2.2f, ", vb->data[vb->count-1]);
                //
                // if(k >= attribs_offset+attribs_stride-4) {
                //     if(is_indexed) {
                //         vb->data[vb->count++] = gfx_color_per_vertex[color_cnt++];
                //         if(color_cnt >= COLORS_PER_VERTEX_COUNT) color_cnt = 0;
                //     } else {
                //         vb->data[vb->count++] = gfx_color_per_triangle[color_cnt++];
                //         if(color_cnt >= COLORS_PER_TRIANGLE_COUNT) color_cnt = 0;
                //     }
                //
                //     print("%4.1f, ", vb->data[vb->count-1]);
                //     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_VERTEX_BUFFER, "%2.2f, ", vb->data[vb->count-1]);
                // }
                // else {
                //     vb->data[vb->count++] = obj_attr_data[k];
                //     print("%4.1f, ",vb->data[vb->count-1]);
                //     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_VERTEX_BUFFER, "%2.2f, ", vb->data[vb->count-1]);
                // }
            }
        }

        attribs_offset += attribs_stride; // go to the next objects start of attributes data
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
    struct shader_attribute_layout* vl = shader_descriptor_vertex_layout_get(build_idx, id);
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
struct vertex_index_buffer_object vertex_index_buffer_table[4];
uint32 index_buffer_table_count = 0;

struct vertex_index_buffer_object* vertex_index_buffer_get_table(void);

uint32 vertex_index_buffer_get_table_count(void){ return index_buffer_table_count; }

struct vertex_index_buffer_object* vertex_index_buffer_get_ib(const ibidx_t ibidx){ return &vertex_index_buffer_table[ibidx]; }

uint32 vertex_index_buffer_get_vertex_count(ibidx_t ibidx){ return vertex_index_buffer_table[ibidx].count; }

uint32 gfx_vertex_index_buffer_create(const uint32 data_size)
{
    GLuint ibo;
    gl_gen_buffers(1, &ibo);

    const ibidx_t ibidx = index_buffer_table_count;
    index_buffer_table_count++;

    struct vertex_index_buffer_object* ib = &vertex_index_buffer_table[ibidx];
    ib->idx = ibidx;
    ib->ibo = ibo;
    memid_t ib_inst_id = arena_allocator_create_new_instance(ROUND_UP(data_size, PAGE_SIZE)/PAGE_SIZE);
    ib->data = arena_allocator_alloc(ib_inst_id, data_size, sizeof(float));
    memset(ib->data, 0xBCD, data_size);
    ib->size = data_size;
    ib->count = 0;

    return ibidx;
}
void gfx_vertex_index_buffer_delete(uint32 n, ibidx_t* buffer){ gl_delete_buffers(n, (GLint*)buffer); }
void gfx_vertex_index_buffer_push_data(const ibidx_t ibidx, void* data){}
void gfx_vertex_index_buffer_update_data(struct ib_cmd_params* params)
{
    const ibidx_t ibo = vertex_index_buffer_table[params->ibidx].ibo;
    void* const data = vertex_index_buffer_table[params->ibidx].data;
    const uint32 data_size = vertex_index_buffer_table[params->ibidx].count;
    /// check if vertex buffer is large enough
    assert(data_size*SIZE_F32 <= vertex_index_buffer_table[params->ibidx].size);

    GL_CHECK();
    gl_bind_buffer(OPENGL_ELEMENT_ARRAY_BUFFER, ibo);
    GL_CHECK();

    gl_buffer_data(OPENGL_ELEMENT_ARRAY_BUFFER, data_size*SIZE_F32, data, OPENGL_STATIC_DRAW);
    GL_CHECK();

}
uint32* gfx_vertex_index_buffer_set_data(struct ib_cmd_params* params)
{

    struct vertex_index_buffer_object* ib = &vertex_index_buffer_table[params->ibidx];

    buffer_t* obj_group_ids = attribsGroupDescr_get_objects_group(params->attribs_idx);
    uint32 cnt = 0, ind_offset = 0, max_index = 0;

    // if(ib->count > 0) ind_offset = ib->last_index+1;
    ind_offset = ib->last_index;


    print("\n\n%s- ADD OBJ TO IB ib:%u -",  YELLOW, params->ibidx);
    // engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_VERTEX_BUFFER, "\n\n- ADD OBJ TO VB vb:%u -", params->vbidx);

    for(uint32 i = 0; i < obj_group_ids->tail+1; i++) {

        struct object* obj = object_get(((objid_t*)obj_group_ids->data)[i]);
        // struct object* obj = object_get(i);
        obj->ib_start = ib->count;

        print("\n%s id:%d, vcount:%u %s\n", YELLOW, obj->id, obj->vertex_count, CLEAR);

        enum MESH_TYPE mesh_type = obj->mesh_type;
        vind_t* ind   = mesh_get_mesh_indices_from_type(mesh_type);
        vind_t  count = mesh_get_mesh_indices_count_from_type(mesh_type);
        cnt = 0;

        for(uint32 j = 0; j < count; j++) {

            if((j%3)==0) print("\n");
            if(ind[cnt]+ind_offset > max_index) max_index = ind[cnt]+ind_offset;

            ib->data[ib->count++] = ind[cnt++] + ind_offset;
            print("%3u, ", ib->data[ib->count-1]);
            // engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_VERTEX_BUFFER, "%2u, ", ib->data[ib->count-1]);
        }

        ind_offset = max_index+1;

    }

    ib->last_index = ind_offset;

    print("\nind_offset:%u\n\n", ind_offset);

    // engine_fdebug_append_to_file_from_arena(ENGINE_FDEBUG_FILE_VERTEX_BUFFER);
    // engine_fdebug_flush_arena(ENGINE_FDEBUG_FILE_VERTEX_BUFFER);


    return ib->data;

} 