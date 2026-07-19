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
#define COMP_MAX_ATTRIBUTES 16
#define COMP_MAX_OBJECTS    16
#define COMP_MAX_FLOATS_PER_ATTRIBUTE 4

/*
 * instanced implementation
 *
 * we need:
 * array of vbs
 * array of divisors
 *
 * ? we need a stride for all attributes and a stride per vertex buffer ?
 *
 * Who decides for instancing???
 * Maybe the attributes descriptor group does, and not each individual objects.
 * Call a function to set "instancing"
 *      how many vertex buffers?
 *      what is the devisor for each vb?
 *      how many attributes in each vb?
 *      What are the attributes? Maybe have an array of indexes pointing to the attrib descriptor buffer
 *
 * Considerations:
 *      1. we no longer can mix different shapes(triangles, rectangle atc)
 */

/*
 * INEFFICIENCIES
 * SAME STRUCT:
 *      1. struct attribs_per_attrib_descriptor attribs_descr[]; /// in attribute groups
 *      2. struct shader_entry                  entries[];       /// in shader build
 *      both of them have: attrib offset, num_elems, flag, and index to one onother
 */

typedef uint8 attr_t;

struct attribs_per_attrib_descriptor {
    sid_t  attrib_flag;    /// flag of type: SID_... VPOS2,WPOS2,COL4, etc
    attr_t attr_offset;    /// the start offset of the attribute  in the attributes data buffer
    attr_t attr_num_elems; /// how many: floats | ints etc, for the attribute
    attr_t divisor;
    /// !IMPORTANT!: This is  the equivalent index of "this" attribute in the shader's attributes entries (struct shader_entry entries).
    /// This is the way of object's attribute data NOT being in order with the shader's attributes.
    /// E.x. obj_attribs_order:{VPOS, WPOS, ..., COL}, shader_attribs_order:{VPOS, COL, WPOS, ...}
    /// attribs_descr[1] == 2.        WPOS=1                                            WPOS=2
    // attr_t shader_attrib_idx;
};

struct attributes_group {
    struct attribs_per_attrib_descriptor attribs_descr[COMP_MAX_ATTRIBUTES]; /// attributes descriptor
    buffer_t* instancing;            /// a buffer of divisors???????????
    buffer_t* obj_bufids;            /// a buffer of object ids
    sid_t     added_components_flag; /// a way to know if an attribute is already added to this descriptor
    sid_t     commands_flag;         /// a way to know any commands scheduled to be run

    uint32   vertex_count;          /// vertices count of all objects of the group

    memid_t  attribs_data_id;       /// the index to the allocator's table for the attributes data buffer.
    sididx_t sid_idx;               /// the index to the shader id buffer

    build_t  build_idx;             /// the index to the shader build
    // vaidx_t  vaidx;                 /// the index to the vertex array
    // vbidx_t  vbidx;                 /// the index to the vertex buffer
    // ibidx_t  ibidx;                 /// the index to the index buffer
    // tbidx_t  tbidx;                 /// the index to the texture buffer
    int16    rqidx;                 /// the index to the renderqueue buffer
    uint8    stride;                /// the size in elements of an object's attributes
    attr_t   attribs_count;         /// the number of unique attribute
    attr_t   attribs_desc_count;    /// a counter for the attribs_desc buffer

};
/* SIMILARITIES WITH SHADER DESCRIPTOR STRUCT */
/*
struct shader_descriptor {

    sididx_t sid_idx;
    sid_t   added_entry_flags_renameme;
    build_t build_idx;


    vaidx_t vaoidx;
    int16   vshader_attribs_idx;
    uint8   vshader_attribs_count;
    uint8   vshader_attribs_stride;
    uint8   vshader_attrib_locations_count;
    uint8   last_loc_num_elems;
    uint32  entry_count;
    uint32  vlayout_count;

    struct shader_entry entries[];
    struct shader_attribute_layout v_layout[];
    struct vertex_attribute_ids vertex_attribute_ids; // BUFFER


};

struct shader_attribute_layout {
    uint8 loc;
    uint8 size;
    uint8 type;
    int8 loc_offset; // attributes's offset of shader's location
    int8 offset;     // attributes's offset from the start
    vlayout_t idx;
    uint8 attr_idx;
};
struct vertex_attribute_ids {
    uint8 ids[SID_ID_COUNT];
    uint8 count;
};
struct shader_entry {
    const char* flagname;
    const char* codename;
    uint32 flag;
    enum TYPE_ID type;
    enum SHADER_INPUT_FLAGS input_flag;
    enum SID_ID shader_input_id; // the enum SID_VAR_ID index of current shader variable input
    uint8 divisor;
    uint8 typesize;
    uint8 elems;
    uint8 this_idx; // the index position in the shader_inputs_table buffer
    uint8 input_idx;
    uint8 pad1;
    uint8 pad2;
    uint8 pad3;
};
*/
void     attribsGroupDescr_table_init(void);
attrid_t attribsGroupDescr_create_new_attribs_group(sididx_t sid_idx);
attrid_t attribsGroupDescr_group_find_match(sididx_t sid_idx);
objid_t  attribsGroupDescr_create_new_obj(attrid_t attribs_idx, objid_t obj_id, uint16 obj_vertex_count);
void     attribsGroupDescr_add_attributes(const attrid_t attribs_idx, const float* const src, const uint32 elems_count,
                                          const uint32 offset, const sid_t attrib_flag, const int16 inst_divisor, const attr_t shader_attrib_idx);

void    attribsGroupDescr_set_stride(attrid_t attribs_idx, uint32 stride);
void    attribsGroupDescr_set_build_idx(attrid_t attribs_idx, build_t build_idx);

struct attributes_group* attribsGroupDescr_get_attribs_group(attrid_t attribs_idx);
buffer_t* attribsGroupDescr_get_objects_group(attrid_t attribs_idx);
struct attribs_per_attrib_descriptor* attribsGroupDescr_get_attribs_descr(const attrid_t attribs_idx);
struct instancing* attribsGroupDescr_get_instancing_data(const attrid_t attribs_idx, const attrid_t inst_id);
buffer_t* attribsGroupDescr_get_instancing_buffer(const attrid_t attribs_idx);
build_t   attribsGroupDescr_get_buildidx(attrid_t attribs_idx);
uint32    attribsGroupDescr_get_attribs_stride(attrid_t attribs_idx);
attr_t    attribsGroupDescr_get_attribs_count(const attrid_t attribs_idx);
uint32    attribsGroupDescr_get_vertex_count(attrid_t attribs_idx);
float*    attribsGroupDescr_get_attribs_data(attrid_t attribs_idx);
uint32    attribsGroupDescr_get_objects_count(attrid_t attribs_idx);
bool      attribsGroupDescr_is_indexed_geometry(attrid_t attribs_idx);

/* DEBUG-PRINT */
void attribsGroupDescr_group_print();

#endif //GRAPHICS_TEST_TEST_H