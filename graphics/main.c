#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "camera.h"
#include "gfx_buffers.h"
// #include "linmath.h"
#include "mem_man_interface.h"
#include "my_macros.h"
#include "mesh.h"
#include "object.h"
#include "opengl.h"
#include "opengl_debug.h"
#include "run.h"
#include "scene.h"
#include "shader_builder.h"
#include "sparse_arr_internals.h"
#include "test.h"


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




build_t
gfx_program_exists(const struct gfx_sid sid)
{
    build_t build_idx = shader_build_is_set(sid);
    return build_idx;
}

struct shader_descriptor*
gfx_create_shader_program(const struct gfx_sid sid, const build_t build_idx)
{
    struct shader_descriptor* desc = opengl_program_create(sid, build_idx, fragment_shader_text);
    GL_CHECK();


    return desc;
}


struct shader_descriptor*
gfx_generate_context(const struct gfx_sid sid, const build_t build_idx, struct camera camera)
{
    struct shader_descriptor* desc = NULL;
    build_t progidx = INT_NULL;
    vbidx_t vbidx = INT_NULL;


    /**********************************************************************************************/
    /* program */
    /* TODO
     * 1. check if same program exists
     * 2. if not, push a new build in the descriptor table
     */
    // // Check if program already exists
    progidx = gfx_program_exists(sid);
    if(progidx == INT_NULL) { // If shader program does not exist, create new program.



        desc = gfx_create_shader_program(sid, build_idx);
        const uint32 stride = desc->vshader_attribs_stride;
        progidx = desc->build_idx;
        GL_CHECK();
        struct opengl_program* prog = shader_program_get(progidx);
        gl_use_program(prog->program);


        vaidx_t vaoidx = gfx_vertex_array_create();
        desc->vaoidx = vaoidx;
        struct vertex_array_object* va = gfx_vertex_array_get_byidx(desc->vaoidx);
        gl_bind_vertex_array(va->vao);


        vbidx = gfx_vertex_buffer_create(sid, 20*stride*SIZE_F32, stride);
        struct vertex_buffer_object* vb = vertex_buffer_get_vbo(vbidx);
        gl_bind_buffer(OPENGL_ARRAY_BUFFER, vb->vbo);

        shader_attributes_enable(desc->vshader_attrib_locations_count, desc->vshader_attribs_stride, desc->last_loc_num_elems);
        opengl_debug_attributes_in(desc->vshader_attrib_locations_count);

        // uint32 total_vertex_count = test_obj_comp_desc_get_total_vertex_count(vbidx);
        renderer_renderqueue_add(prog->program, desc->build_idx, va->vao, va->idx, vb->vbo, vb->idx);

        GLint mvp_location = gl_get_uniform_location(prog->program, "projection_matrix");
        glUniformMatrix4fv(mvp_location, 1, camera.is_column_major, (float*)camera.projection_matrix.projection_matrix);

        gl_bind_vertex_array(0x0);
        gl_use_program(0x0);



        // Scene_set_projection_matrix(sceneidx);
        // Scene_set_screen_res_uniform();
    } else {
        desc = shader_descriptor_get(progidx);
        // struct opengl_program* prog = shader_program_get(progidx);
        // struct vertex_array_object* va = gfx_vertex_array_get_byidx(desc->vaoidx);
        // struct vertex_buffer_object* vb = vertex_buffer_get_vbo(vbidx);
        // renderer_renderqueue_add(prog->program, desc->build_idx, va->vao, va->idx, vb->vbo, vb->idx);
    }


    /**********************************************************************************************/
    /* vertex array object */
    /// set up vertex attrib pointers
    // gl_enable_attrib_array();
    // gl_enable_attrib_pointer();


    /**********************************************************************************************/
    /* vertex buffer object*/
    /* Check for any vbidx match, from already created vbs, in the mesh tree structure */
    /* If no vbidx found from the mesh tree, search the vb pool if any apropriate vb exists.
     * If none found, create a new one */
    // vbidx = gfx_vertex_buffer_find_match(sid);
    // if(vbidx == INT_NULL) {
    //     vbidx = gfx_vertex_buffer_create(sid, 1024);
    // }


    /**********************************************************************************************/
    /* index array object */


    // struct vertex_array_object va = gfx_vertex_array_get_byidx(vaoidx);
    // gl_bind_vertex_array(va->vao);
    // GL_CHECK();
    // assert(va->vao>0);
    //
    // progidx_t program = opengl_program_get_byidx(progidx);
    // vbidx_t vbo = vertex_buffer_get_vbo(vbidx);
    // renderer_renderqueue_add(program, progidx, vao, 0, vbo, vbidx);

    // OpenGlDebugContext();

    return desc;
}



