#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arena_allocator.h"
#include "my_macros.h"
#include "str_utils.h"
#include "camera.h"
#include "gfx_buffers.h"
#include "command.h"
#include "fdebug_test.h"
#include "mem_man_interface.h"
#include "mesh.h"
#include "object.h"
#include "opengl.h"
#include "opengl_debug.h"
#include "precision_timer.h"
#include "run.h"
#include "scene.h"
#include "shader_builder.h"
#include "buffer_arr_internals.h"
#include "string_format.h"
#include "attributes.h"
#include "bubble_sort.h"

#include "test_custom_rendering.h"


#   define COLOR_RED     1.0f, 0.3f, 0.3f, 1.0f
#   define COLOR_GREEN   0.3f, 1.0f, 0.3f, 1.0f
#   define COLOR_BLUE    0.0f, 0.3f, 1.0f, 1.0f
#   define COLOR_CYAN    0.0f, 0.8f, 1.0f, 1.0f
#   define COLOR_YELLOW  1.0f, 1.0f, 0.3f, 1.0f
#   define COLOR_PURPLE0 1.0f, 0.0f, 0.6f, 1.0f
#   define COLOR_PURPLE1 0.6f, 0.0f, 1.0f, 1.0f
#   define COLOR_WHITE   1.0f, 1.0f, 1.0f, 1.0f
#   define COLOR_BLACK   0.0f, 0.0f, 0.0f, 1.0f
#   define COLOR_GREY0   0.1f, 0.1f, 0.1f, 1.0f
#   define COLOR_GREY1   0.2f, 0.2f, 0.2f, 1.0f
#   define COLOR_GREY2   0.5f, 0.5f, 0.5f, 1.0f
#   define COLOR_GREY3   0.7f, 0.7f, 0.7f, 1.0f


// static const char* fragment_shader_text =
//         "#version 330\n"
//         "in vec4 out_color4;\n"
//         // "in vec3 out_color3;\n"
//         "out vec4 fragment;\n"
//         "void main()\n"
//         "{\n"
//         // "    fragment = out_color4;\n"
//         // "    fragment = vec4(1.0, 0.0, 0.0, 1.0);\n"
//         // "    fragment = vec4(out_color3, 1.0);\n"
//         "    vec2 screenSize = vec2(600.0, 400.0);\n"
//         "    vec2 pos = (gl_FragCoord.xy / screenSize) * 2.0 - 1.0;\n"
//         // "    float dist = length( max((abs_uv - dim*scale) + r*scale + .0, 0.0) ) - r*scale;\n"
//         "    vec3 final_color = vec3(out_color4.r*pos.x, out_color4.g*pos.x, out_color4.b);\n"
//         "    fragment = vec4(final_color, 1.0);\n"
//
//         // "    float depth = gl_FragCoord.z*gl_FragCoord.z*2.;\n"
//         // "    fragment = vec4(depth, depth, depth, 1.0);\n"
//         // // "    fragment *= vec4(depth, depth, depth, 1.0);\n"
//         "}\n";
static const char* fragment_shader_text = "\
        #version 330\n\
        in vec2 out_vpos;\n\
        in vec4 out_color4;\n\
        out vec4 fragment;\n\
\n\
        void main()\n\
        {\n\
            vec2 screen_size = vec2(1000.0, 1000.0);\n\
            vec2 uv = (gl_FragCoord.xy / screen_size) * .5 + .5;\n\
            vec2 p = (abs(out_vpos)/screen_size) * .5 + .5;\n\
            float d = length( (p - uv) ) * 2.;\n\
\n\
            //vec3 final_color = vec3(out_color4.rgb*d);\n\
            vec3 final_color = vec3(out_color4.rgb);\n\
            fragment = vec4(final_color, 1.0);\n\
\n\
            //float depth = gl_FragCoord.z/ 2.;\n\
            //fragment = vec4(depth, depth, depth, 1.0);\n\
            //fragment *= vec4(final_color * depth, 1.0);\n\
        }\n";







/**************************************************************************************************
*
* The difference of using multiple VAOs
* 1. The shader program can "dismiss" any unused attribute input, if it is not used
* 2. The attribute devisor can be different. Same shader for instanced geometry VBOs
* 3. VBO seperation
*
* SAME FOR EBO/IBO
*
* AND we only need to bind VAO in the main loop.
* We only need to bind VBO for any vb update
*
* Prerequisites for creating NEW VAO
* 1. different attribute devisor
* 2. different SID flag
*
* CORRECT ORDER
*
* create shader program
*
* create VAO
* bind VAO
*
* create VBO
* bind VBO
*
* enable attributes (vao and vbo must be bound before)
*
* add to renderqueue
*
* vb set data and update data(once)
*
*
***************************************************************************************************/
struct cmd_gfx_params {
    struct camera camera;
    const char* unif_name;
    const float* unif_data;
    sididx_t sid_idx;
    attr_t   shader_attribs_idx;
    attrid_t vao_attribs_idx;
    build_t  build_idx;
    desc_t   desc_idx;
    uint8    vpos_num_elems;
};
build_t command_create_shader_program(struct cmd_gfx_params* params)
{
    build_t build_idx = gfx_program_exists(params->sid_idx);
    if(build_idx == INT_NULL) { // If shader program does not exist, create new program.
        /// TODO: Maybe create program returns the build_idx and not the decriptor object
        // gfx_create_shader_program(params->sid_idx, params->build_idx);
        opengl_program_create(params->sid_idx, params->build_idx, fragment_shader_text);
        GL_CHECK();
    }
}

