/*
 * Created by ckalo on 4/21/26.
 */
#ifndef GRAPHICS_TEST_GFX_INTERFACE_H
#define GRAPHICS_TEST_GFX_INTERFACE_H
#include "my_types.h"
#include "shader_id.h"



typedef int8 build_t; // shader build/shader program
typedef build_t desc_t;  // descriptor of shader build/shader program
typedef int8 vlayout_t;
typedef int8 vaidx_t;
typedef int8 vbidx_t;
typedef int8 ibidx_t;
typedef int8 tbidx_t;


struct gfx_sid gfx_get_sid(const sididx_t sid_idx);

sididx_t gfx_create_sid(struct gfx_sid sid);

sididx_t gfx_sid_find_match(struct gfx_sid sid);

bool sid_check_match(const sididx_t sid_idx1, const sididx_t sid_idx2);
//
// static bool
// sid_copy(const sididx_t dst_sid_idx1, const sididx_t dst_sid_idx2)
// {
//     struct gfx_sid dst = gfx_get_sid(dst_sid_idx1);
//     struct gfx_sid src = gfx_get_sid(src_sid_idx2);
//     dst->attr = src.attr;
//     dst->unif = src.unif;
//     dst->out  = src.out;
//     dst->pass = src.pass;
//     dst->oper = src.oper;
// }



vaidx_t gfx_vertex_array_find_match(const sididx_t sid_idx, const bool attrib_devisor);

vbidx_t gfx_vertex_buffer_find_match(const sididx_t sid_idx);

#endif //GRAPHICS_TEST_GFX_INTERFACE_H