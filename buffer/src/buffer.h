//
// Created by ckalo on 12/29/25.
//

#ifndef BUFFER_BUFFER_H
#define BUFFER_BUFFER_H

#include "buffer_struct.h"



void buffer_init();

buffer_t* buffer_create_uint32(const uint32 capacity);
// buffer_t* buffer_create_vec4f(const uint32 capacity);
// buffer_t* buffer_create_vec3f(const uint32 capacity);
// buffer_t* buffer_create_vec2f(const uint32 capacity);
buffer_t* buffer_create_gentype(const uint32 capacity, const uint32 typesize);
void buffer_destroy(buffer_t* buffer);

int32 buffer_push_uint32(const uint32 val, buffer_t *const buffer);
// uint32 buffer_push_vec4f(const vec4f val, buffer_t *const buffer);
// uint32 buffer_push_vec3f(const vec3f val, buffer_t *const buffer);
// void buffer_push_vec2f(const vec2f val, buffer_t *const buffer);
// uint32 buffer_push_gentype(const void* const val, buffer_t *const buffer);

// sets up buffer's fields, but the data transform happens at caller site
uint32 buffer_set_for_push(buffer_t *const buffer);
void buffer_set_for_insert(buffer_t *const buffer, const bufidx_t index);

void buffer_insert_uint32(const uint32 val, buffer_t* const buffer, bufidx_t index);

void buffer_remove_uint32(buffer_t *const buffer, const bufidx_t index);
// void buffer_remove_vec4f(buffer_t *const buffer, const bufidx_t index);
// void buffer_remove_vec3f(buffer_t *const buffer, const bufidx_t index);
// void buffer_remove_vec2f(buffer_t *const buffer, const bufidx_t index);

/* getters */
uint16 buffer_get_next_free(buffer_t* const buffer);

#endif //BUFFER_BUFFER_H