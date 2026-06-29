/*
 * Created by ckalo on 4/4/26.
 */
#ifndef ENGINE_TEST_OPENGL_H
#define ENGINE_TEST_OPENGL_H

#include <glad/gl.h>
#include "gfx_interface.h"
#include "my_types.h"
#include "shader_descriptor.h"


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


void gl_viewport(GLuint width, GLuint height);

void gl_clear();

void gl_clear_color(const vec4f col);


GLint gl_get_uniform_location(GLuint program, const char* loc_name);

void gl_uniform_matrix_4fv(GLint loc, const GLfloat* mvp);


GLint gl_get_attribute_location(GLuint program, const char* loc_name);

void gl_bind_attribute_location(GLuint program, GLuint index, const char* loc_name);


void gl_gen_buffers(GLsizei n, GLuint* buffer);

void gl_bind_buffer(GLenum target, GLuint buffer);

void gl_buffer_data(GLenum target, GLuint size, void* const vertices, GLenum usage);

void gl_delete_buffers(GLsizei n, GLuint* buffer);

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

#define OPENGL_ARRAY_BUFFER     0x8892 // GL_ARRAY_BUFFER
#define OPENGL_VERTEX_SHADER    0x8B31 // GL_VERTEX_SHADER
#define OPENGL_FRAGMENT_SHADER  0x8B30 // GL_FRAGMENT_SHADER
#define OPENGL_TRIANGLES        0x0004 // GL_TRIANGLES


// #define GL_POINT 0x1B00
// #define GL_POINTS 0x0000
// #define GL_POINT_BIT 0x00000002
// #define GL_POINT_DISTANCE_ATTENUATION 0x8129
// #define GL_POINT_FADE_THRESHOLD_SIZE 0x8128
// #define GL_POINT_SIZE 0x0B11
// #define GL_POINT_SIZE_GRANULARITY 0x0B13
// #define GL_POINT_SIZE_MAX 0x8127
// #define GL_POINT_SIZE_MIN 0x8126
// #define GL_POINT_SIZE_RANGE 0x0B12
// #define GL_POINT_SMOOTH 0x0B10
// #define GL_POINT_SMOOTH_HINT 0x0C51
// #define GL_POINT_SPRITE 0x8861
// #define GL_POINT_SPRITE_COORD_ORIGIN 0x8CA0
// #define GL_POINT_TOKEN 0x0701

// #define GL_TRIANGLES 0x0004
// #define GL_TRIANGLES_ADJACENCY 0x000C
// #define GL_TRIANGLE_FAN 0x0006
// #define GL_TRIANGLE_STRIP 0x0005
// #define GL_TRIANGLE_STRIP_ADJACENCY 0x000D

// #define GL_LINES 0x0001
// #define GL_LINES_ADJACENCY 0x000A
// #define GL_LINE_BIT 0x00000004
// #define GL_LINE_LOOP 0x0002
// #define GL_LINE_RESET_TOKEN 0x0707
// #define GL_LINE_SMOOTH 0x0B20
// #define GL_LINE_SMOOTH_HINT 0x0C52
// #define GL_LINE_STIPPLE 0x0B24
// #define GL_LINE_STIPPLE_PATTERN 0x0B25
// #define GL_LINE_STIPPLE_REPEAT 0x0B26
// #define GL_LINE_STRIP 0x0003
// #define GL_LINE_STRIP_ADJACENCY 0x000B
// #define GL_LINE_TOKEN 0x0702
// #define GL_LINE_WIDTH 0x0B21
// #define GL_LINE_WIDTH_GRANULARITY 0x0B23
// #define GL_LINE_WIDTH_RANGE 0x0B22

struct opengl_program {
    GLint program;
    int32 build_idx;
};

struct opengl_program* shader_program_get(build_t progidx);

build_t gfx_program_exists(const sididx_t sid_idx);

struct shader_descriptor* opengl_program_create(const sididx_t sid_idx, const build_t build_idx, const char* fs);

struct opengl_program* opengl_program_get_program_byidx(const build_t build_idx);

#endif //ENGINE_TEST_OPENGL_H
