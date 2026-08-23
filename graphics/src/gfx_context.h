/*
 * Created by ckalo on 4/21/26.
 */
#ifndef GRAPHICS_TEST_GFX_INTERFACE_H
#define GRAPHICS_TEST_GFX_INTERFACE_H


// #include "my_types.h"
// #include "gfx_types.h"
#include "shader_id.h"


struct gfx_sid gfx_get_sid(const sididx_t sid_idx);

sididx_t gfx_create_sid(struct gfx_sid sid);

sididx_t gfx_sid_find_match(struct gfx_sid sid);

bool sid_check_match(const sididx_t sid_idx1, const sididx_t sid_idx2);

bool sid_check_attr_match_from_flag(const sididx_t sid_idx, const sid_t flag);

vaidx_t gfx_vertex_array_find_match(const sididx_t sid_idx, const bool attrib_devisor);

vbidx_t gfx_vertex_buffer_find_match(const sididx_t sid_idx);

#endif //GRAPHICS_TEST_GFX_INTERFACE_H