void command_set_camera(struct cmd_gfx_params* params)
{
    struct opengl_program* prog = shader_program_get(params->build_idx);
    gl_use_program(prog->program);
    GLint mvp_location = gl_get_uniform_location(prog->program, "projMat");
    struct camera camera = params->camera;
    glUniformMatrix4fv(mvp_location, 1, camera.is_column_major, (float*)camera.projection_matrix.projection_matrix);
    GL_CHECK();
    gl_use_program(0x0);
}
// void command_set_triangle_mesh(struct cmd_gfx_params* params)
// {
//     struct opengl_program* prog = shader_program_get(params->build_idx);
//     gl_use_program(prog->program);
//     GLint location = gl_get_uniform_location(prog->program, params->unif_name);
//     glUniform2fv(location, 1, (float*)params->unif_data);
//     GL_CHECK();
//     gl_use_program(0x0);
// }

void command_create_vao(struct cmd_gfx_params* params)
{
    struct attributes_group* attribs_group = attribsGroupDescr_get_attribs_group(params->shader_attribs_idx);
    struct shader_descriptor* desc = shader_descriptor_get(params->desc_idx);
    struct attribs_per_attrib_descriptor* vao_descr = attribsGroupDescr_get_attribs_descr(params->vao_attribs_idx);
    const uint32 stride = desc->vshader_attribs_stride;
    struct opengl_program* prog = shader_program_get(params->desc_idx);
    gl_use_program(prog->program);

    /* VERTEX ARRAY */
    const bool attrib_devisor = 0; /// TODO: attrib devisor should exist in object OR object component OR shader descriptor???
    bool is_new_vao = FALSE;
    vaidx_t vaoidx = gfx_vertex_array_find_match(desc->sid_idx, attrib_devisor);
    if(vaoidx == INT_NULL) {
        vaoidx = gfx_vertex_array_create(desc->build_idx);
        is_new_vao = TRUE;
    }
    desc->vaoidx = vaoidx;


    struct vertex_array_object* va = gfx_vertex_array_get_byidx(vaoidx);
    gl_bind_vertex_array(va->vao);                                                      GL_CHECK();


    /* VERTEX BUFFER */
    vbidx_t vbidx = INT_NULL;
    struct vertex_buffer_object* vb = NULL;
    if(is_new_vao){

        /*
         * if:
         *      interleaved and !instanced
         *          create 1 vb
         *      interleaved and instanced
         *          create 2 or more vbs
         *      !interleaved
         *          create 2 or more vbs
         *
         * run
         *      for all attributes via the desc->vlayout
         *          for any new divisor: create vertex buffer, push to VAO and set the vertex buffer vertex layout
         *
         */

        struct vertex_array_object* vao = gfx_vertex_array_get_byidx(vaoidx);

        char buf[64] = {0}; char* ptr = &buf[0];
        uint32 size = string_format(&ptr, 64, "--- VAO:%2u ---\n", vao->idx);
        engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf, size);

        uint32 cnt = 0, vao_attr_desc_cnt = 0;
        attr_t divisor = INT_NULL;
        attr_t vb_stride = 0;
        for(uint32 i = 0; i < desc->entry_count; i++){

            if(desc->entries[i].input_flag & IS_ATTR) {

                if(divisor != desc->entries[i].divisor) {
// HERE: we need to pass the correct stride based on the attribs stored per vb
                    vbidx = gfx_vertex_buffer_create(20*3*stride*SIZE_F32, stride);
                    vao->vbos[cnt++] = vbidx;                                                       /// push new vertex buffer's index to VAO


                    vb = vertex_buffer_get_vbo(vbidx);
                    gl_bind_buffer(OPENGL_ARRAY_BUFFER, vb->vbo);                       GL_CHECK();

                    divisor = desc->entries[i].divisor;

                    char buf2[64] = {0}; ptr = &buf[0];
                    uint32 size2 = string_format(&ptr, 64, " VBO:%2u\n", vbidx);
                    engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf2, size2);

                    va->vbcount++;
                    vb_stride = 0;
                }

                vb->v_layout[vb->attrib_count].loc        = desc->v_layout[i].loc;
                vb->v_layout[vb->attrib_count].num_elems  = desc->v_layout[i].num_elems;
                vb->v_layout[vb->attrib_count].type       = desc->v_layout[i].type;
                vb->v_layout[vb->attrib_count].attr_idx   = desc->v_layout[i].attr_idx;             /// UNUSED for vertex buffers???
                vb->v_layout[vb->attrib_count].loc_offset = desc->v_layout[i].loc_offset;           /// UNUSED for vertex buffers???
                vb->v_layout[vb->attrib_count].offset     = vao_descr[vao_attr_desc_cnt].attr_offset;
                vb->v_layout[vb->attrib_count].idx        = desc->v_layout[i].idx;                  /// UNUSED for vertex buffers???
                vb->attrib_count++;
                vb_stride += desc->v_layout[i].num_elems;
                vb->stride = vb_stride;


                shader_attribute_enable(desc->v_layout[i].loc, desc->v_layout[i].num_elems,
                    stride, vao_descr[vao_attr_desc_cnt].attr_offset, SIZE_F32, divisor);

                char buf2[128] = {0}; ptr = &buf[0];
                uint32 size2 = string_format(&ptr, 128,
                    "[%2u]: "
                    "loc: %2u, "
                    "num_elems: %2u, "
                    "type: %s, "
                    "loc_offset: %2u, "
                    "offset: %2u, "
                    "\n",
                    desc->v_layout[i].idx,
                    desc->v_layout[i].loc,
                    desc->v_layout[i].num_elems,
                    shaderinfo_type_to_glsltype_string(desc->v_layout[i].type),
                    desc->v_layout[i].loc_offset,
                    vao_descr[vao_attr_desc_cnt].attr_offset
                );
                engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf2, size2);

                vao_attr_desc_cnt++;
            }



        }

        shader_attributes_enable(desc);
        opengl_debug_attributes_in(desc->vshader_attrib_locations_count);         GL_CHECK();
        attribs_group->rqidx = renderer_renderqueue_add(params->sid_idx, prog->program, desc->build_idx,
                                                        va->vao, va->idx, vb->vbo, vb->idx);
    } else {
        vbidx = gfx_vertex_buffer_find_match(attribs_group->sid_idx);
        vb = vertex_buffer_get_vbo(vbidx);
    }
    assert(vbidx != INT_NULL);



    /* VERTEX INDEX BUFFER */
    ibidx_t ibidx = INT_NULL;
    struct vertex_index_buffer_object* ib = NULL;
    if(is_new_vao){
        /// TODO: calculate the size of the vertex buffer OR use dynamic array vertex buffer
        ibidx = gfx_vertex_index_buffer_create(200*SIZE_I32);

        ib = vertex_index_buffer_get_ib(ibidx);
        gl_bind_buffer(OPENGL_ELEMENT_ARRAY_BUFFER, ib->ibo);
        GL_CHECK();

    } else {
        assert(0>0);
        // vbidx = gfx_vertex_buffer_find_match(attribs_desc->sid_idx);
        // vb = vertex_buffer_get_vbo(vbidx);
    }
    assert(ibidx != INT_NULL);

    /* SET COMMANDS */
    void* p = NULL;
    /// SET COMMAND: IB_SET_DATA
    struct ib_cmd_params ind_args = { .ibidx = ib->idx, .vpos_num_elems = params->vpos_num_elems, .attribs_idx = params->shader_attribs_idx};
    p = &ind_args;
    cmd_add_command((void*)gfx_vertex_index_buffer_set_data, p, sizeof(struct ib_cmd_params));
    // SET COMMAND: IB_UPDATE_DATA
    cmd_add_command((void*) gfx_vertex_index_buffer_update_data, p, sizeof(struct ib_cmd_params));


    /// SET COMMAND: VB_SET_DATA
    struct vb_cmd_params vert_args = { .vaidx = vaoidx, .vbidx = vb->idx, .vpos_num_elems = params->vpos_num_elems, .shader_attribs_idx = params->shader_attribs_idx};
    p = &vert_args;
    cmd_add_command((void*)gfx_vertex_buffer_set_data, p, sizeof(struct vb_cmd_params));
    // SET COMMAND: VB_UPDATE_DATA
    cmd_add_command((void*) gfx_vertex_buffer_update_data, p, sizeof(struct vb_cmd_params));

    gl_use_program(0x0);
    gl_bind_vertex_array(0x0);
    gl_bind_buffer(OPENGL_ARRAY_BUFFER, 0x0);


    /// close file for debug
    engine_fdebug_append_to_file_from_arena(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT);


}