struct object* test_create_triangles(const uint32 num_objects, struct camera camera, float wpos[3], float scale[2], float rot[2], float color[4])
{
    // wpos[0]=0; wpos[1]=0; wpos[2]=-4; scale[0]=1; scale[1]=1;
    // struct object tri = object_create_triangle((struct gfx_sid){0, 0, 0, 0, 0}, wpos, scale, rot, color);
    struct object* tri;
    for(uint32 i = 0; i < num_objects; i++) {
        // wpos[0]=(i*3.f)-8.0f; wpos[1]=3; wpos[2]=-14; scale[0]=1; scale[1]=1;
        wpos[0]=(i*3.f)-8.0f; wpos[2]=-14; scale[0]=1; scale[1]=1;
        tri = object_create_triangle((struct gfx_sid){0, 0, 0, 0, 0}, wpos, scale, rot, color);
    }
    // HERE: create an object buffer table for all objects

    struct shader_descriptor* desc = gfx_generate_context(tri->sid, test_get_build_idx(tri->comp_id), camera);

//     // uint32 num_objects = test_obj_comp_desc_get_total_object_count(tri->comp_id);
//     uint32 obj_vertex_count = test_get_obj_vertex_count(tri->comp_id);
//     uint32 total_vertex_count = test_get_total_vertex_count(tri->comp_id);
//
// // HERE: we always send the total object count and vertices from the components group
//     uint8 num_vpos = shaderinfo_flag_to_numelems(SID_VPOSI2);
//     tri->attrib_data = gfx_vertex_buffer_set_data(tri->comp_id, obj_vertex_count, num_objects, num_vpos, tri->num_faces*tri->verts_per_face*num_vpos, quad_vertices);
//
//     uint32 rqidx = 0;
//     render_queue_add_draw_count(rqidx, total_vertex_count);

    return tri;
}

void test_create_quads(struct camera camera, float wpos[3], float scale[2], float rot[2], float color[4])
{
    wpos[0]=6; wpos[1]=0; wpos[2]=-4; scale[0]=1; scale[1]=1;
    struct object* quad = object_create_rectangle((struct gfx_sid){0, 0, 0, 0, 0}, wpos, scale, rot, color);

    // struct object* quad;
    // for(uint32 i = 0; i < 6; i++) {
    //     wpos[0]=(i*3.f)-8.0f; wpos[1]=3; wpos[2]=-14; scale[0]=1; scale[1]=1;
    //     quad = object_create_rectangle((struct gfx_sid){0, 0, 0, 0, 0}, wpos, scale, rot, color);
    // }
    //


    struct shader_descriptor* desc = gfx_generate_context(quad->sid, test_get_build_idx(quad->comp_id), camera);
    vaidx_t vaoidx = gfx_vertex_array_create();
    desc->vaoidx = vaoidx;

    uint32 num_objects = test_obj_comp_desc_get_total_object_count(quad->comp_id);
    uint32 obj_vertex_count = test_get_obj_vertex_count(quad->comp_id);

    const uint32 stride = desc->vshader_attribs_stride;
    const vbidx_t vbidx = gfx_vertex_buffer_create(quad->sid, num_objects*obj_vertex_count*stride*SIZE_F32, stride);
    struct vertex_buffer_object* vb = vertex_buffer_get_vbo(vbidx);
    uint8 num_vpos = shaderinfo_flag_to_numelems(SID_VPOSI2);
    quad->attrib_data = gfx_vertex_buffer_set_data(quad->comp_id, obj_vertex_count, num_objects, num_vpos, quad->num_faces*quad->verts_per_face*num_vpos, quad_vertices);

    gl_bind_buffer(OPENGL_ARRAY_BUFFER, vb->vbo);
    shader_attributes_enable(desc->vshader_attrib_locations_count, desc->vshader_attribs_stride, desc->last_loc_num_elems);
    opengl_debug_attributes_in(desc->vshader_attrib_locations_count);

    struct opengl_program* prog = opengl_program_get_program_byidx(desc->build_idx);
    struct vertex_array_object* va = gfx_vertex_array_get_byidx(desc->vaoidx);
    uint32 total_vertex_count = test_obj_comp_desc_get_total_vertex_count(quad->comp_id);
    renderer_renderqueue_add(prog->program, desc->build_idx, va->vao, va->idx, vb->vbo, vb->idx);

    gfx_vertex_buffer_update_data(vb->idx);

    gl_use_program(prog->program);
    gl_bind_vertex_array(va->vao);
    GLint mvp_location = gl_get_uniform_location(prog->program, "projection_matrix");
    glUniformMatrix4fv(mvp_location, 1, camera.is_column_major, (float*)camera.projection_matrix.projection_matrix);

    debug_opengl(prog, va, vb, total_vertex_count, desc->vshader_attribs_stride);
    temp_print_builds();

    gl_bind_vertex_array(0x0);
    gl_use_program(0x0);
}

