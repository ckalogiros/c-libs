/*
 * Created by ckalo on 4/4/26.
 */

#define GLAD_GL_IMPLEMENTATION
#include "opengl.h"
#include "my_types.h"


GLuint gl_create_program()             { return glCreateProgram(); }
void gl_use_program(GLuint program)    { glUseProgram(program); }
void gl_delete_program(GLuint program) { glDeleteProgram(program); }
void gl_link_program(GLuint program)   { glLinkProgram(program); }


void gl_gen_vertex_array(GLuint n, GLuint* vao)               { glGenVertexArrays(n, vao); }
void gl_bind_vertex_array(GLuint vao)                         { glBindVertexArray(vao); }
void gl_delete_attrib_array(GLsizei n, const GLuint * arrays) { glDeleteVertexArrays(n, arrays); }


void gl_enable_attrib_array(GLint loc)                                                     { glEnableVertexAttribArray(loc); }
void gl_enable_attrib_pointer(GLint loc, GLuint size, GLuint stride, GLuint vertex_offset) { glVertexAttribPointer(loc, size, GL_FLOAT, GL_FALSE, stride, (void*)(uintptr_t) vertex_offset); }
void gl_vertex_attrib_divisor(GLuint index, GLuint divisor)                                { glVertexAttribDivisor(index, divisor); }
void gl_enable_vertex_attrib_array(GLuint index)                                           { glEnableVertexAttribArray(index); }
void gl_disable_vertex_attrib_array(GLuint index)                                          { glDisableVertexAttribArray(index); }


void gl_draw_arrays(GLenum mode, GLint start_index, GLsizei num_vertices)           { glDrawArrays(mode, start_index, num_vertices); }
void gl_draw_elements(GLenum mode, GLsizei count, GLenum type, const void* indices) { glDrawElements(mode, count, type, indices); }
void gl_draw_arrays_instanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount) { glDrawArraysInstanced(mode, first, count, instancecount); }
void gl_draw_elements_instanced(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount) { glDrawElementsInstanced(mode, count, type, indices, instancecount); }


void gl_viewport(GLuint width, GLuint height) { glViewport(0, 0, width, height); }
void gl_clear()                               { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
void gl_clear_color(const vec4f col)          { glClearColor(col.r, col.g, col.b, col.a); }


GLint gl_get_uniform_location(GLuint program, const char* loc_name) { return glGetUniformLocation(program, loc_name); }
void gl_uniform_matrix_4fv(GLint loc, const GLfloat* mvp)           { glUniformMatrix4fv(loc, 1, GL_FALSE, mvp); }


GLint gl_get_attribute_location(GLuint program, const char* loc_name)                { return glGetAttribLocation(program, loc_name); }
void  gl_bind_attribute_location(GLuint program, GLuint index, const char* loc_name) { glBindAttribLocation(program, index, loc_name); }

void gl_gen_buffers(GLsizei n, GLuint* buffer)                                { glGenBuffers(n, buffer); }
void gl_bind_buffer(GLenum target, GLuint buffer)                             { glBindBuffer(target, buffer); }
void gl_buffer_data(GLenum target, GLuint size, void* vertices, GLenum usage) { glBufferData(target, size, vertices, usage); }
void gl_bind_buffer_base(GLenum target, GLuint index, GLuint buffer)          { glBindBufferBase(target, index, buffer); }
void gl_delete_buffers(GLsizei n, GLuint* buffer)                             { glDeleteBuffers(n, buffer); }
void gl_buffer_sub_data(GLenum target, GLintptr offset, GLsizeiptr size, const void *data) { glBufferSubData(target, offset, size, data); }


GLenum gl_create_shader(GLenum shaderType)           { return glCreateShader(shaderType); }
void gl_shader_source(GLuint shader, GLsizei n, const GLchar** shader_source, const GLint* length) { glShaderSource(shader, n, shader_source, length); }
void gl_compile_shader(GLuint shader)                { glCompileShader(shader); }
void gl_delete_shader(GLuint shader)                 { glDeleteShader(shader); }
void gl_attach_shader(GLuint program, GLuint shader) { glAttachShader(program, shader); }
void gl_detach_shader(GLuint program, GLuint shader) { glDetachShader(program, shader); }


void gl_generate_framebuffers(GLsizei n, GLuint* framebuffers)       { glGenFramebuffers(n, framebuffers); }
void gl_delete_framebuffers(GLsizei n, const GLuint* framebuffers)   { glDeleteFramebuffers(n, framebuffers); }
void gl_generate_renderbuffers(GLsizei n, GLuint* renderbuffers)     { glGenRenderbuffers(n, renderbuffers); }
void gl_delete_renderbuffers(GLsizei n, const GLuint* renderbuffers) { glDeleteRenderbuffers(n, renderbuffers); }

void gl_framebuffer_renderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) { glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer); }
void gl_framebuffer_texture(GLenum target, GLenum attachment, GLuint texture, GLint level)                         {glFramebufferTexture(target, attachment, texture, level);}
void gl_framebuffer_texture_1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)    { glFramebufferTexture1D(target, attachment, textarget, texture, level);}
void gl_framebuffer_texture_2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)    { glFramebufferTexture2D(target, attachment, textarget, texture, level);}
void gl_framebuffer_texture_3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) { glFramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);}
void gl_framebuffer_texture_(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)           { glFramebufferTextureLayer(target, attachment, texture, level, layer);}


void gl_generate_samplers(GLsizei n, GLuint* samplers) { glGenSamplers(n, samplers); }
void gl_delete_samplers(GLsizei n, GLuint* samplers) { glDeleteSamplers(n, samplers); }
void gl_get_sampler_param_Ii32v(GLuint sampler, GLenum pname, GLint* params)       { glGetSamplerParameterIiv(sampler, pname, params); }
void gl_get_sampler_param_Iu32v(GLuint sampler, GLenum pname, GLuint* params)      { glGetSamplerParameterIuiv(sampler, pname, params); }
void gl_get_sampler_param_f32v(GLuint sampler, GLenum pname, GLfloat* params)      { glGetSamplerParameterfv(sampler, pname, params); }
void gl_get_sampler_param_i32v(GLuint sampler, GLenum pname, GLint* params)        { glGetSamplerParameteriv(sampler, pname, params); }
void gl_set_sampler_param_Ii32v(GLuint sampler, GLenum pname, const GLint* param)  { glSamplerParameterIiv(sampler, pname, param); }
void gl_set_sampler_param_Iu32v(GLuint sampler, GLenum pname, const GLuint* param) { glSamplerParameterIuiv(sampler, pname, param); }
void gl_set_sampler_param_f32(GLuint sampler, GLenum pname, GLfloat param)         { glSamplerParameterf(sampler, pname, param); }
void gl_set_sampler_param_f32v(GLuint sampler, GLenum pname, const GLfloat* param) { glSamplerParameterfv(sampler, pname, param); }
void gl_set_sampler_param_i32(GLuint sampler, GLenum pname, GLint param)           { glSamplerParameteri(sampler, pname, param); }
void gl_set_sampler_param_i32v(GLuint sampler, GLenum pname, const GLint* param)   { glSamplerParameteriv(sampler, pname, param); }


void gl_generate_textures(GLsizei n, GLuint* textures)     { glGenTextures(n, textures); }
void gl_delete_textures(GLsizei n, const GLuint* textures) { glDeleteTextures(n, textures); }
void gl_select_active_texture(GLenum texture)              { glActiveTexture(texture); }
void gl_bind_texture(GLenum target, GLuint texture)        { glBindTexture(target, texture); }

