/*
 * Created by ckalo on 4/2/26.
 */
#ifndef ENGINE_TEST_RENDERER_H
#define ENGINE_TEST_RENDERER_H
#include "my_types.h"
#include "opengl.h"

typedef struct Vertex
{
    vec2f pos;
    vec3f col;
} Vertex;


GLuint renderer_create_vb(Vertex* vertices, const uint32 size);
void renderer_bind_vb(GLuint vbo);
void renderer_update_vb(Vertex* vertices);
void renderer_destroy_vb(GLuint* vbo);

void renderer_create_ib();
void renderer_bind_ib();
void renderer_update_ib();
void renderer_destroy_ib();

void renderer_create_texture();
void renderer_bind_texture();
void renderer_update_texture();
void renderer_destroy_texture();

progidx_t renderer_create_shader();
void renderer_destroy_shader();


#endif //ENGINE_TEST_RENDERER_H