void test_create_pentagon(struct camera camera, float wpos[3], float scale[2], float rot[2], float color[4])
{
    wpos[0]=0; wpos[1]=0; wpos[2]=-4; scale[0]=1; scale[1]=1;
    struct object* rect = object_create_pentagon((struct gfx_sid){0, 0, SID_VPOSI2, SID_VPOSI2, 0}, wpos, scale, rot, color);


    struct shader_descriptor* desc = gfx_generate_context(rect->sid, test_get_build_idx(rect->comp_id), camera);
    vaidx_t vaoidx = gfx_vertex_array_create();
    desc->vaoidx = vaoidx;

    uint32 num_objects = test_obj_comp_desc_get_total_object_count(rect->comp_id);
    uint32 obj_vertex_count = test_get_obj_vertex_count(rect->comp_id);

    const uint32 stride = desc->vshader_attribs_stride;
    const vbidx_t vbidx = gfx_vertex_buffer_create(rect->sid, num_objects*obj_vertex_count*stride*SIZE_F32, stride);
    struct vertex_buffer_object* vb = vertex_buffer_get_vbo(vbidx);
    uint8 num_vpos = shaderinfo_flag_to_numelems(SID_VPOSI2);
    rect->attrib_data = gfx_vertex_buffer_set_data(rect->comp_id, obj_vertex_count, num_objects, num_vpos, rect->num_faces*rect->verts_per_face*num_vpos, pentagon_vertices);

    gl_bind_buffer(OPENGL_ARRAY_BUFFER, vb->vbo);
    shader_attributes_enable(desc->vshader_attrib_locations_count, desc->vshader_attribs_stride, desc->last_loc_num_elems);
    opengl_debug_attributes_in(desc->vshader_attrib_locations_count);

    struct opengl_program* prog = opengl_program_get_program_byidx(desc->build_idx);
    struct vertex_array_object* va = gfx_vertex_array_get_byidx(desc->vaoidx);
    uint32 total_vertex_count = test_obj_comp_desc_get_total_vertex_count(rect->comp_id);
    renderer_renderqueue_add(prog->program, desc->build_idx, va->vao, va->idx, vb->vbo, vb->idx);

    gfx_vertex_buffer_update_data(vb->idx);

    gl_use_program(prog->program);
    gl_bind_vertex_array(va->vao);
    GLint mvp_location = gl_get_uniform_location(prog->program, "projection_matrix");
    glUniformMatrix4fv(mvp_location, 1, camera.is_column_major, (float*)camera.projection_matrix.projection_matrix);

    debug_opengl(prog, va, vb, total_vertex_count, desc->vshader_attribs_stride);
    temp_print_builds();

    gl_bind_vertex_array(0x0);
    gl_use_program(0x0);
}

