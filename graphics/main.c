#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
    // float* mesh_data;
    struct camera camera;
    sididx_t sid_idx;
    uint16   attribs_idx;
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
    GLint mvp_location = gl_get_uniform_location(prog->program, "projection_matrix");
    struct camera camera = params->camera;
    glUniformMatrix4fv(mvp_location, 1, camera.is_column_major, (float*)camera.projection_matrix.projection_matrix);
    GL_CHECK();
    gl_use_program(0x0);
}
void command_create_vao(struct cmd_gfx_params* params)
{
    struct attributes_group* attribs_group = attribsGroupDescr_get_group(params->attribs_idx);
    struct shader_descriptor* desc = shader_descriptor_get(params->desc_idx);
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
    gl_bind_vertex_array(va->vao);
    GL_CHECK();

    /* VERTEX BUFFER */
    vbidx_t vbidx = INT_NULL;
    struct vertex_buffer_object* vb = NULL;
    if(is_new_vao){
        /// TODO: calculate the size of the vertex buffer OR use dynamic array vertex buffer
        vbidx = gfx_vertex_buffer_create(200*stride*SIZE_F32, stride);

        vb = vertex_buffer_get_vbo(vbidx);
        gl_bind_buffer(OPENGL_ARRAY_BUFFER, vb->vbo);
        GL_CHECK();

        shader_attributes_enable(desc->vshader_attrib_locations_count, desc->vshader_attribs_stride, desc->last_loc_num_elems);
        opengl_debug_attributes_in(desc->vshader_attrib_locations_count);
        GL_CHECK();
        attribs_group->rqidx = renderer_renderqueue_add(params->sid_idx, prog->program, desc->build_idx, va->vao, va->idx, vb->vbo, vb->idx);
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
    /// SET COMMAND: IB_SET_DATE
    struct ib_cmd_params ind_args = { .ibidx = ib->idx, .vpos_num_elems = params->vpos_num_elems, .attribs_idx = params->attribs_idx};
    p = &ind_args;
    cmd_add_command((void*)gfx_vertex_index_buffer_set_data, p, sizeof(struct ib_cmd_params));

    // SET COMMAND: VB_SET_DATA
    cmd_add_command((void*) gfx_vertex_index_buffer_update_data, p, sizeof(struct ib_cmd_params));


    /// SET COMMAND: VB_SET_DATE
    struct vb_cmd_params vert_args = { .vbidx = vb->idx, .vpos_num_elems = params->vpos_num_elems, .attribs_idx = params->attribs_idx};
    p = &vert_args;
    cmd_add_command((void*)gfx_vertex_buffer_set_data, p, sizeof(struct vb_cmd_params));

    // SET COMMAND: VB_SET_DATA
    cmd_add_command((void*) gfx_vertex_buffer_update_data, p, sizeof(struct vb_cmd_params));



    gl_use_program(0x0);
    gl_bind_vertex_array(0x0);
    gl_bind_buffer(OPENGL_ARRAY_BUFFER, 0x0);


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

struct object* test_create_quads(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
{
    struct object* obj;
    scale[0]=1; scale[1]=1;
    for(uint32 i = 0; i < num_objects; i++) {
        obj = object_create_rectangle((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
        wpos[0] +=3.f;
    }

    return obj;
}

struct object* test_create_pentagon(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
{
    struct object* obj;
    scale[0]=1; scale[1]=1;
    for(uint32 i = 0; i < num_objects; i++) {
        obj = object_create_pentagon((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
        wpos[0] += 3.f;
    }
    return obj;
}

struct object* test_create_hexagon(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
{
    struct object* obj;
    scale[0]=1; scale[1]=1;
    wpos[2] += .01;
    for(uint32 i = 0; i < num_objects; i++) {
        obj = object_create_hexagon((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
        wpos[0] += 3.f;
    }
    return obj;
}

struct object* test_create_octagon(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
{
    struct object* obj;
    scale[0]=1; scale[1]=1;
    for(uint32 i = 0; i < num_objects; i++) {
        obj = object_create_octagon((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
        wpos[0] += 3.f;
    }
    return obj;
}

struct object* test_create_cube(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
{
    struct object* obj;
    scale[0]=1; scale[1]=1;
    for(uint32 i = 0; i < num_objects; i++) {
        obj = object_create_cube((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
        wpos[0] -= 3.f;
    }
    return obj;
}

struct object* test_create_octahedron(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
{
    struct object* obj;
    scale[0]=1; scale[1]=1;
    for(uint32 i = 0; i < num_objects; i++) {
        obj = object_create_octahedron((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
        wpos[0] -= 3.f;
    }
    return obj;
}

struct object* test_create_dodecahedron(const uint32 num_objects, float wpos[3], float scale[2], float rot[2], float color[4], const uint64 attr_flags)
{
    struct object* obj;
    scale[0]=1; scale[1]=1;
    for(uint32 i = 0; i < num_objects; i++) {
        obj = object_create_dodecahedron((struct gfx_sid){attr_flags, 0, 0, 0, 0}, wpos, scale, rot, color);
        wpos[0] -= 3.f;
    }
    return obj;
}


void test_create_object(const uint16 attribs_idx, const objid_t obj_id, const uint32 num_objects, enum MESH_TYPE mesh_type, struct camera* camera)
{
    struct attributes_group* attribs_group = attribsGroupDescr_get_group(attribs_idx);

    struct cmd_gfx_params gfx_params = {
        .sid_idx = attribs_group->sid_idx,
        .attribs_idx = attribs_idx,
        .build_idx = attribs_group->build_idx,
        .desc_idx = INT_NULL,
        .vpos_num_elems = mesh_get_vpos_num_elems(mesh_type),
        .camera = *camera,
    };

    if((attribs_group->commands_flag & CMD_CREATE_SHADER_PROGRAM)==0) {
        cmd_add_command((void*)command_create_shader_program, &gfx_params, sizeof(struct cmd_gfx_params));
        attribs_group->commands_flag |= CMD_CREATE_SHADER_PROGRAM;
        gfx_params.desc_idx =  attribsGroupDescr_get_group(attribs_idx)->build_idx;
    }


    if((attribs_group->commands_flag & CMD_CREATE_VAO)==0) {
        cmd_add_command((void*)command_create_vao, &gfx_params, sizeof(struct cmd_gfx_params));
        attribs_group->commands_flag |= CMD_CREATE_VAO;
    }


    if((attribs_group->commands_flag & CMD_SET_CAMERA_PROJ)==0) {
        cmd_add_command((void*)command_set_camera, &gfx_params, sizeof(struct cmd_gfx_params));
        attribs_group->commands_flag |= CMD_SET_CAMERA_PROJ;
    }

    // struct command_render_queue_params rq_params = {.attribs_idx = attribs_idx, .vertex_count = num_objects*num_vertices};
    struct command_render_queue_params rq_params = {
        .attribs_idx = attribs_idx,
        .index_count  = num_objects * object_get_vertex_index_count(obj_id),
        .vertex_count = num_objects * object_get_vertex_count(obj_id)
    };
    cmd_add_command((void*) render_queue_add_draw_count, &rq_params, sizeof(struct command_render_queue_params));

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

    // const uint64 indexed_geometry = SID_INDEXED;
    const uint64 indexed_geometry = 0x0;

    float wpos[3] = {-5, 3,-14}, rot[2] = {cos(0.0),sin(0.0)}, scale[2] = {4,4}, color[4] = {.2,.5,.9,1};
    uint16 num_objects = 1;

    wpos[0] = -4; wpos[1] = 4;
    struct object* tri = test_create_triangles(num_objects, wpos, scale, rot, color, indexed_geometry);
    test_create_object(tri->attribs_id, tri->id, num_objects, MESH_TRIANGLE, &camera);
    // object_destroy(tri2->id);

    wpos[0] = -2; wpos[1] = 4;
    struct object* quad = test_create_quads(num_objects, wpos, scale, rot, color, indexed_geometry);
    test_create_object(quad->attribs_id, quad->id, num_objects, MESH_TRIANGLE, &camera);


    wpos[0] = -4; wpos[1] = 2;
    struct object* penta = test_create_pentagon(num_objects, wpos, scale, rot, color, indexed_geometry);
    test_create_object(penta->attribs_id, penta->id, num_objects, MESH_PENTAGON, &camera);

    wpos[0] = -4; wpos[1] = 0;
    struct object* hex = test_create_hexagon(num_objects, wpos, scale, rot, color, indexed_geometry);
    test_create_object(hex->attribs_id, hex->id, num_objects, MESH_HEXAGON, &camera);

    wpos[0] = -4; wpos[1] = -2;
    struct object* octa = test_create_octagon(num_objects, wpos, scale, rot, color, indexed_geometry);
    test_create_object(octa->attribs_id, octa->id, num_objects, MESH_OCTAGON, &camera);

    wpos[0] = 2; wpos[1] = 4;
    struct object* cube = test_create_cube(num_objects, wpos, scale, rot, color, indexed_geometry);
    test_create_object(cube->attribs_id, cube->id, num_objects, MESH_CUBE, &camera);
    //
    // wpos[0] = 2; wpos[1] = 0;
    // // wpos[0] = 0; wpos[1] = 0; wpos[2]=-14;
    // struct object* octahedron = test_create_octahedron(num_objects, wpos, scale, rot, color, indexed_geometry);
    // test_create_object(octahedron->attribs_id, octahedron->id, num_objects, MESH_OCTAHEDRON, &camera);
    //
    // wpos[0] = 2; wpos[1] = -3;
    // // mesh_build_dodecahedron();
    // struct object* dodecahedron = test_create_dodecahedron(num_objects, wpos, scale, rot, color, indexed_geometry);
    // test_create_object(dodecahedron->attribs_id, dodecahedron->id, num_objects, MESH_DODECAHEDRON, &camera);

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


    temp_print_builds();
    attribsGroupDescr_group_print();

    struct timer* loop_timer = timer_create_perf_timer();

    string_format_test();


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


