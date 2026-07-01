/*
 * Created by ckalo on 5/29/26.
 */
#ifndef GRAPHICS_TEST_TEST_H
#define GRAPHICS_TEST_TEST_H

#include "gfx_interface.h"
#include "my_types.h"
#include "mesh.h"
#include "engine.h"
#include "mem_man_types.h"


#define COMP_MAX_GROUPS     32
#define COMP_MAX_ATTRIBUTES 32
#define COMP_MAX_OBJECTS    16
#define COMP_MAX_FLOATS_PER_ATTRIBUTE 4 /// TODO:

struct comp_per_attrib_descriptor {
    uint8 attr_offset;
    uint8 attr_count;
};

/// TODO: this belongs to the object struct
struct comp_per_object_descriptor {
    uint32 vb_start;          /// the start index of an object, in the vertex buffer
    uint8  vertex_count;      /// number of object's vertices
    enum MESH_TYPE mesh_type; /// used by "gfx_vertex_buffer_set_data()" to get the object's correct vpos mesh
};
struct comp_per_group_descriptor {
    sididx_t sid_idx;
    sid_t    added_components_flag; /// A way to know if a component is already added to this descriptor
    sid_t    commands_flag;         /// A way to know any commands scheduled to be run
    uint32   obj_count;             /// num objects
    uint32   obj_vertex_count;      /// vertices of an object
    uint32   total_vertex_count;    /// vertices of all objects of a group
    build_t  build_idx;             /// the index to the shader build
    vaidx_t  vaidx;                 /// the index to the vertex array
    vbidx_t  vbidx;                 /// the index to the vertex buffer
    ibidx_t  ibidx;                 /// the index to the index buffer
    tbidx_t  tbidx;                 /// the index to the texture buffer
    int16    rqidx;                 /// the index to the renderqueue buffer
    uint8    stride;                /// the size in elements of object's components
    uint8    components_count;      /// the number of unique components

    /// TODO: per_attr_desc should be a dynamic buffer(sparse array container)
    struct   comp_per_attrib_descriptor per_attr_desc[COMP_MAX_ATTRIBUTES]; /// descriptor per component attribute, one buffer per component group
    uint8    per_attr_desc_counter; /// a counter for the per_attr_desc buffer

    /// TODO: per_obj_desc should be a dynamic buffer(sparse array container)
    /// TODO: this belongs to the object struct
    struct   comp_per_object_descriptor per_obj_desc[COMP_MAX_OBJECTS];     /// a component's per object parameters
    uint8    per_obj_desc_counter; /// a counter for the per_obj_desc buffer
};
struct components_group {
    compid_t comp_group_id;
    objid_t* obj_group;
    struct comp_per_group_descriptor comp_desc;
    memid_t attribs_data_inst_id; // index to a memoru allocator table, that will store component's attributes of all objects in a contiguous float buffer
};

void     obj_component_table_init(void);
compid_t obj_component_create_new_components_group(const sididx_t sid_idx);
compid_t obj_component_group_find_match(const sididx_t sid_idx);
compid_t obj_component_create_new_obj(const vbidx_t group_id, const uint16 obj_vertex_count, enum MESH_TYPE mesh_type);
void     obj_component_add_comp(const vbidx_t group_id, const uint32 obj_idx, float* src, uint32 elems_count, uint32 offset, const sid_t comp_flag);

void    obj_component_set_stride(const vbidx_t group_id, uint32 stride);
build_t obj_component_set_build_idx(const vbidx_t group_id, const build_t build_idx);
void    obj_component_set_obj_vertex_count(const vbidx_t group_id, uint32 obj_vertex_count);

struct  comp_per_group_descriptor* obj_component_get_comp_desc(const vbidx_t group_id);
build_t obj_component_get_build_idx(const vbidx_t group_id);
uint32  obj_component_obj_comp_desc_get_stride(const vbidx_t group_id);
uint32  obj_component_get_obj_vertex_count(const vbidx_t group_id);
uint32  obj_component_get_total_vertex_count(const vbidx_t group_id);
float*  obj_component_get_obj_attribs_data(const vbidx_t group_id);
uint32  obj_component_obj_comp_desc_get_total_vertex_count(const vbidx_t group_id);
uint32  obj_component_obj_comp_desc_get_total_object_count(const vbidx_t group_id);

/* DEBUG-PRINT */
void obj_component_group_print();

#endif //GRAPHICS_TEST_TEST_H