void test_create_octagon(struct camera camera, float wpos[3], float scale[2], float rot[2], float color[4])
{
    wpos[0]=0; wpos[1]=0; wpos[2]=-4; scale[0]=1; scale[1]=1;
    struct object* rect = object_create_octagon((struct gfx_sid){0, 0, 0, 0, 0}, wpos, scale, rot, color);


    struct shader_descriptor* desc = gfx_generate_context(rect->sid, test_get_build_idx(rect->comp_id), camera);
    vaidx_t vaoidx = gfx_vertex_array_create();
    desc->vaoidx = vaoidx;

    uint32 num_objects = test_obj_comp_desc_get_total_object_count(rect->comp_id);
    uint32 obj_vertex_count = test_get_obj_vertex_count(rect->comp_id);

    const uint32 stride = desc->vshader_attribs_stride;
    const vbidx_t vbidx = gfx_vertex_buffer_create(rect->sid, num_objects*obj_vertex_count*stride*SIZE_F32, stride);
    struct vertex_buffer_object* vb = vertex_buffer_get_vbo(vbidx);
    uint8 num_vpos = shaderinfo_flag_to_numelems(SID_VPOSI2);
    rect->attrib_data = gfx_vertex_buffer_set_data(rect->comp_id, obj_vertex_count, num_objects, num_vpos, rect->num_faces*rect->verts_per_face*num_vpos, octagon_vertices);

    gl_bind_buffer(OPENGL_ARRAY_BUFFER, vb->vbo);
    shader_attributes_enable(desc->vshader_attrib_locations_count, desc->vshader_attribs_stride, desc->last_loc_num_elems);
    opengl_debug_attributes_in(desc->vshader_attrib_locations_count);

    struct opengl_program* prog = opengl_program_get_program_byidx(desc->build_idx);
    struct vertex_array_object* va = gfx_vertex_array_get_byidx(desc->vaoidx);
    uint32 total_vertex_count = test_obj_comp_desc_get_total_vertex_count(rect->comp_id);
    renderer_renderqueue_add(prog->program, desc->build_idx, va->vao, va->idx, vb->vbo, vb->idx);

    gfx_vertex_buffer_update_data(vb->idx);

    gl_use_program(prog->program);
    gl_bind_vertex_array(va->vao);
    GLint mvp_location = gl_get_uniform_location(prog->program, "projection_matrix");
    glUniformMatrix4fv(mvp_location, 1, camera.is_column_major, (float*)camera.projection_matrix.projection_matrix);

    debug_opengl(prog, va, vb, total_vertex_count, desc->vshader_attribs_stride);
    temp_print_builds();

    gl_bind_vertex_array(0x0);
    gl_use_program(0x0);
}

