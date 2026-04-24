/*
 * Created by ckalo on 4/2/26.
 *
    Straight answer: Renderer is responsible for DRAWING + STATE MANAGEMENT + PIPELINE EXECUTION.
    Complete Renderer Responsibilities

    Responsibility	        What It Does	                            Example
    Draw execution	        Issues GPU draw calls	                    glDrawElements(), glDrawArrays()
    Resource binding	    Binds programs, VBOs, IBOs, textures	    glBindVertexArray(), glUseProgram()
    Pipeline state	        Manages depth, stencil, culling, blend	    glEnable(GL_DEPTH_TEST), glBlendFunc()
    Render order	        Sorts objects by layer, material, distance	Opaque front-to-back, transparent back-to-front
    Culling	                Skips invisible objects	                    Frustum culling, occlusion culling
    Viewport/scissor	    Sets render area	                        glViewport(), glScissor()
    Clear buffers	        Clears color, depth, stencil	            glClear(GL_COLOR_BUFFER_BIT)
    Frame orchestration	    Manages render passes	                    Shadow pass → GBuffer → Lighting → UI
    Render target management	Switches framebuffers	                glBindFramebuffer()
    Draw call batching	    Groups compatible draw calls	            Same program + same VBO = batch
    Statistics	            Counts draw calls, triangles, etc	        Debug overlay

    What Renderer Does NOT Do
    Not Renderer's Job	Who Does It
    Create programs, VBOs, IBOs	Resource Manager
    Load textures	Asset Loader
    Compile shaders	Shader Compiler
    Manage scene graph	Scene Manager
    Update transforms	GameObject/Transform system
    Handle input	Input Manager
    Physics	Physics Engine
    Audio	Audio System
 */

#include "renderer.h"
#include "my_types.h"
#include "shaders.h"
#include "shader_builder.h"



GLuint renderer_create_vb(void* vertices, const uint32 size)
{
    GLuint vbo;
    gl_gen_buffers(1, &vbo);
    gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo);
    gl_buffer_data(OPENGL_ARRAY_BUFFER, size, vertices, OPENGL_STATIC_DRAW);

    return vbo;
}
void renderer_bind_vb(GLuint vbo){ gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo); }
void renderer_update_vb(void* vertices, const uint32 bytesize)
{
    gl_buffer_data(OPENGL_ARRAY_BUFFER, bytesize, vertices, OPENGL_STATIC_DRAW);
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
    sid_t sid_attr = SID_VPOSI2 | SID_SCALE2 | SID_COLOR3;
    // sid_t sid_attr = SID_VPOSI2 | SID_SCALE2 | SID_COLOR4;
    const sid_t sid_attr_out = SID_COLOR3;
    sid_t sid_unif = SID_PROJMAT4X4;
    const sid_t sid_pass = SID_COLOR3;


    // const string_t* vs = shader_builder_build(sid_attr, sid_attr_out, sid_unif, sid_pass);

    // progidx_t progidx = opengl_program_create(vs->data, fragment_shader_text);
    // progidx_t progidx = opengl_program_create(vertex_shader_text, fragment_shader_text);
    // progidx_t program = opengl_program_get_byidx(progidx);
    // return program;
}
void renderer_destroy_shader(){}
