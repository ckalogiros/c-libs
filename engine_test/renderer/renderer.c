/*
 * Created by ckalo on 4/2/26.
 */

#include "renderer.h"
#include "my_types.h"
#include "shaders.h"



GLuint renderer_create_vb(Vertex* vertices, const uint32 size)
{
    GLuint vbo;
    gl_gen_buffers(1, &vbo);
    gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo);
    gl_buffer_data(OPENGL_ARRAY_BUFFER, size, vertices, OPENGL_STATIC_DRAW);

    return vbo;
}
void renderer_bind_vb(GLuint vbo){ gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo); }
void renderer_update_vb(Vertex* vertices)
{
    gl_buffer_data(OPENGL_ARRAY_BUFFER, sizeof(vertices), vertices, OPENGL_STATIC_DRAW);
}
void renderer_destroy_vb(GLuint* vbo){ gl_delete_buffers(1, vbo); }

void renderer_create_ib(){}
void renderer_bind_ib(){}
void renderer_update_ib(){}
void renderer_destroy_ib(){}

void renderer_create_texture(){}
void renderer_bind_texture(){}
void renderer_update_texture(){}
void renderer_destroy_texture(){}

progidx_t renderer_create_shader()
{
    progidx_t progidx = opengl_program_create(vertex_shader_text, fragment_shader_text);
    progidx_t program = opengl_program_get_byidx(progidx);
    return program;
}
void renderer_destroy_shader(){}