void test_create_cube(struct camera camera, float wpos[3], float scale[2], float rot[2], float color[4])
{
    wpos[0]=0; wpos[1]=0; wpos[2]=-4; scale[0]=1; scale[1]=1; color[1]=.2; color[2]=.9;
    struct object* cube = object_create_cube((struct gfx_sid){0, 0, 0, 0, 0}, wpos, scale, rot, color);
    //
    // wpos[0] = 0; color[0]=.7;color[1]=.7;color[2]=.2;
    // struct object* obj2 = object_create_cube(cube->sid, wpos, scale, rot, color);
    // wpos[0] = 10; color[0]=.7;color[1]=.4;color[2]=.2;
    // struct object* obj3 = object_create_cube(cube->sid, wpos, scale, rot, color);
    // wpos[0] = -10; wpos[1] = 0; color[0]=.2;color[1]=.7;color[2]=.2;
    // struct object* obj4 = object_create_cube(cube->sid, wpos, scale, rot, color);
    // wpos[0] = 0; wpos[1] = 0; color[0]=.2;color[1]=.7;color[2]=.7;
    // struct object* obj5 = object_create_cube(cube->sid, wpos, scale, rot, color);
    // wpos[0] = 10; wpos[1] = 0; color[0]=.2;color[1]=.4;color[2]=.9;
    // struct object* obj6 = object_create_cube(cube->sid, wpos, scale, rot, color);

    struct shader_descriptor* desc = gfx_generate_context(cube->sid, test_get_build_idx(cube->comp_id), camera);
    vaidx_t vaoidx = gfx_vertex_array_create();
    desc->vaoidx = vaoidx;


    uint32 num_objects = test_obj_comp_desc_get_total_object_count(cube->comp_id);
    uint32 obj_vertex_count = test_get_obj_vertex_count(cube->comp_id);

    const uint32 stride = desc->vshader_attribs_stride;
    const vbidx_t vbidx = gfx_vertex_buffer_create(cube->sid, num_objects*obj_vertex_count*stride*SIZE_F32, stride);
    struct vertex_buffer_object* vb = vertex_buffer_get_vbo(vbidx);
    uint8 num_vpos = shaderinfo_flag_to_numelems(SID_VPOSI3);
    cube->attrib_data = gfx_vertex_buffer_set_data(cube->comp_id, obj_vertex_count, num_objects, num_vpos, cube->num_faces*cube->verts_per_face*num_vpos, cube_vertices);

    gl_bind_buffer(OPENGL_ARRAY_BUFFER, vb->vbo);
    shader_attributes_enable(desc->vshader_attrib_locations_count, desc->vshader_attribs_stride, desc->last_loc_num_elems);
    opengl_debug_attributes_in(desc->vshader_attrib_locations_count);

    struct opengl_program* prog = opengl_program_get_program_byidx(desc->build_idx);
    struct vertex_array_object* va = gfx_vertex_array_get_byidx(desc->vaoidx);
    uint32 total_vertex_count = test_obj_comp_desc_get_total_vertex_count(cube->comp_id);
    renderer_renderqueue_add(prog->program, desc->build_idx, va->vao, va->idx, vb->vbo, vb->idx);

    gfx_vertex_buffer_update_data(vb->idx);

    gl_use_program(prog->program);
    gl_bind_vertex_array(va->vao);
    GLint mvp_location = gl_get_uniform_location(prog->program, "projection_matrix");
    glUniformMatrix4fv(mvp_location, 1, camera.is_column_major, (float*)camera.projection_matrix.projection_matrix);

    debug_opengl(prog, va, vb, total_vertex_count, desc->vshader_attribs_stride);
    temp_print_builds();

    gl_bind_vertex_array(0x0);
    gl_use_program(0x0);
}

