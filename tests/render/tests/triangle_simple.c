/*
 * Created by ckalo on 8/15/26.
 */

#include "tests.h"
#include "opengl.h"
#include "window.h"
#include "camera.h"
#include "GLOBALS.h"
#include "opengl_debug.h"
#include "my_macros.h"
#include "precision_timer.h"
#include "opengl_debug.h"
#include "opengl_perf.h"

// static GLuint64 start_time, stop_time;
// static GLuint query_id;
// static GLuint query_cnt;
// static uint64 query_avg;
// #define GL_PERF_INIT()      glGenQueries(2, query_id);
// #define GL_PERF_START()     glQueryCounter(query_id[0], GL_TIMESTAMP);
// #define GL_PERF_STOP()      glQueryCounter(query_id[1], GL_TIMESTAMP);
// #define GL_PERF_DELETE()    glDeleteQueries(2, query_id)
// #define GL_PERF_PRINT()\
//     glGetQueryObjectui64v(query_id[0], GL_QUERY_RESULT, &start_time);\
//     glGetQueryObjectui64v(query_id[1], GL_QUERY_RESULT, &stop_time);\
//     double timeElapsedMs = (double)(start_time - stop_time) / 1000000.0;\
//     print("GPU render time: %f ms\n", timeElapsedMs)\


static void draw(GLint program, GLint vao, uint32 draw_count)
{
    // const float t = glfwGetTime()*1.;
    gl_use_program(program);                            GFX_CHECK_ERROR();
    gl_bind_vertex_array(vao);                          GFX_CHECK_ERROR();
    gl_draw_arrays(OPENGL_TRIANGLES, 0, draw_count);    GFX_CHECK_ERROR();
    glBindVertexArray(0);
}

static void run(GLint program, GLint vao, uint32 draw_count)
{
    vec4f color = {.r = .1, .g = .1, .b = .1, .a = 1.0};
    gl_perf_create_timer();

    while(1) {

        window_poll_events();
        if(window_should_close()) {
            window_terminate();
            break;
        }

        gl_perf_timer_start();
        gl_clear();
        gl_clear_color(color);
        draw(program, vao, draw_count);
        gl_perf_timer_stop();

        window_swap_buffers();
        // gl_perf_timer_print();

    }
    gl_perf_average_print();

}

static void window_setup()
{
    window_create(SCREEN_RES_X, SCREEN_RES_Y);
    gl_viewport(SCREEN_RES_X, SCREEN_RES_Y);
}

static void opengl_setup()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_MULTISAMPLE);
}


void triangle_simple()
{
    window_setup();
    opengl_setup();

    gl_print_context_info();

    gl_debug_enable();

    // glEnable(0x91827654728);
    glEnable(0xDEADBEEF);

    GLuint program = 0;
    {
        const char* vshader = "\n\
#version 330 core\n\
layout (location = 0) in vec2 vpos2;\n\
layout (location = 1) in vec4 color4;\n\
layout (location = 2) in vec3 wpos3;\n\
uniform mat4 projMat;\n\
out vec4 out_color4;\n\
void main()\n\
{\n\
    vec4 vpos = vec4(vpos2.x+wpos3.x, vpos2.y+wpos3.y, wpos3.z, 1.0);\n\
    gl_Position = projMat * vpos;\n\
    out_color4 = vec4(color4.x, color4.y, color4.z, color4.w);\n\
}\n\
";

        const char* fshader = "\n\
#version 330\n\
in vec4 out_color4;\n\
out vec4 fragment;\n\
void main()\n\
{\n\
    fragment = out_color4;\n\
}\n";
        print("%s %s %s\n", ry, vshader, CLEAR);
        print("%s %s %s\n", ry, fshader, CLEAR);

        const GLuint vertex_shader = gl_create_shader(OPENGL_VERTEX_SHADER);
        gl_shader_source(vertex_shader, 1, &(vshader), NULL);
        gl_compile_shader(vertex_shader);
        GFX_CHECK_ERROR();

        const GLuint fragment_shader = gl_create_shader(OPENGL_FRAGMENT_SHADER);
        gl_shader_source(fragment_shader, 1, &fshader, NULL);
        gl_compile_shader(fragment_shader);
        GFX_CHECK_ERROR();

        program = gl_create_program();
        gl_attach_shader(program, vertex_shader);
        gl_attach_shader(program, fragment_shader);
        gl_link_program(program);
        GFX_CHECK_ERROR();

        gl_delete_shader(vertex_shader);
        gl_delete_shader(fragment_shader);
        GFX_CHECK_ERROR();

    }

    float attribs[] = {
        -1,  1, 0.9, 0.6, 0.1, 1.0, 0, 0, -8,
        -1, -1, 0.7, 0.1, 0.9, 1.0, 0, 0, -8,
         1, -1, 0.1, 0.7, 0.9, 1.0, 0, 0, -8,
    };
    uint32 typesize  = SIZE_F32,
           stride    = 2+4+3,
           vertices  = 3,
           attributes_elems = stride * vertices;

    GLuint vao, vbo;
    {
        gl_gen_vertex_array(1, &vao);
        gl_bind_vertex_array(vao);

        gl_gen_buffers(1, &vbo);
        gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo);
        gl_buffer_data(OPENGL_ARRAY_BUFFER, attributes_elems*typesize, attribs, OPENGL_STATIC_DRAW);

        int32 loc = -1, offset = -1, attrib_elems = -1;
        loc = 0, offset = 0, attrib_elems = 2;
        gl_enable_vertex_attrib_array(loc);                                                  GFX_CHECK_ERROR();
        gl_enable_attrib_pointer(loc, attrib_elems, stride * typesize, offset * typesize);   GFX_CHECK_ERROR();
        gl_vertex_attrib_divisor(loc, 0);                                                    GFX_CHECK_ERROR();

        loc = 1, offset = 2, attrib_elems = 4;
        gl_enable_vertex_attrib_array(loc);                                                  GFX_CHECK_ERROR();
        gl_enable_attrib_pointer(loc, attrib_elems, stride * typesize, offset * typesize);   GFX_CHECK_ERROR();
        gl_vertex_attrib_divisor(loc, 0);                                                    GFX_CHECK_ERROR();

        loc = 2, offset = 2+4, attrib_elems = 3;
        gl_enable_vertex_attrib_array(loc);                                                  GFX_CHECK_ERROR();
        gl_enable_attrib_pointer(loc, attrib_elems, stride * typesize, offset * typesize);   GFX_CHECK_ERROR();
        gl_vertex_attrib_divisor(loc, 0);
    }


    gl_use_program(program);

    struct camera camera;
    {
        /// create camera
        float aspect = (float)SCREEN_RES_X / (float)SCREEN_RES_Y;
        float near = 0.01f;
        float far = 100.0f;
        float fov = 45.0f;
        struct camera_projection_matrix projection_matrix =
            scene_create_perspective_projection_matrix(fov, near, far, aspect);

        camera = camera_create(projection_matrix, GL_FALSE);
        GLint uinif_loc = glGetUniformLocation(program, "projMat");
        if(uinif_loc != -1) {}
            glUniformMatrix4fv(uinif_loc, 1, camera.is_column_major, &camera.projection_matrix.projection_matrix[0]);
    }


    run(program, vao, vertices);


}