struct object* test_create_triangles(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
{
    struct object* obj;
    scale[0]=1; scale[1]=1;
    for(uint32 i = 0; i < num_objects; i++) {
        obj = object_create_triangle((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
        wpos[0] += 3.f;
    }

    return obj;
}
//
// struct object* test_create_quads(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
// {
//     struct object* obj;
//     scale[0]=1; scale[1]=1;
//     for(uint32 i = 0; i < num_objects; i++) {
//         obj = object_create_rectangle((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
//         wpos[0] +=3.f;
//     }
//
//     return obj;
// }
//
// struct object* test_create_pentagon(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
// {
//     struct object* obj;
//     scale[0]=1; scale[1]=1;
//     for(uint32 i = 0; i < num_objects; i++) {
//         obj = object_create_pentagon((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
//         wpos[0] += 3.f;
//     }
//     return obj;
// }
//
// struct object* test_create_hexagon(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
// {
//     struct object* obj;
//     scale[0]=1; scale[1]=1;
//     wpos[2] += .01;
//     for(uint32 i = 0; i < num_objects; i++) {
//         obj = object_create_hexagon((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
//         wpos[0] += 3.f;
//     }
//     return obj;
// }
//
// struct object* test_create_octagon(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
// {
//     struct object* obj;
//     scale[0]=1; scale[1]=1;
//     for(uint32 i = 0; i < num_objects; i++) {
//         obj = object_create_octagon((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
//         wpos[0] += 3.f;
//     }
//     return obj;
// }
//
// struct object* test_create_cube(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
// {
//     struct object* obj;
//     scale[0]=1; scale[1]=1;
//     for(uint32 i = 0; i < num_objects; i++) {
//         obj = object_create_cube((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
//         wpos[0] -= 3.f;
//     }
//     return obj;
// }
//
// struct object* test_create_octahedron(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
// {
//     struct object* obj;
//     scale[0]=1; scale[1]=1;
//     for(uint32 i = 0; i < num_objects; i++) {
//         obj = object_create_octahedron((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
//         wpos[0] -= 3.f;
//     }
//     return obj;
// }
//
// struct object* test_create_dodecahedron(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
// {
//     struct object* obj;
//     scale[0]=1; scale[1]=1;
//     for(uint32 i = 0; i < num_objects; i++) {
//         obj = object_create_dodecahedron((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
//         wpos[0] -= 3.f;
//     }
//     return obj;
// }


void test_create_object(const attr_t shader_attribs_idx, const attr_t vao_attribs_idx, const objid_t obj_id, const uint32 num_objects, enum MESH_TYPE mesh_type,
                        struct camera* camera, const char* unif_name, const float* unif_data)
{
    struct attributes_group* attribs_group = attribsGroupDescr_get_attribs_group(shader_attribs_idx);

    struct cmd_gfx_params gfx_params = {
        .sid_idx = attribs_group->sid_idx,
        .shader_attribs_idx = shader_attribs_idx,
        .shader_attribs_idx = vao_attribs_idx,
        .build_idx = attribs_group->build_idx,
        .desc_idx = INT_NULL,
        .vpos_num_elems = mesh_get_num_elems_per_vertex(mesh_type),
        .camera = *camera,
        .unif_name = NULL,
        .unif_data = NULL
    };

    if((attribs_group->commands_flag & CMD_CREATE_SHADER_PROGRAM)==0) {
        cmd_add_command((void*)command_create_shader_program, &gfx_params, sizeof(struct cmd_gfx_params));
        attribs_group->commands_flag |= CMD_CREATE_SHADER_PROGRAM;
        gfx_params.desc_idx =  attribsGroupDescr_get_attribs_group(shader_attribs_idx)->build_idx;
    }


    if((attribs_group->commands_flag & CMD_CREATE_VAO)==0) {
        cmd_add_command((void*)command_create_vao, &gfx_params, sizeof(struct cmd_gfx_params));
        attribs_group->commands_flag |= CMD_CREATE_VAO;
    }


    // if((attribs_group->commands_flag & CMD_CREATE_UNIF_MESH)==0) {
    //     gfx_params.unif_name = unif_name;
    //     gfx_params.unif_data = unif_data;
    //     cmd_add_command((void*)command_set_triangle_mesh, &gfx_params, sizeof(struct cmd_gfx_params));
    //     attribs_group->commands_flag |= CMD_CREATE_UNIF_MESH;
    // }


    if((attribs_group->commands_flag & CMD_SET_CAMERA_PROJ)==0) {
        cmd_add_command((void*)command_set_camera, &gfx_params, sizeof(struct cmd_gfx_params));
        attribs_group->commands_flag |= CMD_SET_CAMERA_PROJ;
    }

    // struct command_render_queue_params rq_params = {.attribs_idx = attribs_idx, .vertex_count = num_objects*num_vertices};
    struct command_render_queue_params rq_params = {
        .attribs_idx  = shader_attribs_idx,
        .index_count  = num_objects * object_get_vertex_index_count(obj_id),
        .vertex_count = num_objects * object_get_vertex_count(obj_id)
    };
    cmd_add_command((void*) render_queue_add_draw_count, &rq_params, sizeof(struct command_render_queue_params));

}

void test_bubble_sort();



struct TEMP_attribs_per_attrib_descriptor {
    sid_t  attrib_flag;    /// flag of type: SID_... VPOS2, WPOS2, COL4, etc
    sid_t  shader_input_flags;    /// flag of type: SHADER_INPUT_FLAGS aka IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS, etc
    attr_t attr_offset;    /// the start offset of the attribute  in the attributes data buffer
    attr_t attr_num_elems; /// how many: floats | ints etc, for the attribute
    attr_t divisor;
};

struct attribs_per_attrib_descriptor;

struct object* tmp(struct gfx_sid sid, float* attribs, struct attribs_per_attrib_descriptor *desc,
    uint32 num_desc, uint32 num_attr_per_desc, uint32 num_attribs, uint32 stride, uint32 attribs_size)
{
    /* shared vertex positions but no divisor=1 VBO for per instance data
     *
        layout(location = 0) in vec3 aPos;  // VBO 0, divisor 0 (shared)

        layout(std430, binding = 0) buffer PerVertexData {
            vec2 uvs[];
            vec3 normals[];
        };

        layout(std430, binding = 1) buffer PerInstanceData {
            vec4 colors[];
            vec3 worldPositions[];
            vec3 scales[];
        };

        uniform mat4 viewProj;

        void main() {
            int vertexIndex = gl_InstanceID * 3 + gl_VertexID;
            int instanceIndex = gl_InstanceID;

            vec2 uv = uvs[vertexIndex];
            vec3 normal = normals[vertexIndex];
            vec4 color = colors[instanceIndex];
            vec3 worldPos = worldPositions[instanceIndex];
            vec3 scale = scales[instanceIndex];

            vec3 finalPos = worldPos + aPos * scale;
            gl_Position = viewProj * vec4(finalPos, 1.0);
        }
     */

    sid.attr |= SID_VPOSI2 | SID_SCALE2 | SID_COLOR4;
    sid.unif |= SID_PROJMAT4X4;
    sid.out  |= SID_COLOR4;
    sid.pass |= SID_COLOR4;
    sid.oper |= SID_NULL;

    enum MESH_TYPE mesh_type = MESH_TRIANGLE;

    sididx_t sid_idx = gfx_sid_find_match(sid);
    attrid_t attribs_group_id = INT_NULL;

    if(sid_idx != INT_NULL) { /// 1. attribute descriptor group exists
        attribs_group_id = attribsGroupDescr_group_find_match(sid_idx);
    } else {                  /// 2. create new attribute descriptor group
        sid_idx = gfx_create_sid(sid);
        attribs_group_id = attribsGroupDescr_create_new_attribs_group(sid_idx);
    }

    struct object* obj  = object_create(sid_idx, mesh_type);
    obj->num_faces      = 1;
    obj->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
    obj->index_count    = mesh_get_mesh_indices_count_from_type(mesh_type);
    obj->attribs_group_id = attribs_group_id;

    object_set_vertex_count(obj->id, obj->num_faces * obj->verts_per_face);
    attribsGroupDescr_create_new_obj(obj->attribs_group_id, obj->id, obj->num_faces * obj->verts_per_face);

    /// 1. find or create shader build
    build_t build_idx = shader_build_exists(sid_idx);
    if(build_idx == INT_NULL) build_idx = shader_builder_create_new_build(sid_idx);
    attribsGroupDescr_set_build_idx(attribs_group_id, build_idx);


    enum SHADER_INPUT_FLAGS attrib_flags = 0x0;
    enum SID_ID shader_input_id = 0;

    /**********************************************************************************************
     * ATTRIBUTES */
    // add vpos attribute
    attrib_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS;
    shader_input_id = SID_VPOSI2_ID;
    shader_create_input(build_idx, shader_input_id, attrib_flags, 0);


    /// sort all attributes by their divisor
    // bubble_sort_generic_fast((void*)desc, num_attribs, sizeof(struct attribs_per_attrib_descriptor), compare_struct_attribs_per_attrib_descriptor);

    /// store sorted attributes to a buffer for rendering
    struct attribs_per_attrib_descriptor* attribs_descr = attribsGroupDescr_get_attribs_descr(attribs_group_id);
    struct attributes_group* attribs_group = attribsGroupDescr_get_attribs_group(attribs_group_id);

    float* buffer_ptr = arena_allocator_alloc(attribs_group->attribs_data_id, attribs_size*SIZE_F32, sizeof(float));
    memcpy(buffer_ptr, attribs, attribs_size*SIZE_F32);

    // for(uint32 i = 0; i < num_attribs; i++) {
    for(uint32 i = 0; i < num_desc; i++) {

        struct attribs_per_attrib_descriptor* descr = desc;

        for(uint32 j = 0; j < num_attr_per_desc; j++) {

        }


        print("attr_offset: %u attr_num_elems: %u\n", attribs_descr[i].attr_offset, attribs_descr[i].attr_num_elems);
        const attr_t shader_input_id = shaderinfo_flag_to_id(desc[1].attrib_flag);
        struct attribs_per_attrib_descriptor* attribs_descr = attribsGroupDescr_get_attribs_descr(shader_input_id);
        shader_create_input(build_idx, shader_input_id, desc[i].shader_input_flags, desc[i].divisor);

        // attribsGroupDescr_add_attributes(attribs_group_id, temp_buffer[i].src, entry->elems, attribs_descr->attr_offset, entry->flag, temp_buffer[i].divisor, shader_input_id);
        if((attribs_group->added_components_flag & desc[i].attrib_flag) == 0x0) {
            const attr_t attrib_idx = attribs_group->attribs_count;
            assert(attrib_idx < COMP_MAX_ATTRIBUTES);
            attribs_group->added_components_flag                       |= desc[i].attrib_flag;
            attribs_group->attribs_descr[attrib_idx].attrib_flag        = desc[i].attrib_flag;
            attribs_group->attribs_descr[attrib_idx].attr_offset        = desc[i].attr_offset;
            attribs_group->attribs_descr[attrib_idx].attr_num_elems     = desc[i].attr_num_elems;
            attribs_group->attribs_descr[attrib_idx].divisor            = desc[i].divisor;
            attribs_group->attribs_descr[attrib_idx].shader_input_flags = desc[i].shader_input_flags;
            attribs_group->attribs_count++;
        }
    }
    // for(uint32 i = 0; i < num_attribs; i++) {
    //
    //     print("attr_offset: %u attr_num_elems: %u\n", attribs_descr[i].attr_offset, attribs_descr[i].attr_num_elems);
    //     const attr_t shader_input_id = shaderinfo_flag_to_id(desc[1][i].attrib_flag);
    //     struct attribs_per_attrib_descriptor* attribs_descr = attribsGroupDescr_get_attribs_descr(shader_input_id);
    //     shader_create_input(build_idx, shader_input_id, desc[i].shader_input_flags, desc[i].divisor);
    //
    //     // attribsGroupDescr_add_attributes(attribs_group_id, temp_buffer[i].src, entry->elems, attribs_descr->attr_offset, entry->flag, temp_buffer[i].divisor, shader_input_id);
    //     if((attribs_group->added_components_flag & desc[i].attrib_flag) == 0x0) {
    //         const attr_t attrib_idx = attribs_group->attribs_count;
    //         assert(attrib_idx < COMP_MAX_ATTRIBUTES);
    //         attribs_group->added_components_flag                       |= desc[i].attrib_flag;
    //         attribs_group->attribs_descr[attrib_idx].attrib_flag        = desc[i].attrib_flag;
    //         attribs_group->attribs_descr[attrib_idx].attr_offset        = desc[i].attr_offset;
    //         attribs_group->attribs_descr[attrib_idx].attr_num_elems     = desc[i].attr_num_elems;
    //         attribs_group->attribs_descr[attrib_idx].divisor            = desc[i].divisor;
    //         attribs_group->attribs_descr[attrib_idx].shader_input_flags = desc[i].shader_input_flags;
    //         attribs_group->attribs_count++;
    //     }
    // }
    print_struct_attribs_per_attrib_descriptor(attribs_descr, num_attribs);



    /***********************************************************************************************
     * UNIFORMS */
    /// add projection matrix uniform
    attrib_flags = IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ;
    shader_input_id = SID_PROJMAT4X4_ID;
    shader_create_input(build_idx, shader_input_id, attrib_flags, 0);

    attribsGroupDescr_set_stride(attribs_group_id, stride);

    print("%s- Create object: triangle -%s\n"
          "   id:%u\n   attribs_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->attribs_group_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");

    return obj;
}

int
main(void)
{
    memman_init();
    buffer_arr_init_table(128);
    shader_descriptor_init();
    shader_builder_init();
    timer_init_table();
    object_table_init();
    attribsGroupDescr_table_init();

    /* DEBUG-FPRINT */
    engine_fdebug_init();
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "- DEBUG FPRINT: INITIALIZED -\n");

    scene_init();

    struct scene* scene = scene_create("scene 1");

    /// create camera
    float aspect = (float)SCREEN_RES_X / (float)SCREEN_RES_Y;
    float near = 0.1f;
    float far = 100.0f;

    /// perspective
    float fov = 45.0f;                    // Field of view in degrees
    struct camera_projection_matrix projection_matrix =
        scene_create_perspective_projection_matrix(fov, near, far, aspect);

    /// orthographic
    // struct camera_projection_matrix projection_matrix =
    //     scene_create_orthographic_projection_matrix(0, SCREEN_RES_X, 0, SCREEN_RES_Y, near, far);



    struct float_number f = str_conv_double_to_int_parts(10.0918);

    struct camera camera = camera_create(projection_matrix, GL_FALSE);

    /// create layer
    struct layer layer1 = {
        .name = "layer1",
    };
    layer_create_layer(&layer1);
    scene_push_layer(scene, layer1);


    // glDisable(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glDisable(GL_CULL_FACE);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK); // Cull front faces instead of back
    // glCullFace(GL_FRONT); // Cull front faces instead of back
    // glFrontFace(GL_CW);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LEQUAL);
    glDepthFunc(GL_LESS);
    // glDepthMask(GL_TRUE);
    // glHint(GLenum target, GLenum mode = GL_FASTEST, GL_NICEST, and GL_DONT_CARE);
    // glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_MULTISAMPLE);



    gl_viewport(SCREEN_RES_X, SCREEN_RES_Y);
    opengl_debug_init();

    {
        // temp
        /*
         * create triangle()
         *      creates an object
         * add_attributes(
         *      VPOS_for_triangle_mesh,
         *      inst_col4, vec4[r,g,b,a],
         *      inst_col4, vec4[r,g,b,a],   // 2 colors for gradient
         *      inst_wpos3, vec3[x,y,z],
         *      inst_scale, vec2[x,y]
         * )
         *
         * Either user passes 1 array of attribs data(already sorted??)
         * Or calls an enable() function for each attribute?
         *
         * object group attributes buffer: mock
         *      obj id = 0
         *      mesh id = TRIANGLE MESH
         *      stride = 13
         *      [ [color4   ], [color4  ], [wpos3   ], [scale2  ] ]: instanced buffer
         *         offset=0    offset=4    offset=8    offset=11
         *         size=4      size=4      size=3      size=2
         *         divisor=1   divisor=1   divisor=1   divisor=1
         *         attributes layout[4]: offset, size
         *
         *      calculate start: (obj id * stride) + offset
         *
         *      equivalent vertex buffer:
         *      per vertex-vpos2: [x,y, x,y, x,y]
         *      instanced:        [r,g,b,a, r,g,b,a, x,y,z, x,y]
         *
         *
         *      ***************************************************************************
         *      Other potential examples
         *      ***************************************************************************
         *      color per vertex and the rest per instance
         *      [[r,g,b,a  ], [r,g,b,a  ], [r,g,b,a  ], [x,y,z    ], [x,y      ]]:
         *        divisor=0    divisor=0    divisor=0    divisor=1    divisor=1
         *
         *      equivalent vertex buffer:
         *      per vertex: [x,y, r,g,b,a, x,y, r,g,b,a, x,y, r,g,b,a]
         *      instanced : [x,y,z, x,y]
         *
         *      color per vertex and many instaned buffers
         *      [[r,g,b,a  ], [r,g,b,a  ], [r,g,b,a  ], [x,y,z    ], [x,y      ]]:
         *        divisor=0    divisor=0    divisor=0    divisor=1    divisor=2
         *
         *      equivalent vertex buffer:
         *      per vertex: [x,y, r,g,b,a, x,y, r,g,b,a, x,y, r,g,b,a]
         *      instanced : [x,y,z] // taken per 1 instance
         *      instanced : [x,y]   // taken per 2 instances
         */
        // struct attribs_per_attrib_descriptor desc[][4] = {
        //     [0][0] = {.attr_offset = 0 , .attr_num_elems = 2, .divisor = 0, .attrib_flag = SID_VPOSI2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS},
        //     [1][0] = {.attr_offset = 0 , .attr_num_elems = 4, .divisor = 0, .attrib_flag = SID_COLOR4, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_OUT},
        //     [1][1] = {.attr_offset = 8 , .attr_num_elems = 3, .divisor = 1, .attrib_flag = SID_WPOSI3, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS},
        //     [1][2] = {.attr_offset = 11, .attr_num_elems = 2, .divisor = 1, .attrib_flag = SID_SCALE2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE},
        //     [1][3] = {.attr_offset = 13, .attr_num_elems = 2, .divisor = 1, .attrib_flag = SID_ROTAT2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT}
        // };
        // float attribs[] = {
        //     /// color4           wpos3       scale2       rot2
        //     1.0,0.0,1.0,1.0,   0, 0,-10,   1.0,1.0,   cos(0.0),sin(0.0), // v0
        //     1.0,1.0,0.0,1.0,   0, 0,-10,   1.0,1.0,   cos(0.0),sin(0.0), // v1
        //     0.0,1.0,1.0,1.0,   0, 0,-10,   1.0,1.0,   cos(0.0),sin(0.0), // v2
        // };
        // struct object* tri = tmp((struct gfx_sid){0, 0, 0, 0, 0}, attribs, desc, 2, 4, 4+3+2+2, ARRAY_LENGTH(attribs) );
        // const attr_t vao_attribs_idx = 0;
        // test_create_object(tri->attribs_group_id, vao_attribs_idx, tri->id, 1, MESH_TRIANGLE, &camera, "vpos2", mesh_get_mesh_vertices_from_type(MESH_TRIANGLE));
    }
    { // temp

        // /// create attrib buffer:
        // struct attribs_per_attrib_descriptor desc[] = {
        //     {.attr_offset = 0 , .attr_num_elems = 2, .divisor = 0, .attrib_flag = SID_VPOSI2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS},
        //     {.attr_offset = 0 , .attr_num_elems = 4, .divisor = 0, .attrib_flag = SID_COLOR4, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_OUT},
        //     {.attr_offset = 8 , .attr_num_elems = 3, .divisor = 1, .attrib_flag = SID_WPOSI3, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS},
        //     {.attr_offset = 11, .attr_num_elems = 2, .divisor = 1, .attrib_flag = SID_SCALE2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE},
        //     {.attr_offset = 13, .attr_num_elems = 2, .divisor = 1, .attrib_flag = SID_ROTAT2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT}
        // };
        // float* triVert = mesh_get_mesh_vertices_from_type(MESH_TRIANGLE);
        // float attribs[] = {
        //     /// vpos2              color4             wpos3      scale2     rot2
        //     triVert[0],triVert[1], 1.0,0.0,1.0,1.0,   0,0,-10,   1.0,1.0,   cos(0.0),sin(0.0), // v0
        //     triVert[2],triVert[3], 1.0,1.0,0.0,1.0,   0,0,-10,   1.0,1.0,   cos(0.0),sin(0.0), // v1
        //     triVert[4],triVert[5], 0.0,1.0,1.0,1.0,   0,0,-10,   1.0,1.0,   cos(0.0),sin(0.0), // v2
        // };
        //
        // uint32 stride = 13;
        //
        // struct gfx_sid sid = {0,0,0,0,0};
        // sid.attr |= SID_VPOSI2 | SID_SCALE2 | SID_COLOR4;
        // sid.unif |= SID_PROJMAT4X4;
        // sid.out  |= SID_COLOR4;
        // sid.pass |= SID_COLOR4;
        // sid.oper |= SID_NULL;
        //
        // enum MESH_TYPE mesh_type = MESH_TRIANGLE;
        //
        // sididx_t sid_idx = gfx_sid_find_match(sid);
        // attrid_t attribs_group_id = INT_NULL;
        //
        // if(sid_idx != INT_NULL) { /// 1. attribute descriptor group exists
        //     attribs_group_id = attribsGroupDescr_group_find_match(sid_idx);
        // } else {                  /// 2. create new attribute descriptor group
        //     sid_idx = gfx_create_sid(sid);
        //     attribs_group_id = attribsGroupDescr_create_new_attribs_group(sid_idx);
        // }
        //
        // struct object* obj  = object_create(sid_idx, mesh_type);
        // obj->num_faces      = 1;
        // obj->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
        // obj->index_count    = mesh_get_mesh_indices_count_from_type(mesh_type);
        // obj->attribs_group_id = attribs_group_id;
        //
        // object_set_vertex_count(obj->id, obj->num_faces * obj->verts_per_face);
        // attribsGroupDescr_create_new_obj(obj->attribs_group_id, obj->id, obj->num_faces * obj->verts_per_face);
        //
        // /// 1. find or create shader build
        // build_t build_idx = shader_build_exists(sid_idx);
        // if(build_idx == INT_NULL) build_idx = shader_builder_create_new_build(sid_idx);
        // attribsGroupDescr_set_build_idx(attribs_group_id, build_idx);
        //
        //
        // enum SHADER_INPUT_FLAGS attrib_flags = 0x0;
        // enum SID_ID shader_input_id = 0;
        //
        // /**********************************************************************************************
        //  * ATTRIBUTES */
        // // add vpos attribute
        // attrib_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS;
        // shader_input_id = SID_VPOSI2_ID;
        // shader_create_input(build_idx, shader_input_id, attrib_flags, 0);
        //
        // /// sort all attributes by their divisor
        // // bubble_sort_generic_fast((void*)desc, num_attribs, sizeof(struct attribs_per_attrib_descriptor), compare_struct_attribs_per_attrib_descriptor);
        //
        // /// store sorted attributes to a buffer for rendering
        // // struct attribs_per_attrib_descriptor* attribs_descr = attribsGroupDescr_get_attribs_descr(attribs_group_id);
        // struct attributes_group* attribs_group = attribsGroupDescr_get_attribs_group(attribs_group_id);
        //
        // uint32 attrib_size = 3*stride*SIZE_F32;
        // float* buffer_ptr = arena_allocator_alloc(attribs_group->attribs_data_id, attrib_size, SIZE_F32);
        // memcpy(buffer_ptr, attribs, attrib_size);
        //
        // // for(uint32 i = 0; i < num_attribs; i++) {
        // for(uint32 i = 0; i < 2; i++) {
        //     // struct attribs_per_attrib_descriptor* descr = desc[i];
        //
        //     for(uint32 j = 0; j < 4; j++) {
        //
        //         if((attribs_group->added_components_flag & desc[i].attrib_flag) == 0x0) {
        //             const attr_t attrib_idx = attribs_group->attribs_count;
        //             assert(attrib_idx < COMP_MAX_ATTRIBUTES);
        //             attribs_group->added_components_flag                       |= desc[i].attrib_flag;
        //             attribs_group->attribs_descr[attrib_idx].attrib_flag        = desc[i].attrib_flag;
        //             attribs_group->attribs_descr[attrib_idx].attr_offset        = desc[i].attr_offset;
        //             attribs_group->attribs_descr[attrib_idx].attr_num_elems     = desc[i].attr_num_elems;
        //             attribs_group->attribs_descr[attrib_idx].divisor            = desc[i].divisor;
        //             attribs_group->attribs_descr[attrib_idx].shader_input_flags = desc[i].shader_input_flags;
        //             attribs_group->attribs_count++;
        //         }
        //
        //         const attr_t shader_input_id = shaderinfo_flag_to_id(desc[i].attrib_flag);
        //         shader_create_input(build_idx, shader_input_id, desc[i].shader_input_flags, desc[i].divisor);
        //     }
        // }
        //
        //
        //
        // /***********************************************************************************************
        //  * UNIFORMS */
        // /// add projection matrix uniform
        // attrib_flags = IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ;
        // shader_input_id = SID_PROJMAT4X4_ID;
        // shader_create_input(build_idx, shader_input_id, attrib_flags, 0);
        //
        // attribsGroupDescr_set_stride(attribs_group_id, stride);
        //
        // print("%s- Create object: triangle -%s\n"
        //       "   id:%u\n   attribs_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
        //       BLUE, CLEAR, obj->id, obj->attribs_group_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
        //
        // engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
        // const attr_t vao_attribs_idx = 0;
        //
        // struct object* tri = obj;
        // test_create_object(tri->attribs_group_id, vao_attribs_idx, tri->id, 1, MESH_TRIANGLE, &camera, "vpos2", mesh_get_mesh_vertices_from_type(MESH_TRIANGLE));
    }

    {
        // // const uint64 indexed_geometry = SID_INDEXED;
    // const uint64 indexed_geometry = 0x0;
    //
    // float wpos[3] = {-5, 3,-14}, rot[2] = {cos(0.0),sin(0.0)}, scale[2] = {4,4}, color[4] = {.2,.5,.9,1};
    // uint16 num_objects = 1;
    //
    // wpos[0] = -4; wpos[1] = 4;
    // struct object* tri = test_create_triangles(num_objects, wpos, scale, rot, color, indexed_geometry);
    // test_create_object(tri->attribs_group_id, tri->id, num_objects, MESH_TRIANGLE, &camera);
    // object_destroy(tri2->id);

    // wpos[0] = -2; wpos[1] = 4;
    // struct object* quad = test_create_quads(num_objects, wpos, scale, rot, color, indexed_geometry);
    // test_create_object(quad->attribs_id, quad->id, num_objects, MESH_TRIANGLE, &camera);
    //
    //
    // wpos[0] = -4; wpos[1] = 2;
    // struct object* penta = test_create_pentagon(num_objects, wpos, scale, rot, color, indexed_geometry);
    // test_create_object(penta->attribs_id, penta->id, num_objects, MESH_PENTAGON, &camera);
    //
    // wpos[0] = -4; wpos[1] = 0;
    // struct object* hex = test_create_hexagon(num_objects, wpos, scale, rot, color, indexed_geometry);
    // test_create_object(hex->attribs_id, hex->id, num_objects, MESH_HEXAGON, &camera);
    //
    // wpos[0] = -4; wpos[1] = -2;
    // struct object* octa = test_create_octagon(num_objects, wpos, scale, rot, color, indexed_geometry);
    // test_create_object(octa->attribs_id, octa->id, num_objects, MESH_OCTAGON, &camera);
    //
    // wpos[0] = 2; wpos[1] = 4;
    // struct object* cube = test_create_cube(num_objects, wpos, scale, rot, color, indexed_geometry);
    // test_create_object(cube->attribs_id, cube->id, num_objects, MESH_CUBE, &camera);

    // wpos[0] = 2; wpos[1] = 0;
    // // wpos[0] = 0; wpos[1] = 0; wpos[2]=-14;
    // struct object* octahedron = test_create_octahedron(num_objects, wpos, scale, rot, color, indexed_geometry);
    // test_create_object(octahedron->attribs_id, octahedron->id, num_objects, MESH_OCTAHEDRON, &camera);
    //
    // wpos[0] = 2; wpos[1] = -3;
    // // mesh_build_dodecahedron();
    // struct object* dodecahedron = test_create_dodecahedron(num_objects, wpos, scale, rot, color, indexed_geometry);
    // test_create_object(dodecahedron->attribs_id, dodecahedron->id, num_objects, MESH_DODECAHEDRON, &camera);
        }

    render_tri(&camera);

    /// DEBUG OPENGL
    {
    // GL_CHECK();
    //     struct shader_descriptor* desc = shader_descriptor_get(0);
    //     uint32 total_vertex_count = test_get_total_vertex_count(0);
    //     struct opengl_program* prog = shader_program_get(0);
    //     struct vertex_array_object* va = gfx_vertex_array_get_byidx(0);
    //     struct vertex_buffer_object* vb = vertex_buffer_get_vbo(0);
    //     debug_opengl(prog, va, vb, total_vertex_count, desc->vshader_attribs_stride);
    // GL_CHECK();
    }

    /* DEBUG-PRINT */
    attribsGroupDescr_group_print();
    shader_desc_table_print_build_entries(0);
    temp_print_builds();
    // string_format_test_validity();


    /* PERF-TIMER */
    struct timer* loop_timer = timer_create_perf_timer();



    /* RUN APP */
    run(loop_timer);

    // engine_fdebug_append_to_file_from_arena(ENGINE_FDEBUG_FILE_GENERIC);
    engine_fdebug_append_to_file_from_arena(ENGINE_FDEBUG_FILE_GENERIC);
    engine_fdebug_flush_arena(ENGINE_FDEBUG_FILE_GENERIC);


    /* CLOSE APP */
    buffer_arr_destroy_table();
    shader_builder_destroy();
    memman_terminate();

    return 0;
}


void test_bubble_sort() {
    // Example 1: Sorting integer buffer
    printf("=== Integer Bubble Sort ===\n");
    int int_arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(int_arr) / sizeof(int_arr[0]);

    printf("Original array: ");
    print_int_array(int_arr, n);

    // bubble_sort_int(int_arr, n);

    printf("Sorted array: ");
    print_int_array(int_arr, n);
    printf("\n");

    // Example 2: Sorting character buffer
    printf("=== Character Bubble Sort ===\n");
    char char_arr[] = {'z', 'a', 'm', 'b', 'c', 'f', 'e'};
    int char_n = sizeof(char_arr) / sizeof(char_arr[0]);

    printf("Original array: ");
    print_char_array(char_arr, char_n);

    bubble_sort_char(char_arr, char_n);

    printf("Sorted array: ");
    print_char_array(char_arr, char_n);
    printf("\n");

    // Example 3: Generic bubble sort
    printf("=== Generic Bubble Sort ===\n");
    int generic_arr[] = {64, 34, 25, 12, 22, 11, 90};
    int gen_n = sizeof(generic_arr) / sizeof(generic_arr[0]);

    printf("Original array: ");
    print_int_array(generic_arr, gen_n);

    bubble_sort_generic(generic_arr, gen_n, sizeof(int), compare_int);

    printf("Sorted array: ");
    print_int_array(generic_arr, gen_n);
    printf("\n");

    // Example 4: Sorting float array using generic function
    printf("=== Float Bubble Sort ===\n");
    float float_arr[] = {3.14, 1.5, 2.7, 0.5, 4.2, 1.1};
    int float_n = sizeof(float_arr) / sizeof(float_arr[0]);

    printf("Original array: ");
    for (int i = 0; i < float_n; i++) {
        printf("%.2f ", float_arr[i]);
    }
    printf("\n");

    bubble_sort_generic(float_arr, float_n, sizeof(float), compare_float);

    printf("Sorted array: ");
    for (int i = 0; i < float_n; i++) {
        printf("%.2f ", float_arr[i]);
    }
    printf("\n");

}