void test_create_octahedron(struct camera camera, float wpos[3], float scale[2], float rot[2], float color[4])
{
        wpos[0]=0; wpos[1]=0; wpos[2]=-4; scale[0]=1; scale[1]=1; color[0]=.7; color[1]=.2; color[2]=.9;
        struct object* octa = object_create_octahedron((struct gfx_sid){0, 0, 0, 0, 0}, wpos, scale, rot, color);
        //
        // wpos[0] = 0; color[0]=.7;color[1]=.7;color[2]=.2;
        // struct object obj2 = object_create_cube(cube->sid, wpos, scale, rot, color);
        // wpos[0] = 10; color[0]=.7;color[1]=.4;color[2]=.2;
        // struct object obj3 = object_create_cube(cube->sid, wpos, scale, rot, color);
        // wpos[0] = -10; wpos[1] = 0; color[0]=.2;color[1]=.7;color[2]=.2;
        // struct object obj4 = object_create_cube(cube->sid, wpos, scale, rot, color);
        // wpos[0] = 0; wpos[1] = 0; color[0]=.2;color[1]=.7;color[2]=.7;
        // struct object obj5 = object_create_cube(cube->sid, wpos, scale, rot, color);
        // wpos[0] = 10; wpos[1] = 0; color[0]=.2;color[1]=.4;color[2]=.9;
        // struct object obj6 = object_create_cube(cube->sid, wpos, scale, rot, color);

        struct shader_descriptor* desc = gfx_generate_context(octa->sid, test_get_build_idx(octa->comp_id), camera);
        vaidx_t vaoidx = gfx_vertex_array_create();
        desc->vaoidx = vaoidx;


        uint32 num_objects = test_obj_comp_desc_get_total_object_count(octa->comp_id);
        uint32 obj_vertex_count = test_get_obj_vertex_count(octa->comp_id);

        const uint32 stride = desc->vshader_attribs_stride;
        const vbidx_t vbidx = gfx_vertex_buffer_create(octa->sid, num_objects*obj_vertex_count*stride*SIZE_F32, stride);
        struct vertex_buffer_object* vb = vertex_buffer_get_vbo(vbidx);
        uint8 num_vpos = shaderinfo_flag_to_numelems(SID_VPOSI3);
        octa->attrib_data = gfx_vertex_buffer_set_data(octa->comp_id, obj_vertex_count, num_objects, num_vpos, octa->num_faces*octa->verts_per_face*num_vpos, octahedron_vertices);
        // cube->attrib_data = gfx_vertex_buffer_set_data(cube->comp_id, obj_vertex_count, num_objects, num_vpos, FACES_PER_OCTAHEDRON * VERTICES_PER_TRI * VERTEX_POS3*num_vpos, octahedron_vertices);

        gl_bind_buffer(OPENGL_ARRAY_BUFFER, vb->vbo);
        shader_attributes_enable(desc->vshader_attrib_locations_count, desc->vshader_attribs_stride, desc->last_loc_num_elems);
        opengl_debug_attributes_in(desc->vshader_attrib_locations_count);

        struct opengl_program* prog = opengl_program_get_program_byidx(desc->build_idx);
        struct vertex_array_object* va = gfx_vertex_array_get_byidx(desc->vaoidx);
        uint32 total_vertex_count = test_obj_comp_desc_get_total_vertex_count(octa->comp_id);
        renderer_renderqueue_add(prog->program, desc->build_idx, va->vao, va->idx, vb->vbo, vb->idx);

        gfx_vertex_buffer_update_data(vb->idx);

        gl_use_program(prog->program);
        gl_bind_vertex_array(va->vao);
        GLint mvp_location = gl_get_uniform_location(prog->program, "projection_matrix");
        glUniformMatrix4fv(mvp_location, 1, camera.is_column_major, (float*)camera.projection_matrix.projection_matrix);

        debug_opengl(prog, va, vb, total_vertex_count, desc->vshader_attribs_stride);
        temp_print_builds();

        gl_bind_vertex_array(0x0);
        gl_use_program(0x0);
}

void test_create_dodecahedron(struct camera camera, float wpos[3], float scale[2], float rot[2], float color[4])
{
    wpos[0]=0; wpos[1]=0; wpos[2]=-4; scale[0]=1; scale[1]=1;
    struct object* rect = object_create_dodecahedron((struct gfx_sid){0, 0, 0, 0, 0}, wpos, scale, rot, color);


    struct opengl_program* prog = NULL;
    struct vertex_buffer_object* vb = NULL;
    
    struct shader_descriptor* desc = gfx_generate_context(rect->sid, test_get_build_idx(rect->comp_id), camera);
    
    
    
    if(desc->is_set) {
        
    } else {
        
    }
    
    vaidx_t vaoidx = gfx_vertex_array_create();
    desc->vaoidx = vaoidx;

    uint32 num_objects = test_obj_comp_desc_get_total_object_count(rect->comp_id);
    uint32 obj_vertex_count = test_get_obj_vertex_count(rect->comp_id);

    const uint32 stride = desc->vshader_attribs_stride;
    const vbidx_t vbidx = gfx_vertex_buffer_create(rect->sid, num_objects*obj_vertex_count*stride*SIZE_F32, stride);
    vb = vertex_buffer_get_vbo(vbidx);
    uint8 num_vpos = shaderinfo_flag_to_numelems(SID_VPOSI3);
    mesh_build_dodecahedron();
    rect->attrib_data = gfx_vertex_buffer_set_data(rect->comp_id, obj_vertex_count, num_objects, num_vpos, rect->num_faces*rect->verts_per_face*num_vpos, dodecahedron_vertices);

    gl_bind_buffer(OPENGL_ARRAY_BUFFER, vb->vbo);
    shader_attributes_enable(desc->vshader_attrib_locations_count, desc->vshader_attribs_stride, desc->last_loc_num_elems);
    opengl_debug_attributes_in(desc->vshader_attrib_locations_count);

    prog = opengl_program_get_program_byidx(desc->build_idx);
    struct vertex_array_object* va = gfx_vertex_array_get_byidx(desc->vaoidx);
    uint32 total_vertex_count = test_obj_comp_desc_get_total_vertex_count(rect->comp_id);
    renderer_renderqueue_add(prog->program, desc->build_idx, va->vao, va->idx, vb->vbo, vb->idx);

    gfx_vertex_buffer_update_data(vb->idx);

    gl_use_program(prog->program);
    gl_bind_vertex_array(va->vao);
    GLint mvp_location = gl_get_uniform_location(prog->program, "projection_matrix");
    glUniformMatrix4fv(mvp_location, 1, camera.is_column_major, (float*)camera.projection_matrix.projection_matrix);

    debug_opengl(prog, va, vb, total_vertex_count, desc->vshader_attribs_stride);
    temp_print_builds();

    gl_bind_vertex_array(0x0);
    gl_use_program(0x0);
}


