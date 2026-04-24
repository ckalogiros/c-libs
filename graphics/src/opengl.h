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
void gl_delete_attrib_array(GLsizei n, const GLuint * arrays);


void gl_enable_attrib_array(GLint loc);
void gl_enable_attrib_pointer(GLint loc, GLuint size, GLuint stride, GLuint vertex_offset);


void gl_draw_arrays(GLenum mode, GLint start_index, GLsizei vert_n);


void gl_viewport(GLuint width, GLuint height);
void gl_clear();
void gl_clear_color(const vec4f col);


GLint gl_get_uniform_location(GLuint program, const char* loc_name);
void gl_uniform_matrix_4fv(GLint loc, const GLfloat* mvp);


GLint gl_get_attribute_location(GLuint program, const char* loc_name);


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
void gl_generate_framebuffers(GLsizei n, GLuint * framebuffers);
void gl_delete_framebuffers(GLsizei n, const GLuint * framebuffers);
void gl_generate_renderbuffers(GLsizei n, GLuint * renderbuffers);
void gl_delete_renderbuffers(GLsizei n, const GLuint * renderbuffers);
void gl_framebuffer_renderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void gl_framebuffer_texture(GLenum target, GLenum attachment, GLuint texture, GLint level);
void gl_framebuffer_texture_1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void gl_framebuffer_texture_2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void gl_framebuffer_texture_3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
void gl_framebuffer_texture_(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);


void gl_generate_samplers(GLsizei n, const GLuint * samplers);
void gl_delete_samplers(GLsizei n, const GLuint * samplers);
void gl_get_sampler_param_Ii32v(GLuint sampler, GLenum pname, GLint * params);
void gl_get_sampler_param_Iu32v(GLuint sampler, GLenum pname, GLuint * params);
void gl_get_sampler_param_f32v(GLuint sampler, GLenum pname, GLfloat * params);
void gl_get_sampler_param_i32v(GLuint sampler, GLenum pname, GLint * params);
void gl_set_sampler_param_Ii32v(GLuint sampler, GLenum pname, const GLint * param);
void gl_set_sampler_param_Iu32v(GLuint sampler, GLenum pname, const GLuint * param);
void gl_set_sampler_param_f32(GLuint sampler, GLenum pname, GLfloat param);
void gl_set_sampler_param_f32v(GLuint sampler, GLenum pname, const GLfloat * param);
void gl_set_sampler_param_i32(GLuint sampler, GLenum pname, GLint param);
void gl_set_sampler_param_i32v(GLuint sampler, GLenum pname, const GLint * param);


void gl_generate_textures(GLsizei n, GLuint * textures);
void gl_delete_textures(GLsizei n, const GLuint * textures);
void gl_select_active_texture(GLenum texture);
void gl_bind_texture(GLenum target, GLuint texture);


/* copy of opengl constants */
#define OPENGL_ARRAY_BUFFER     0x8892 // GL_ARRAY_BUFFER
#define OPENGL_STATIC_DRAW      0x88E4 // GL_STATIC_DRAW
#define OPENGL_VERTEX_SHADER    0x8B31 // GL_VERTEX_SHADER
#define OPENGL_FRAGMENT_SHADER  0x8B30 // GL_FRAGMENT_SHADER
#define OPENGL_TRIANGLES        0x0004 // GL_TRIANGLES

// struct vertex_layout {
//
// }

struct opengl_program {
    GLint program;
    int32 idx;
    struct vertex_layout vertex_layout;

    // sid
    // state = { mask: 0x0 };

    // shader_ops
    // if (shader_ops_idxs !== null) {
    // 	this.AddShaderOperation(shader_ops_idxs); // Build the array
    // }

    // shader_program
    // attribsidx
    // attribs_indexing

    //vb_group
    //ib
    //vao
    //vaoidx
    //ubidxs
    //attribs_per_vertex

    // blend_mode_ubidx

    // uniforms;
    // this.uniforms.push({
    // 	name: uniforms[i].name,
    // 	shader: uniforms[i].shader, // Some shader flags(IN_FS, IN_VS, ...)
    // 	type: uniforms[i].type, // UNIF_TYPE.FARR,
    // 	ubidx: uniforms[i].ubidx, // Index
    // 	arr_size: uniforms[i].arr_size, // If this uniform is a uniform buffer
    // 	shader_variables_unif_names: null// A collection of all names and types of the variable written inside the shader program code
    // });

    // shaderinfo
};
progidx_t opengl_program_create(const struct gfx_sid sid, const char* fs);
progidx_t opengl_program_get_byidx(progidx_t idx);

#endif //ENGINE_TEST_OPENGL_H