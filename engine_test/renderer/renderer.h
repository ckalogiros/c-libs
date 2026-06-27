/*
 * Created by ckalo on 4/2/26.
 */
#ifndef ENGINE_TEST_RENDERER_H
#define ENGINE_TEST_RENDERER_H
#include "my_types.h"
#include "opengl.h"




#define SCREEN_RES_X 800
#define SCREEN_RES_Y 800


GLuint renderer_create_vb(void* vertices, const uint32 size);
void renderer_bind_vb(GLuint vbo);
void renderer_update_vb(void* vertices, const uint32 bytesize);
void renderer_destroy_vb(GLuint* vbo);

void renderer_create_ib();
void renderer_bind_ib();
void renderer_update_ib();
void renderer_destroy_ib();

void renderer_create_texture();
void renderer_bind_texture();
void renderer_update_texture();
void renderer_destroy_texture();

build_t renderer_create_shader();
void renderer_destroy_shader();


#endif //ENGINE_TEST_RENDERER_H