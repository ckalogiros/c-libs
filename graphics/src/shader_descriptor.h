/*
 * Created by ckalo on 4/22/26.
 */
#ifndef GRAPHICS_TEST_SHADER_ATTRIBUTES_H
#define GRAPHICS_TEST_SHADER_ATTRIBUTES_H
#include "gfx_interface.h"
#include "my_types.h"




enum SHADER_INPUT_FLAGS {
    IS_ATTR = BIT_MASK_FROM_UINT(1), /// is input an attribute?
    IS_UNIF = BIT_MASK_FROM_UINT(2), /// is input a uniform?
    IS_OUT = BIT_MASK_FROM_UINT(3),
    IS_ASSIGN = BIT_MASK_FROM_UINT(4),
    IN_VSHADER = BIT_MASK_FROM_UINT(5), /// does input belong to vertex shader?
    IN_FSHADER = BIT_MASK_FROM_UINT(6), /// does input belong to vertex shader?
    IS_TRANSFORM_VPOS = BIT_MASK_FROM_UINT(7),
    IS_TRANSFORM_WPOS = BIT_MASK_FROM_UINT(8),
    IS_TRANSFORM_SCALE = BIT_MASK_FROM_UINT(9),
    IS_TRANSFORM_ROT = BIT_MASK_FROM_UINT(10),
    IS_TRANSFORM_PROJ = BIT_MASK_FROM_UINT(11),
};


/***************************************************************************************************
 * shader vertex layout
 **************************************************************************************************/

struct vertex_layout {
    uint8 loc;
    uint8 size;
    uint8 type;
    int8 loc_offset; // attributes's offset of shader's location
    int8 offset; // attributes's offset from the start
    vlayout_t idx;
    uint8 attr_idx;
};

struct uniform_layout {
    uint8 size;
    uint8 type;
    uint8 idx;
};

struct vertex_attribute_ids {
    /* INPUT: enum SID_VAR_IDI */
    uint8 ids[SID_IDI_COUNT];
    uint8 count;
};


struct shader_in_variables_struct {
    /* flag | type | elems | typesize | flagname | codername | selfidx *
     * 0x1  | 1    | 3     | 4        | COL3     | color3    | 1       */
    const char* flagname;
    const char* codename;
    uint32 flag;
    enum TYPES_INDEXING type;
    enum SHADER_INPUT_FLAGS which_renameme;
    uint8 typesize;
    uint8 elems;
    uint8 this_idx; // the index position in the shader_vars_table buffer
    uint8 var_table_idx;
    /// TODO: maybe unnecessary, cause enum SID_VAR_IDI does not guarantee same index in vertex_attribute_ids
    enum SID_ID shader_var_id; // the enum SID_VAR_IDI index of current shader variable input
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
    sid_t added_entry_flags_renameme;

    int16 vshader_attribs_idx;
    uint16 vshader_code_size;
    uint8 vshader_attribs_count;
    uint8 vshader_attribs_stride;
    uint8 vshader_attrib_locations_count;
    uint8 last_loc_num_elems;
    uint32 entry_count;
    uint32 vlayout_count;
    build_t build_idx;
    vaidx_t vaoidx; // TODO: Maybe many VAO's for a shader program(???)
    bool is_set; // A build maybe created but not set(shader_builder_build())

    struct gfx_sid sid;
    struct shader_in_variables_struct entry[SHADER_INPUT_ENTRIES_MAX_COUNT];
    struct vertex_layout v_layout[SHADER_INPUT_ENTRIES_MAX_COUNT];
    struct vertex_attribute_ids vertex_attribute_ids; // TODO: use sparse array for dynamicaly sized buffer


    const char* vshader_code;

};


struct shader_descriptor* shader_descriptor_get(const desc_t desc_idx);

uint32 shader_descriptor_get_count(void);

// void shader_descriptor_increment_count(void);

void shader_descriptor_init(void);

build_t shader_descriptor_create(struct gfx_sid sid);

void shader_descriptor_create_vertex_layout(const desc_t desc_idx, struct vertex_layout attribs_desc);

void shader_init_attribsidxs(const desc_t desc_idx);

struct vertex_layout* shader_descriptor_vertex_layout_get(const desc_t desc_idx, const vlayout_t vlayout_idx);

uint8 shader_attrib_id_get(const desc_t desc_idx, const enum SID_ID idx);

uint8 shader_attrib_get_offset(const desc_t desc_idx, const enum SID_ID idx);

uint8 shader_attrib_get_stride(const desc_t desc_idx);


uint32 shader_descriptor_vertex_layout_set(const desc_t desc_idx, struct vertex_layout attribs_desc[], const uint32 count);

void shader_descriptor_set_attribute_ids(const desc_t desc_idx, const uint32 attrib_id, const uint32 vid);

void shader_descriptor_print_attribute_ids(const desc_t desc_idx, const uint32 vid);

void shader_attributes_enable(const uint32 count, const uint32 stride, const uint8 last_loc_num_elems);

#endif //GRAPHICS_TEST_SHADER_ATTRIBUTES_H
