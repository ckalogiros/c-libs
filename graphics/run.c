/*
 * Created by ckalo on 4/22/26.
 */

#include "run.h"

#include <math.h>

#include "gfx_buffers.h"
#include "my_types.h"
#include "opengl.h"
#include "opengl_debug.h"
#include "window.h"

struct render_queue render_queue[8];
uint32 render_queue_count = 0;


struct render_queue*
render_queue_get() { return render_queue; }

uint32
render_queue_get_count() { return render_queue_count; }

struct render_queue*
renderer_renderqueue_add(
    progidx_t program,
    progidx_t progidx,
    vbidx_t vao,
    vbidx_t vaidx,
    vbidx_t vbo,
    vbidx_t vbidx,
    uint32 vertex_count)
{
    uint32 idx = render_queue_count;
    render_queue_count++;

    struct render_queue* rq = &render_queue[idx];
    rq->program = program;
    rq->progidx = progidx;
    rq->vao = vao;
    rq->vbo = vbo;
    rq->vbidx = vbidx;

    rq->vertex_count = vertex_count;

    return rq;
}


void
draw()
{
    const float t = glfwGetTime();

    for(uint32 i = 0; i < render_queue_count; i++) {

        vec2f anglvel = {.x = cos(t)*1., .y = sin(t)*1.};
        vec3f wpos = {.x = cos(t)*.5, .y = 0};

        struct render_queue rq = render_queue[i];
        gl_use_program(rq.program);
        GL_CHECK();

        gl_bind_vertex_array(rq.vao);
        GL_CHECK();

        gfx_vertex_buffer_set_rotation(rq.vbidx, anglvel);
        gfx_vertex_buffer_set_wpos(rq.progidx, rq.vbidx, wpos);

        gl_bind_buffer(GL_ARRAY_BUFFER, rq.vbo);
        gfx_vertex_buffer_update_data(rq.vbidx);
        GL_CHECK();

        gl_draw_arrays(OPENGL_TRIANGLES, 0, 3);
        // gl_draw_arrays(OPENGL_TRIANGLES, 0, rq.vertex_count);
        GL_CHECK();
    }
}

// void draw() {
//     for(uint32 i = 0; i < render_queue_count; i++) {
//         struct render_queue rq = render_queue[i];
//
//         print("Drawing: program=%u, vao=%u, vbo=%u\n", rq.program, rq.vao, rq.vbo);
//
//         glUseProgram(rq.program);
//
//         // Verify program is active
//         GLint current_prog;
//         glGetIntegerv(GL_CURRENT_PROGRAM, &current_prog);
//         print("Current program: %d (expected: %u)\n", current_prog, rq.program);
//
//         glBindVertexArray(rq.vao);
//
//         // Verify VAO is bound
//         GLint bound_vao;
//         glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &bound_vao);
//         print("Bound VAO: %d (expected: %u)\n", bound_vao, rq.vao);
//
//         print("Calling glDrawArrays...\n");
//         glDrawArrays(GL_TRIANGLES, 0, 3);
//         print("glDrawArrays completed\n");
//
//         // Check for errors immediately after draw
//         GLenum err = glGetError();
//         if(err != GL_NO_ERROR) {
//             print("OpenGL Error after draw: 0x%x\n", err);
//         }
//     }
// }

void
run()
{
    vec4f color = {.r = .1, .g = .1, .b = .1, .a = 1.0};

    while(1) {
        // const float t = glfwGetTime();
        // color.r = sin(t)*.9;
        // color.g = cos(t)*.5;

        window_poll_events();

        if(window_should_close()) {
            window_terminate();
            break;
        }

        gl_clear();
        gl_clear_color(color);

        draw();
        window_swap_buffers();

        // OpenGlDebugContext();
    }
}
