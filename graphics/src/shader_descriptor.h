/*
 * Created by ckalo on 4/22/26.
 */
#ifndef GRAPHICS_TEST_SHADER_ATTRIBUTES_H
#define GRAPHICS_TEST_SHADER_ATTRIBUTES_H
#include "gfx_interface.h"
#include "my_types.h"

/***************************************************************************************************
 * shader vertex layout
 **************************************************************************************************/
// struct attribute_location {
//     uint8 locid;
//     // uint8 type;
// };
struct vertex_layout {
    uint8 loc;
    uint8 size;
    uint8 type;
    int8 offset;
    uint8 idx;
    uint8 attr_idx;
};
struct uniform_layout {
    uint8 size;
    uint8 type;
    uint8 idx;
};

#define SHADER_MAX_VERTEX_ATTRIBUTES 8
struct vertex_attribute_ids {
    /* INPUT: enum SID_VAR_IDI */
    uint8 ids[SHADER_MAX_VERTEX_ATTRIBUTES];
    uint8 count;
};

struct VERTEX_ATTRIB_RENAME_ME {
    struct vertex_layout* attribs_desc;
    struct vertex_attribute_ids vertex_attribute_ids;
};

/***************************************************************************************************
 * shader descriptor struct
 **************************************************************************************************/
struct shader_descriptor {
    const char* vshader_code;
    int16 vshader_attribs_idx;
    /// for uniforms
    /// for operations
    uint16 vshader_code_size;
    uint8 vshader_attribs_count;
    uint8 vshader_attribs_stride;
    uint8 vshader_attrib_locations_count;
    uint8 last_loc_num_elems;
    struct vertex_attribute_ids vertex_attribute_ids; // TODO: use sparse array for dynamicaly sized buffer
    /// E.x.
    /// [0]:SID_VPOSI2_IDI, meaning vpos attribute of current shader program is the first attribute
    /// [1]:SID_WPOSI3_IDI, meaning wpos attribute of current shader program is the second attribute
    /// this is how we can update attributes of a shader program

    progidx_t progidx;
    vaidx_t vaoidx;
};


struct vertex_layout* shader_attributes_desc_get_byidx(const uint32 idx);

uint32 shader_attributes_desc_get_count(void);

uint32 shader_attributes_desc_set(struct vertex_layout attribs_desc[], const uint32 count);

void shader_attributes_enable(const uint32 count, const uint32 stride, const uint8 last_loc_num_elems);

#endif //GRAPHICS_TEST_SHADER_ATTRIBUTES_H