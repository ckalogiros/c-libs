/*
 * Created by ckalo on 4/20/26.
 */
#ifndef ENGINE_TEST_ENTITY_TEST_H
#define ENGINE_TEST_ENTITY_TEST_H
#include "renderer.h"


typedef struct Vertex
{
    vec2f pos;
    vec2f sca;
    vec3f col;
} Vertex;

struct entity {
    const char* name;
    Vertex* vertices;
    uint32 program;
    uint32 vao;
    uint32 vbo;
    uint32 mvp_loc;
};

struct entity* entity_create_buffer(const uint32 count);

#endif //ENGINE_TEST_ENTITY_TEST_H