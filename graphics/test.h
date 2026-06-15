/*
 * Created by ckalo on 5/29/26.
 */
#ifndef GRAPHICS_TEST_TEST_H
#define GRAPHICS_TEST_TEST_H

#include "gfx_interface.h"
#include "my_types.h"
#include "shader_descriptor.h"


uint32 test_create_new_buffer(struct gfx_sid sid);
uint32 test_find_match(struct gfx_sid sid);
uint32 test_create_new_obj(const vbidx_t group_id);
void test_add_comp(const vbidx_t group_id, const uint32 obj_idx, float* src, uint32 elems_count, uint32 offset, const char* debug_info);

void test_set_build_idx(const vbidx_t group_id, build_t build_idx);
void test_set_stride(const vbidx_t group_id, uint32 stride);
void test_set_obj_vertex_count(const vbidx_t group_id, uint32 obj_vertex_count);

build_t test_get_build_idx(const vbidx_t group_id);
uint32 test_obj_comp_desc_get_stride(const vbidx_t group_id);
uint32 test_get_obj_vertex_count(const vbidx_t buffer_id);
uint32 test_get_total_vertex_count(const vbidx_t buffer_id);
float* test_get_obj_attribs_data(const vbidx_t buffer_id);
uint32 test_obj_comp_desc_get_total_vertex_count(const vbidx_t group_id);
uint32 test_obj_comp_desc_get_total_object_count(const vbidx_t group_id);


/* DEBUG-PRINT */
void obj_comp_group_print();

#endif //GRAPHICS_TEST_TEST_H