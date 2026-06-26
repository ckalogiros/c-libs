/*
 * Created by ckalo on 4/22/26.
 */

#include "run.h"

#include <math.h>

#include "command.h"
#include "gfx_buffers.h"
#include "my_types.h"
#include "opengl.h"
#include "opengl_debug.h"
#include "precision_timer.h"
#include "test.h"
#include "window.h"

struct render_queue render_queue[8];
uint32 render_queue_count = 0;


struct render_queue*
render_queue_get(const uint32 rqidx) { return &render_queue[rqidx]; }

uint32
render_queue_get_count() { return render_queue_count; }

void
render_queue_add_draw_count(struct command_render_queue_params* params)
{
    struct object_components_descriptor* comp_desc =  test_get_comp_desc(params->comp_idx);
    struct render_queue* rq = &render_queue[comp_desc->rqidx];
    rq->draw_count += params->vertex_count;
    print("%s- Renderqueue draw count-\n  rqidx:%u, add:%u, total:%u%s\n", br2, comp_desc->rqidx, params->vertex_count, rq->draw_count, CLEAR);
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
    rq->vaidx = vaidx;
    rq->vbo = vbo;
    rq->vbidx = vbidx;
    rq->draw_count = 0;

    return idx;
}

void opengl_debug_attributes(const uint32 i){
    // DEBUG: Check enabled attributes BEFORE binding
    GLint maxAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
    print("Before draw %d - Enabled attributes: ", i);
    for(int j = 0; j < maxAttribs; j++) {
        GLint enabled;
        glGetVertexAttribiv(j, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
        if(enabled) print("%d ", j);
    }
    print("\n");
}
void opengl_debug_vbo_size(const uint32 i, struct render_queue* rq){
    // Get the VBO bound to this VAO
    GLint current_vbo;
    glBindVertexArray(rq->vao);
    glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &current_vbo);

    // Get VBO size
    GLint vbo_size;
    glBindBuffer(GL_ARRAY_BUFFER, current_vbo);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vbo_size);

    // Calculate required size (assuming 4 attributes of 3 floats each = 12 floats per vertex)
    int vertex_size = 12 * sizeof(float); // Adjust based on your actual vertex structure
    int required_size = rq->draw_count * vertex_size;

    print("Object %d: VAO=%u, VBO=%u, VBO size=%d bytes, draw_count=%d, required=%d bytes\n",
           i, rq->vao, current_vbo, vbo_size, rq->draw_count, required_size);

    if (vbo_size < required_size) {
        print("ERROR: VBO too small! Need %d bytes but have %d\n", required_size, vbo_size);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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


        // opengl_debug_attributes(i);
        // opengl_debug_vbo_size(i, &rq);


        gl_use_program(rq.program);
        GL_CHECK();

        gl_bind_vertex_array(rq.vao);
        GL_CHECK();

        gfx_vertex_buffer_set_rotation(rq.progidx, rq.vbidx, anglvel);
        // gfx_vertex_buffer_set_wpos(rq.progidx, rq.vbidx, wpos);
        // gfx_vertex_buffer_add_wpos(rq.progidx, rq.vbidx, wpos);
        // gfx_vertex_buffer_set_scale(rq.progidx, rq.vbidx, scale);

        gl_bind_buffer(GL_ARRAY_BUFFER, rq.vbo);
        gfx_vertex_buffer_update_data(&(struct vb_cmd_params){.vbidx = rq.vbidx});
        GL_CHECK();

        gl_draw_arrays(OPENGL_TRIANGLES, 0, rq.draw_count);
        // gl_draw_arrays(OPENGL_TRIANGLES, 0, 6);
        // gl_draw_arrays(OPENGL_TRIANGLES, 0, 36);
        GL_CHECK();

        // glBindVertexArray(0);
    }
}

void
run(struct timer* loop_timer)
{
    vec4f color = {.r = .2, .g = .2, .b = .2, .a = 1.0};
    // vec4f color = {.r = .6, .g = .6, .b = .6, .a = 1.0};


    while(1) {

        window_poll_events();

        if(window_should_close()) {
            window_terminate();
            break;
        }

        gl_clear();
        gl_clear_color(color);

        cmd_run();
        // perf_timer_start(loop_timer);

        draw();
        // perf_timer_stop(loop_timer);
        window_swap_buffers();


        // uint32 fps = perf_timer_get_fps(loop_timer);
        // print("%u\n", fps);

    }
}
