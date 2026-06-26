/*
 * Created by ckalo on 5/29/26.
 */
#ifndef GRAPHICS_TEST_TEST_H
#define GRAPHICS_TEST_TEST_H

#include "gfx_interface.h"
#include "my_types.h"
// #include "shader_descriptor.h"


#define MAX_COMPONENTS 32
#define MAX_OBJECTS 16

struct attribs_descriptor {
    uint8 offset;
    uint8 elems_count;
};
struct object_components_descriptor {
    // struct gfx_sid sid;
    sididx_t sid_idx;
    sid_t    added_components_flag; /// A way to know if a component is already added to this descriptor
    uint32   obj_count;             /// num objects
    uint32   obj_vertex_count;      /// vertices of an object
    uint32   total_vertex_count;    /// vertices of all objects of a group
    build_t  build_idx;             /// the index to the shader build
    vaidx_t  vaidx;                 /// the index to the vertex array
    vbidx_t  vbidx;                 /// the index to the vertex buffer
    ibidx_t  ibidx;                 /// the index to the index buffer
    tbidx_t  tbidx;                 /// the index to the texture buffer
    uint16   rqidx;                 /// the index to the renderqueue buffer
    uint8    stride;                /// the size in elements of object's components
    uint8    num_components;        /// the number of different components
    struct attribs_descriptor attr_dsc[MAX_COMPONENTS]; /// descriptor for each attribute
    uint32 obj_vb_start[MAX_OBJECTS];
};
struct object_components_group {
    uint32 group_id;
    struct object_components_descriptor comp_desc;
    /// TODO: Maybe data should be moved outside the component descriptor buffer, be separated
    float data[1024]; // the actual components in contiguous float values
};



uint32 test_create_new_components_buffer(const sididx_t sid_idx);
uint32 test_component_group_find_match(const sididx_t sid_idx);
uint32 test_create_new_obj(const vbidx_t group_id);
void test_add_comp(const vbidx_t group_id, const uint32 obj_idx, float* src, uint32 elems_count, uint32 offset, const sid_t comp_flag);

// void test_set_build_idx(const vbidx_t group_id, build_t build_idx);
void test_set_stride(const vbidx_t group_id, uint32 stride);
void test_set_obj_vertex_count(const vbidx_t group_id, uint32 obj_vertex_count);

struct object_components_descriptor* test_get_comp_desc(const vbidx_t group_id);
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