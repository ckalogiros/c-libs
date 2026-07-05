/*
 * Created by ckalo on 5/29/26.
 *
 * TODO: maybe the "components" is the wrong name, with the correct being "object attributes".
 * Then we can name "components" any higher structure, (like: {scroller}), that has a set of attributes and functionality.
 */
#ifndef GRAPHICS_TEST_TEST_H
#define GRAPHICS_TEST_TEST_H

#include "gfx_interface.h"
#include "my_types.h"
#include "engine.h"
#include "mem_man_types.h"
#include "buffer_arr_struct.h"


#define COMP_MAX_GROUPS     32
#define COMP_MAX_ATTRIBUTES 32
#define COMP_MAX_OBJECTS    16
#define COMP_MAX_FLOATS_PER_ATTRIBUTE 4


struct attribs_per_attrib_descriptor {
    uint8 attr_offset;
    uint8 attr_count;
};
struct attributes_group {
    struct attribs_per_attrib_descriptor attribs_descr[COMP_MAX_ATTRIBUTES]; /// attributes descriptor

    buffer_t* obj_bufids;            /// a buffer of object ids
    sid_t     added_components_flag; /// a way to know if an attribute is already added to this descriptor
    sid_t     commands_flag;         /// a way to know any commands scheduled to be run

    uint32   vertex_count;          /// vertices count of all objects of the group

    memid_t  attribs_data_id;       /// the index to the allocator's table for the attributes data buffer.
    sididx_t sid_idx;               /// the index to the shader id buffer

    build_t  build_idx;             /// the index to the shader build
    vaidx_t  vaidx;                 /// the index to the vertex array
    vbidx_t  vbidx;                 /// the index to the vertex buffer
    ibidx_t  ibidx;                 /// the index to the index buffer
    tbidx_t  tbidx;                 /// the index to the texture buffer
    int16    rqidx;                 /// the index to the renderqueue buffer
    uint8    stride;                /// the size in elements of an object's attributes
    uint8    attribs_count;         /// the number of unique attribute
    uint8    attribs_desc_counter;  /// a counter for the attribs_desc buffer

};

void     attribsGroupDescr_table_init(void);
attrid_t attribsGroupDescr_create_new_components_group(const sididx_t sid_idx);
attrid_t attribsGroupDescr_group_find_match(const sididx_t sid_idx);
objid_t  attribsGroupDescr_create_new_obj(attrid_t group_id, objid_t obj_id, uint16 obj_vertex_count);
void     attribsGroupDescr_add_comp(const attrid_t group_id, const float* src, uint32 elems_count, uint32 offset, const sid_t attribs_flag);

void    attribsGroupDescr_set_stride(const attrid_t group_id, uint32 stride);
void    attribsGroupDescr_set_build_idx(const attrid_t group_id, const build_t build_idx);

struct attributes_group* attribsGroupDescr_get_group(const attrid_t group_id);
buffer_t* attribsGroupDescr_get_objects_group(const attrid_t group_id);
build_t   attribsGroupDescr_get_buildidx(const attrid_t group_id);
uint32    attribsGroupDescr_get_attribs_stride(const attrid_t group_id);
uint32    attribsGroupDescr_get_vertex_count(const attrid_t group_id);
float*    attribsGroupDescr_get_attribs_data(const attrid_t group_id);
uint32    attribsGroupDescr_get_objects_count(const attrid_t group_id);
bool      attribsGroupDescr_is_indexed_geometry(const attrid_t group_id);

/* DEBUG-PRINT */
void attribsGroupDescr_group_print();

#endif //GRAPHICS_TEST_TEST_H