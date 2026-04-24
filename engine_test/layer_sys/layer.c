/*
 * Created by ckalo on 3/28/26.
 */

#include "layer.h"
#include <unistd.h>
// #include "glfw3.h"
#include <stdlib.h>

#include "linmath.h"
#include "logger.h"
#include "my_types.h"
#include "opengl_debug.h"
#include "window.h"


#define STACK_SIZE 4


struct layer layer_stack_buffer[STACK_SIZE];
uint32 stack_layer_count = 0;

struct layer* struct_layer_get_byid(const uint32 layer_id)
{
    return &layer_stack_buffer[layer_id];
}

void layer_stack_push(struct layer layer)
{
    const uint32 idx = stack_layer_count;
    layer_stack_buffer[idx] = layer;
    stack_layer_count++;
}

void layer_create_layer(struct layer* layer)
{
    /// TODO: possibly here we will implement
    // GLuint vbo = renderer_create_vb(layer->vertices, sizeof(layer->vertices));
    //
    progidx_t program = renderer_create_shader();
    // GLint mvp_location = gl_get_uniform_location(program, "projection_matrix");
    // // GLint vpos_location = gl_get_attribute_location(program, "vPos");
    // // GLint vcol_location = gl_get_attribute_location(program, "vCol");
    //
    // GLuint vao;
    // gl_gen_vertex_array(1, &vao);
    // gl_bind_vertex_array(vao);
    //
    // gl_enable_attrib_array(0);
    // gl_enable_attrib_pointer(0, 4, (2+2+3)*4, 0);
    // gl_enable_attrib_array(1);
    // gl_enable_attrib_pointer(1, 3, (2+2+3)*4, 4*4);
    // OpenGlDebugContext();
    //
    // layer->program = program;
    // layer->vao = vao;
    // layer->vbo = vbo;
    // layer->mvp_loc = mvp_location;
    // OpenGlDebugContext();

}

void layer_on_event(){}
void layer_on_update(struct layer* layer)
{
    int width = window_get_width();
    int height = window_get_height();
    const float ratio = (float) width / (float) height;
    const float time = (float) glfwGetTime();

    /// layer meshes update
    mat4x4 m, p, mvp;
    mat4x4_identity(mvp);
    // mat4x4_identity(m);
    // float x = sin(time)*.2;
    // if (x < .0) x *= -1;
    //
    // mat4x4_translate(m, x, 0.0f, 0.0f);
    // float scale = 2.0f * x+1.;
    // m[0][0] *= scale;
    // m[1][1] *= scale;
    // m[2][2] *= scale;
    // mat4x4_scale(m, m, 1.0f);
    //
    // mat4x4_rotate_Z(m, m, time*time);


    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);

    // mat4x4_mul(mvp, p, m);
    mat4x4_mul(mvp, mvp, p);

    /// layer meshes draw
    gl_use_program(layer->program);
    gl_uniform_matrix_4fv(layer->mvp_loc, (const GLfloat*) &mvp);
    gl_bind_vertex_array(layer->vao);
}
void layer_on_render(struct layer* layer)
{
    gl_draw_arrays(OPENGL_TRIANGLES, 0, 3);
}
