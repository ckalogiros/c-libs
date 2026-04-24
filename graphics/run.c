/*
 * Created by ckalo on 4/22/26.
 */

#include "run.h"

#include <math.h>

#include "my_types.h"
#include "opengl.h"
#include "opengl_debug.h"
#include "window.h"

struct render_queue render_queue[8];
uint32 render_queue_count = 0;


struct render_queue* render_queue_get(){ return render_queue; }
uint32 render_queue_get_count(){ return render_queue_count; }

struct render_queue* renderer_renderqueue_add(
                                    progidx_t program,
                                    progidx_t progidx,
                                    vbidx_t vao,
                                    vbidx_t vaidx,
                                    vbidx_t vbo,
                                    vbidx_t vbidx)
{
    uint32 idx = render_queue_count;
    render_queue_count++;

    struct render_queue rq = render_queue[idx];
    rq.program = program;
    rq.progidx = progidx;
    rq.vao = vao;
    rq.vbo = vbo;
    rq.vbidx = vbidx;

    return &rq;
}



void draw()
{
    for(uint32 i = 0; i < render_queue_count; i++) {

        struct render_queue rq = render_queue[i];

        gl_use_program(rq.program);
        // gl_uniform_matrix_4fv(layer->mvp_loc, (const GLfloat*) &mvp);
        gl_bind_vertex_array(rq.vao);
        gl_bind_buffer(GL_ARRAY_BUFFER, rq.vbo);

        // gl_draw_arrays(OPENGL_TRIANGLES, 0, 3);
    }
}

void run()
{
    vec4f color = {.r = .1, .g = .1, .b = .7, .a = 1.0};

    while (1)
    {
        const float t = glfwGetTime();
        color.r = sin(t)*.9;
        color.g = cos(t)*.5;
        // color.b = cos(t)*.5;

        window_poll_events();

        if (window_should_close())
        {
            window_terminate();
            break;
        }

        gl_clear();
        gl_clear_color(color);

        // for(uint32 i = 0; i < scene->layer_stack.count; i++) {
        //     struct layer* layer = &scene->layer_stack.layer[i];
        //     layer_on_update(layer);
        //     layer_on_render(layer);
        // }

        draw();


        window_swap_buffers();

        OpenGlDebugContext();

        // OpenGlDebugContext();
        // float currentTime = GetTime();
        // float timestep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
        // lastTime = currentTime;
        //
        // // Main layer update here
        // for (const std::unique_ptr<Layer>& layer : m_LayerStack)
        //     layer->OnUpdate(timestep);
        //
        // // NOTE: rendering can be done elsewhere (eg. render thread)
        // for (const std::unique_ptr<Layer>& layer : m_LayerStack)
        //     layer->OnRender();
        //
        // m_Window->Update();
    }
}
