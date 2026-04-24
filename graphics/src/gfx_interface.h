/*
 * Created by ckalo on 4/21/26.
 */
#ifndef GRAPHICS_TEST_GFX_INTERFACE_H
#define GRAPHICS_TEST_GFX_INTERFACE_H
#include "my_types.h"
#include "shader_id.h"

typedef int32 progidx_t;
typedef int32 vaidx_t;
typedef int32 vbidx_t;
typedef int32 ibidx_t;
typedef int32 tbidx_t;


uint32 gfx_vertex_buffer_find_match(const struct gfx_sid sid);

#endif //GRAPHICS_TEST_GFX_INTERFACE_H