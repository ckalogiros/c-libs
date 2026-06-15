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
render_queue_get(const uint32 rqidx) { return &render_queue[rqidx]; }

uint32
render_queue_get_count() { return render_queue_count; }

void
render_queue_add_draw_count(const uint32 rqidx, const uint32 vertex_count)
{
    struct render_queue* rq = &render_queue[rqidx];
    rq->draw_count += vertex_count;
    print("%s- Renderqueue: add draw count:  add:%u, total:%u%s\n", br2, vertex_count, rq->draw_count, CLEAR);
}

uint32
renderer_renderqueue_add(
    const build_t program,
    const build_t progidx,
    const vbidx_t vao,
    const vbidx_t vaidx,
    const vbidx_t vbo,
    const vbidx_t vbidx)
{
    uint32 idx = render_queue_count;
    render_queue_count++;

    struct render_queue* rq = &render_queue[idx];
    rq->program = program;
    rq->progidx = progidx;
    rq->vao = vao;
    rq->vbo = vbo;
    rq->vbidx = vbidx;
    rq->draw_count = 0;

    return idx;
}


void
draw()
{
    const float t = glfwGetTime()*1.;
    vec2f64 mouse_pos = io_mouse_get_pos();

    for(uint32 i = 0; i < render_queue_count; i++) {

        vec2f anglvel = {.x = cos((float)mouse_pos.x*.01), .y = sin((float)mouse_pos.x*.01)};
        // vec2f anglvel = {.x = cos(t), .y = sin(t)};
        // vec2f anglvel = {.x = cos(t*20)*1, .y = sin(t*20)*1};
        // vec2f anglvel = {.x = cos(t)*1., .y = sin(t)*1.};
        // vec2f anglvel = {.x = t*4, .y = 0};

        // vec3f wpos = {.x = cos(t)*.5, .y = 0};
        // vec3f wpos = {.x = cos(t)*.0001, .y = 0, .z=0};
        // vec3f wpos = {.x = cos(t)*.0, .y = 0, .z=0};
        // vec3f wpos = {.x = 0, .y = 0, .z=mouse_pos.y};
        vec3f wpos = {.x = 100, .y = 100, .z=mouse_pos.y*.03};

        vec2f scale = {.x = (cos(t*10)+1.4)*1., .y = (cos(t*10)+1.4)*1.};

        struct render_queue rq = render_queue[i];
        gl_use_program(rq.program);
        GL_CHECK();

        gl_bind_vertex_array(rq.vao);
        GL_CHECK();

        gfx_vertex_buffer_set_rotation(rq.progidx, rq.vbidx, anglvel);
        // gfx_vertex_buffer_set_wpos(rq.progidx, rq.vbidx, wpos);
        // gfx_vertex_buffer_add_wpos(rq.progidx, rq.vbidx, wpos);
        // gfx_vertex_buffer_set_scale(rq.progidx, rq.vbidx, scale);

        gl_bind_buffer(GL_ARRAY_BUFFER, rq.vbo);
        gfx_vertex_buffer_update_data(rq.vbidx);
        GL_CHECK();

        gl_draw_arrays(OPENGL_TRIANGLES, 0, rq.draw_count);
        // gl_draw_arrays(OPENGL_TRIANGLES, 0, 36);
        // gl_draw_arrays(GL_LINE_STRIP, 0, rq.vertex_count);
        GL_CHECK();
    }
}

void
run()
{
    vec4f color = {.r = .3, .g = .3, .b = .3, .a = 1.0};
    // vec4f color = {.r = .6, .g = .6, .b = .6, .a = 1.0};

    while(1) {

        window_poll_events();

        if(window_should_close()) {
            window_terminate();
            break;
        }

        gl_clear();
        gl_clear_color(color);

        draw();
        window_swap_buffers();

    }

}
