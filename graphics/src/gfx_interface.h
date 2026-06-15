/*
 * Created by ckalo on 4/21/26.
 */
#ifndef GRAPHICS_TEST_GFX_INTERFACE_H
#define GRAPHICS_TEST_GFX_INTERFACE_H
#include "my_types.h"
#include "shader_id.h"



typedef int8 build_t; // shader build/shader program
typedef int8 desc_t;  // descriptor of shader build/shader program
typedef int8 vlayout_t;
typedef int8 vaidx_t;
typedef int8 vbidx_t;
typedef int8 ibidx_t;
typedef int8 tbidx_t;


uint32 gfx_vertex_buffer_find_match(const struct gfx_sid sid);

#endif //GRAPHICS_TEST_GFX_INTERFACE_H