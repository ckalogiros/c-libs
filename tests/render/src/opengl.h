/*
 * Created by ckalo on 4/4/26.
 */
#ifndef ENGINE_TEST_OPENGL_H
#define ENGINE_TEST_OPENGL_H

#include "glad.h"
#include "my_types.h"


GLuint gl_create_program();

void gl_use_program(GLuint program);

void gl_delete_program(GLuint program);

void gl_link_program(GLuint program);


void gl_gen_vertex_array(GLuint n, GLuint* vao);

void gl_bind_vertex_array(GLuint vao);

void gl_delete_attrib_array(GLsizei n, const GLuint* arrays);


void gl_enable_attrib_array(GLint loc);

void gl_enable_attrib_pointer(GLint loc, GLuint size, GLuint stride, GLuint vertex_offset);

void gl_vertex_attrib_divisor(GLuint index, GLuint divisor);

void gl_enable_vertex_attrib_array(GLuint index);

void gl_disable_vertex_attrib_array(GLuint index);

void gl_draw_arrays(GLenum mode, GLint start_index, GLsizei num_vertices);

void gl_draw_elements(GLenum mode, GLsizei count, GLenum type, const void* indices);

void gl_draw_arrays_instanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);

void gl_draw_elements_instanced(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount);


void gl_viewport(GLuint width, GLuint height);

void gl_clear();

void gl_clear_color(const vec4f col);


GLint gl_get_uniform_location(GLuint program, const char* loc_name);

void gl_uniform_matrix_4fv(GLint loc, const GLfloat* mvp);


GLint gl_get_attribute_location(GLuint program, const char* loc_name);

void gl_bind_attribute_location(GLuint program, GLuint index, const char* loc_name);


void gl_gen_buffers(GLsizei n, GLuint* buffer);

void gl_bind_buffer(GLenum target, GLuint buffer);

void gl_buffer_data(GLenum target, GLuint size, void* vertices, GLenum usage);

void gl_bind_buffer_base(GLenum target, GLuint index, GLuint buffer);

void gl_delete_buffers(GLsizei n, GLuint* buffer);

void gl_buffer_sub_data(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);


GLenum gl_create_shader(GLenum shaderType);

void gl_shader_source(GLuint shader, GLsizei n, const GLchar** shader_source, const GLint* length);

void gl_compile_shader(GLuint shader);

void gl_delete_shader(GLuint shader);

void gl_attach_shader(GLuint program, GLuint shader);

void gl_detach_shader(GLuint program, GLuint shader);

void gl_generate_framebuffers(GLsizei n, GLuint* framebuffers);

void gl_delete_framebuffers(GLsizei n, const GLuint* framebuffers);

void gl_generate_renderbuffers(GLsizei n, GLuint* renderbuffers);

void gl_delete_renderbuffers(GLsizei n, const GLuint* renderbuffers);

void gl_framebuffer_renderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

void gl_framebuffer_texture(GLenum target, GLenum attachment, GLuint texture, GLint level);

void gl_framebuffer_texture_1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

void gl_framebuffer_texture_2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

void gl_framebuffer_texture_3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);

void gl_framebuffer_texture_(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);


void gl_generate_samplers(GLsizei n, GLuint* samplers);

void gl_delete_samplers(GLsizei n, GLuint* samplers);

void gl_get_sampler_param_Ii32v(GLuint sampler, GLenum pname, GLint* params);

void gl_get_sampler_param_Iu32v(GLuint sampler, GLenum pname, GLuint* params);

void gl_get_sampler_param_f32v(GLuint sampler, GLenum pname, GLfloat* params);

void gl_get_sampler_param_i32v(GLuint sampler, GLenum pname, GLint* params);

void gl_set_sampler_param_Ii32v(GLuint sampler, GLenum pname, const GLint* param);

void gl_set_sampler_param_Iu32v(GLuint sampler, GLenum pname, const GLuint* param);

void gl_set_sampler_param_f32(GLuint sampler, GLenum pname, GLfloat param);

void gl_set_sampler_param_f32v(GLuint sampler, GLenum pname, const GLfloat* param);

void gl_set_sampler_param_i32(GLuint sampler, GLenum pname, GLint param);

void gl_set_sampler_param_i32v(GLuint sampler, GLenum pname, const GLint* param);


void gl_generate_textures(GLsizei n, GLuint* textures);

void gl_delete_textures(GLsizei n, const GLuint* textures);

void gl_select_active_texture(GLenum texture);

void gl_bind_texture(GLenum target, GLuint texture);


/* copy of opengl constants */
// Buffer usage hints explained:
//     GL_STREAM_DRAW - Write once, used once (no read)
//     GL_STREAM_READ - Read once, written once
//     GL_STREAM_COPY - Read and write, used once
//     GL_STATIC_DRAW - Write once, used many times (no read) ← You're likely here
//     GL_STATIC_READ - Read many times, written once
//     GL_STATIC_COPY - Read and write many times
//     GL_DYNAMIC_DRAW - Write many times, used many times (no read)
//     GL_DYNAMIC_READ - Read many times, written many times
//     GL_DYNAMIC_COPY - Read and write many times
#define OPENGL_DYNAMIC_COPY 0x88EA
#define OPENGL_DYNAMIC_DRAW 0x88E8
#define OPENGL_DYNAMIC_READ 0x88E9
#define DEBUG_GL_VERTEX_BUFFER
#ifdef DEBUG_GL_VERTEX_BUFFER
#   define OPENGL_STATIC_DRAW      OPENGL_DYNAMIC_COPY // FOR READING VERTEX BUFFER DATA
#else
#   define OPENGL_STATIC_DRAW      0x88E4 // GL_STATIC_DRAW
#endif

#define OPENGL_ELEMENT_ARRAY_BUFFER     0x8893 // GL_ELEMENT_ARRAY_BUFFER
#define OPENGL_ARRAY_BUFFER     0x8892 // GL_ARRAY_BUFFER
#define OPENGL_VERTEX_SHADER    0x8B31 // GL_VERTEX_SHADER
#define OPENGL_FRAGMENT_SHADER  0x8B30 // GL_FRAGMENT_SHADER
#define OPENGL_TRIANGLES        0x0004 // GL_TRIANGLES


#endif //ENGINE_TEST_OPENGL_H