int
main(void)
{
    memman_init();
    sparse_arr_init_table(64);
    shader_descriptor_init();
    shader_builder_init();

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
    // glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LEQUAL);
    glDepthFunc(GL_LESS);
    // glDepthMask(GL_TRUE);
    // glHint(GLenum target, GLenum mode = GL_FASTEST, GL_NICEST, and GL_DONT_CARE);
    // glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);



    gl_viewport(SCREEN_RES_X, SCREEN_RES_Y);
    opengl_debug_init();


    float wpos[3] = {0, 0,-40}, rot[2] = {cos(0.0),sin(.0)}, scale[2] = {4,4}, color[4] = {.2,.5,.9,1};

    /// render triangles
    wpos[1] = 3;
    struct object* tri = test_create_triangles(3, camera, wpos, scale, rot, color);
    wpos[1] = 0;
    test_create_triangles(3, camera, wpos, scale, rot, color);

    /// render quads
    // test_create_quads(camera, wpos, scale, rot, color);

    /// render pentagons
    // test_create_pentagon(camera, wpos, scale, rot, color);

    /// render octagons
    // test_create_octagon(camera, wpos, scale, rot, color);

    /// render cubes
    // test_create_cube(camera, wpos, scale, rot, color);

    /// render octahedrons
    // test_create_octahedron(camera, wpos, scale, rot, color);

    /// render octahedrons
    // test_create_dodecahedron(camera, wpos, scale, rot, color);

    uint32 num_objects = test_obj_comp_desc_get_total_object_count(tri->comp_id);
    uint32 obj_vertex_count = test_get_obj_vertex_count(tri->comp_id);
    uint32 total_vertex_count = test_get_total_vertex_count(tri->comp_id);
    //
    // // HERE: we always send the total object count and vertices from the components group
    uint8 num_vpos = shaderinfo_flag_to_numelems(SID_VPOSI2);
    tri->attrib_data = gfx_vertex_buffer_set_data(tri->comp_id, obj_vertex_count, num_objects, num_vpos, tri->num_faces*tri->verts_per_face*num_vpos, quad_vertices);

    uint32 rqidx = 0;
    render_queue_add_draw_count(rqidx, total_vertex_count);


    struct opengl_program* prog = shader_program_get(0);
    struct vertex_array_object* va = gfx_vertex_array_get_byidx(0);
    struct vertex_buffer_object* vb = vertex_buffer_get_vbo(0);
    gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);
    gfx_vertex_buffer_update_data(vb->idx);
    struct shader_descriptor* desc = shader_descriptor_get(0);

    debug_opengl(prog, va, vb, total_vertex_count, desc->vshader_attribs_stride);
    // // debug_opengl(prog, va, vb, total_vertex_count, desc->vshader_attribs_stride);
    temp_print_builds();

    obj_comp_group_print();

    run();

    sparse_arr_destroy_table();
    shader_builder_destroy();
    memman_terminate();

    return 0;
}


