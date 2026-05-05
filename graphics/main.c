#include <math.h>
#include <stdio.h>

#include "gfx_buffers.h"
#include "mem_man_interface.h"
#include "opengl.h"
#include "opengl_debug.h"
#include "run.h"
#include "scene.h"
#include "shader_builder.h"
#include "sparse_arr_internals.h"


static const char* fragment_shader_text =
        "#version 330\n"
        "in vec4 out_color4;\n"
        // "in vec3 out_color3;\n"
        "out vec4 fragment;\n"
        "void main()\n"
        "{\n"
        // "    fragment = out_color4;\n"
        // "    fragment = vec4(1.0, 0.0, 0.0, 1.0);\n"
        // "    fragment = vec4(out_color3, 1.0);\n"
        "    fragment = vec4(out_color4.rgb, 1.0);\n"
        "}\n";


progidx_t
gfx_program_exists(const struct gfx_sid sid) { return INT_NULL; }

struct shader_descriptor
gfx_create_shader_program(const struct gfx_sid sid)
{
    struct shader_descriptor desc = opengl_program_create(sid, fragment_shader_text);
    GL_CHECK();


    return desc;
}


struct shader_descriptor
gfx_generate_context(const struct gfx_sid sid)
{
    struct shader_descriptor desc = {0};
    progidx_t progidx = INT_NULL;
    // vaidx_t vaoidx = INT_NULL;
    // vaidx_t vao    = INT_NULL;
    // vbidx_t vbidx  = INT_NULL;
    // ibidx_t ibidx  = INT_NULL;
    // uint32 vb_group = NULL;
    // uint32 vbidxs = NULL;

    /**********************************************************************************************/
    /* program */
    // Check if program already exists
    progidx = gfx_program_exists(sid);
    if(progidx == INT_NULL) { // If shader program does not exist, create new program.


        desc = gfx_create_shader_program(sid);
        progidx = desc.progidx;
        GL_CHECK();
        // Scene_set_projection_matrix(sceneidx);
        // Scene_set_screen_res_uniform();


        //
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
    // gl_bind_vertex_array(va.vao);
    // GL_CHECK();
    // assert(va.vao>0);
    //
    // progidx_t program = opengl_program_get_byidx(progidx);
    // vbidx_t vbo = vertex_buffer_get_vbo(vbidx);
    // renderer_renderqueue_add(program, progidx, vao, 0, vbo, vbidx);

    // OpenGlDebugContext();

    return desc;
}

struct mesh {
    struct gfx_sid sid;
};

struct mesh
create_mesh()
{
    const struct gfx_sid sid = {
        .attr = SID_VPOSI2 | SID_SCALE2 | SID_COLOR3,
        .unif = SID_PROJMAT4X4,
        .out = SID_COLOR3,
        .pass = SID_COLOR3,
        .oper = SID_NULL,
    };

    struct mesh mesh = {.sid = sid};

    // register sid: registration excludes duplicates of "sid"
    shader_create_input(sid, "", VEC2F, SID_VPOSI2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS);
    // shader_create_input(sid, "", VEC2F, SID_SCALE2_IDI, IN_VSHADER|IS_ATTR|IS_TRANSFORM_SCALE);
    shader_create_input(sid, "", VEC2F, SID_ROTAT2_IDI, IN_VSHADER|IS_ATTR|IS_TRANSFORM_ROT);
    // shader_create_input(sid, "", MAT4X4F, SID_PROJMAT4X4_IDI, IN_VSHADER|IS_ATTR);
    shader_create_input(sid, "", VEC3F, SID_WPOSI3_IDI, IN_VSHADER|IS_ATTR|IS_TRANSFORM_WPOS);
    shader_create_input(sid, "", VEC4F, SID_COLOR4_IDI, IN_VSHADER | IS_ATTR | IS_OUT);
    // shader_create_input(sid, "", MAT4X4F, SID_PROJMAT4X4_IDI, IN_VSHADER|IS_UNIF|IS_TRANSFORM_PROJ);

    /// TESTS
    // shader_create_input(sid, "TEST", VEC2F, INT_NULL, IN_VSHADER|IS_UNIF);
    // shader_create_input(sid, "", VEC2F, SID_VPOSI2_, IN_VSHADER|IS_ATTR);
    // shader_create_input(sid, "", VEC3F, SID_WPOSI3_, IN_VSHADER|IS_ATTR);
    // shader_create_input(sid, "", VEC2F, SID_SCALE2_, IN_VSHADER|IS_ATTR|IS_OUT);

    // shader_create_input(sid, "", VEC4F, SID_COLOR4_IDI, IN_VSHADER|IS_ATTR|IS_OUT);
    // shader_create_input(sid, "", VEC2F, SID_VPOSI2_IDI, IN_VSHADER|IS_ATTR|IS_TRANSFORM_VPOS);
    // shader_create_input(sid, "", VEC3F, SID_WPOSI3_IDI, IN_VSHADER|IS_ATTR|IS_TRANSFORM_WPOS);
    // shader_create_input(sid, "", VEC2F, SID_SCALE2_IDI, IN_VSHADER|IS_ATTR|IS_TRANSFORM_SCALE);
    // shader_create_input(sid, "", VEC4F, SID_COLOR4_IDI, IN_VSHADER|IS_ATTR|IS_OUT);
    // shader_create_input(sid, "", VEC2F, SID_VPOSI2_IDI, IN_VSHADER|IS_ATTR|IS_TRANSFORM_VPOS);
    // shader_create_input(sid, "", VEC3F, SID_WPOSI3_IDI, IN_VSHADER|IS_ATTR|IS_TRANSFORM_WPOS);
    // shader_create_input(sid, "", VEC2F, SID_SCALE2_IDI, IN_VSHADER|IS_ATTR|IS_TRANSFORM_SCALE);
    // shader_create_input(sid, "", VEC3F, SID_WPOSI3_IDI, IN_VSHADER|IS_ATTR|IS_TRANSFORM_WPOS);
    // shader_create_input(sid, "", MAT4X4F, SID_PROJMAT4X4_IDI, IN_VSHADER|IS_ATTR);

    // temp_print_builds();

    return mesh;
}


int
main(void)
{
    memman_init();
    sparse_arr_init_table(32);
    shader_builder_init();


    struct scene* scene = scene_create("scene 1");
    struct layer layer1 = {
        .name = "layer1",
    };
    layer_create_layer(&layer1);
    scene_push_layer(scene, layer1);

    struct mesh mesh = create_mesh();

    struct shader_descriptor desc = gfx_generate_context(mesh.sid);


    vaidx_t vaoidx = gfx_vertex_array_create();
    desc.vaoidx = vaoidx;
    GL_CHECK();


    glDisable(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    gl_viewport(640, 480);

    // float vertices[] = {
    //     //vpos2       color4
    //     -0.5f, -0.5f, 1.f, 0.f, 0.f, 1.0,
    //      0.5f, -0.5f, 0.f, 1.f, 0.f, 1.0,
    //     -0.5f,  0.5f, 0.f, 0.f, 1.f, 1.0,
    //
    //     // -0.5f,  0.5f, 0.f, 0.f, 1.f, 1.0,
    //     //  0.5f, -0.5f, 0.f, 1.f, 0.f, 1.0,
    //     //  0.5f,  0.5f, 0.f, 1.f, 1.f, 1.0,
    //
    //     // 0.5f, -0.5f, 0.f, 1.f, 0.f, 1.0,
    //     // 0.5f, 0.5f, 0.f, 1.f, 1.f, 1.0,
    //     // 1.0f, 0.0f, 1.f, 1.f, 1.f, 1.0,
    //     //
    //     // -0.5f, 0.5f, 0.f, 0.f, 1.f, 1.0,
    //     // 0.5f, 0.5f, 0.f, 1.f, 1.f, 1.0,
    //     // 0.0f, 1.0f, 1.f, 1.f, 1.f, 1.0
    // };

    // float vertices[] = {
    //     //vpos2       color4
    //     -0.5f, -0.5f, 1.f, 0.f, 0.f, 1.0,
    //      0.5f, -0.5f, 0.f, 1.f, 0.f, 1.0,
    //     -0.5f,  0.5f, 0.f, 0.f, 1.f, 1.0,
    // };

    // float vertices[] = {
    //     //vpos2        scale2    color4
    //      -0.5f, -0.5f, 2.0, 2.0, 1.f, 0.f, 0.f, 1.0,
    //       0.5f, -0.5f, 1.0, 1.0, 0.f, 1.f, 0.f, 1.0,
    //       0.0f,  0.5f, 1.0, 1.0, 0.f, 0.f, 1.f, 1.0
    // };
    // float vertices[] = {
    //     //vpos2        scale2    wpos3          color4
    //      -0.5f, -0.5f, 1.0, 1.0, 0.6, 0.0, 0.0, 1.f, 0.f, 0.f, 1.0, // left
    //       0.5f, -0.5f, 1.0, 1.0, 0.4, 0.0, 0.0, 0.f, 1.f, 0.f, 1.0, // right
    //       0.0f,  0.5f, 1.0, 2.0, 0.3, 0.0, 0.0, 0.f, 0.f, 1.f, 1.0  // top
    // };
    vec2f r = {.x = cos(0), .y = sin(0)};
    // float vertices[] = {
    //     //vpos2        scale2    rot2      wpos3          color4
    //      -0.5f, -0.5f, 1.0, 1.0, r.x, r.y, 0.0, 0.0, 0.0, 1.f, 0.f, 0.f, 1.0, // left
    //       0.5f, -0.5f, 1.0, 1.0, r.x, r.y, 0.0, 0.0, 0.0, 0.f, 1.f, 0.f, 1.0, // right
    //       0.0f,  0.5f, 1.0, 1.0, r.x, r.y, 0.0, 0.0, 0.0, 0.f, 0.f, 1.f, 1.0  // top
    // };
    float vertices[] = {
        //vpos2        rot2      wpos3          color4
         -0.5f, -0.5f, r.x, r.y, 0.0, 0.0, 0.0, 1.f, 0.f, 0.f, 1.0, // left
          0.5f, -0.5f, r.x, r.y, 0.0, 0.0, 0.0, 0.f, 1.f, 0.f, 1.0, // right
          0.0f,  0.5f, r.x, r.y, 0.0, 0.0, 0.0, 0.f, 0.f, 1.f, 1.0  // top
    };


    const uint32 elem_count = ARRAY_LENGTH(vertices);

    // uint32 vbidx = gfx_vertex_buffer_create(mesh.sid, sizeof(vertices));
    uint32 vbidx = gfx_vertex_buffer_create(mesh.sid, sizeof(vertices), elem_count, desc.vshader_attribs_stride);
    gfx_vertex_buffer_set_data(0, (void*) &vertices);
    gfx_vertex_buffer_update_data(vbidx);

    print("---------------------\n");
    print("%sVERTICES-size:%u%s\n", YELLOW, sizeof(vertices), CLEAR);
    print("%sVERTICES-count:%u%s\n", YELLOW, elem_count, CLEAR);
    print("%sVERTICES-stride:%u%s\n", YELLOW, desc.vshader_attribs_stride, CLEAR);


    // struct shader_build build = shader_builds_get_build(0);
    shader_attributes_enable(desc.vshader_attrib_locations_count, desc.vshader_attribs_stride, desc.last_loc_num_elems);
    opengl_debug_attributes_in(desc.vshader_attrib_locations_count);



    struct opengl_program prog = opengl_program_get_program_byidx(desc.progidx);
    struct vertex_array_object va = gfx_vertex_array_get_byidx(desc.vaoidx);
    struct vertex_buffer_object vb = vertex_buffer_get_vbo(vbidx);
    renderer_renderqueue_add(prog.program, desc.progidx, va.vao, 0, vb.vbo, vbidx, elem_count);

    {
        // After shader_attributes_enable, verify the state:
        glBindVertexArray(va.vao); // Your VAO
        //
        // GLint bound_vbo;
        // glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &bound_vbo);
        // print("VBO bound after attribute setup: %d\n", bound_vbo);
        //
        // GLint attrib0_enabled, attrib1_enabled;
        // glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &attrib0_enabled);
        // glGetVertexAttribiv(1, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &attrib1_enabled);
        // print("Attrib0 enabled: %d, Attrib1 enabled: %d\n", attrib0_enabled, attrib1_enabled);
        //
        // GLint64 attrib0_offset;
        // GLint attrib0_stride;
        // glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &bound_vbo);
        // glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &attrib0_stride);
        // glGetVertexAttribPointerv(0, GL_VERTEX_ATTRIB_ARRAY_POINTER, (void*) &attrib0_offset);
        // print("Attrib0:  Stride: %d, Offset: %p\n", attrib0_stride, attrib0_offset);
        //
        // GLint64 attrib1_offset;
        // GLint attrib1_stride;
        // glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &bound_vbo);
        // glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &attrib1_stride);
        // glGetVertexAttribPointerv(0, GL_VERTEX_ATTRIB_ARRAY_POINTER, (void*) &attrib1_offset);
        // print("Attrib1:  Stride: %d, Offset: %p\n", attrib0_stride, attrib1_offset);


        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        print("\nViewport: x=%d y=%d w=%d h=%d\n", viewport[0], viewport[1], viewport[2], viewport[3]);

        print(""
              "program: %u, progidx:%u\n"
              "va: %u, vaoidx:%u\n"
              "vb: %u, vboidx:%u\n"
              "\n",
              prog.program, prog.idx,
              va.vao, va.idx,
              vb.vbo, vb.idx
        );

        // After gfx_vertex_buffer_update_data, read back the data
        glBindBuffer(GL_ARRAY_BUFFER, vb.vbo);
        float* readback = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
        if(readback) {
            print("VBO contents:");
            for(int i = 0; i < elem_count; i++) {
                float val = readback[i];
                if(i % desc.vshader_attribs_stride == 0)
                    print("\n");

                if(val >= 0) print("  [%2d]:  %.2f,", i, val);
                else print("  [%2d]: %.2f,", i, val);
            }
            print("\n\n");
            glUnmapBuffer(GL_ARRAY_BUFFER);
        } else {
            print("Failed to map buffer\n");
        }
    }

    run();

    sparse_arr_destroy_table();
    shader_builder_destroy();
    memman_terminate();

    return 0;
}
