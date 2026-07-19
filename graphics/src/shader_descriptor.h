/*
 * Created by ckalo on 4/22/26.
 */
#ifndef GRAPHICS_TEST_SHADER_ATTRIBUTES_H
#define GRAPHICS_TEST_SHADER_ATTRIBUTES_H
#include "gfx_interface.h"
#include "my_types.h"




enum SHADER_INPUT_FLAGS {
    IS_ATTR    = BIT_MASK_FROM_UINT(1), /// is input an attribute?
    IS_UNIF    = BIT_MASK_FROM_UINT(2), /// is input a uniform?
    IS_OUT     = BIT_MASK_FROM_UINT(3),
    IS_ASSIGN  = BIT_MASK_FROM_UINT(4),
    IN_VSHADER = BIT_MASK_FROM_UINT(5), /// does input belong to vertex shader?
    IN_FSHADER = BIT_MASK_FROM_UINT(6), /// does input belong to vertex shader?
    IS_TRANSFORM_VPOS  = BIT_MASK_FROM_UINT(7),
    IS_TRANSFORM_WPOS  = BIT_MASK_FROM_UINT(8),
    IS_TRANSFORM_SCALE = BIT_MASK_FROM_UINT(9),
    IS_TRANSFORM_ROT   = BIT_MASK_FROM_UINT(10),
    IS_TRANSFORM_PROJ  = BIT_MASK_FROM_UINT(11),
};


/***************************************************************************************************
 * shader vertex layout
 **************************************************************************************************/

struct shader_attribute_layout {
    uint8 loc;
    uint8 size;
    uint8 type;
    int8 loc_offset; // attributes's offset of shader's location
    int8 offset;     // attributes's offset from the start
    vlayout_t idx;
    uint8 attr_idx;
};

struct uniform_layout {
    uint8 size;
    uint8 type;
    uint8 idx;
};

/// TODO: ids[SID_ID_COUNT]  couldbe packed to 6bits(for 32 total values)
struct vertex_attribute_ids {
    /* INPUT: enum SID_VAR_ID
     * with ths buffer we can find the index of an attribute
     * (in the descriptor->vlayout buffer) using an "enum SID_ID".
     * E.x. ids[SID_VPOSI2_ID] -> gets the vlayout[] index of the SID_WPOS2 attribute
     */
    uint8 ids[SID_ID_COUNT];
    uint8 count;
};


struct shader_entry {
    /* flag | type | elems | typesize | flagname | codername | selfidx | divisor *
     * 0x1  | 1    | 3     | 4        | COL3     | color3    | 1       | 0       */
    const char* flagname;
    const char* codename;
    uint32 flag;
    enum TYPE_ID type;
    enum SHADER_INPUT_FLAGS input_flag;
    /// TODO: maybe unnecessary, because enum SID_VAR_ID does not guarantee same index in vertex_attribute_ids
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

/***************************************************************************************************
 * shader descriptor struct
 **************************************************************************************************/
#define SHADER_INPUT_ENTRIES_MAX_COUNT 16

struct shader_descriptor {
    /// used during shader build construction.
    /// the final attributes are what the "sid" is, but we need a temporary flag to "add" each attribute entry
    /// so that we can easily compare if an entry already exists, otherwise we would have to run a loop
    /// and compare each entry's flag field for matching.
    /// When all attributes of a shader build have been created,
    /// the "added_entry_flags_renameme" should be equal to "sid.attr".
    sid_t   added_entry_flags_renameme;
    int16   vshader_attribs_idx;
    uint16  vshader_code_size;
    uint8   vshader_attribs_count;
    uint8   vshader_attribs_stride;
    uint8   vshader_attrib_locations_count;
    uint8   last_loc_num_elems;
    uint32  entry_count;
    uint32  vlayout_count;
    build_t build_idx;
    vaidx_t vaoidx; // TODO: Maybe many VAO's for a shader program(???)
    bool    is_set; // A build maybe created but not set(shader_builder_build())
    sididx_t sid_idx;

    struct shader_entry entries[SHADER_INPUT_ENTRIES_MAX_COUNT];
    struct shader_attribute_layout v_layout[SHADER_INPUT_ENTRIES_MAX_COUNT];
    // TODO: use sparse array for dynamicaly sized buffer
    struct vertex_attribute_ids vertex_attribute_ids; /// this is a SID_ID buffer of attribute entries. E.x. vertex_attribute_ids[SID_WPOS_ID]->gets the SID_WPOS for this shader

    const char* vshader_code;

};


struct shader_descriptor* shader_descriptor_get(desc_t desc_idx);

uint32 shader_descriptor_get_count(void);

void shader_descriptor_init(void);

build_t shader_descriptor_create(sididx_t sid_idx);

void shader_descriptor_create_vertex_layout(desc_t desc_idx, struct shader_attribute_layout vlayout);

void shader_init_attribsidxs(desc_t desc_idx);

struct shader_attribute_layout* shader_descriptor_vertex_layout_get(desc_t desc_idx, vlayout_t vlayout_idx);

uint8 shader_attrib_id_get(desc_t desc_idx, enum SID_ID idx);

uint8 shader_attrib_get_offset(desc_t desc_idx, enum SID_ID idx);

uint8 shader_attrib_get_stride(desc_t desc_idx);

sididx_t shader_dscriptor_get_sid(desc_t desc_idx);


// uint32 shader_descriptor_vertex_layout_set(desc_t desc_idx, struct vertex_layout attribs_desc[], uint32 count);

void shader_descriptor_set_attribute_ids(desc_t desc_idx, uint32 attrib_id, uint32 vid);

void shader_attributes_enable(uint32 count, uint32 stride, uint8 last_loc_num_elems);

void shader_attributes_instanced_enable(uint32 count, uint32 stride, uint8 last_loc_num_elems);

/* DEBUG-PRINT */

void shader_descriptor_print_attribute_ids(const desc_t desc_idx, const uint32 vid);

void shader_descriptor_print_vertex_layout(const desc_t desc_idx, const uint32 count);


#endif //GRAPHICS_TEST_SHADER_ATTRIBUTES_H
