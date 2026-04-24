/*
 * Created by ckalo on 4/22/26.
 */
#ifndef GRAPHICS_TEST_SHADER_ATTRIBUTES_H
#define GRAPHICS_TEST_SHADER_ATTRIBUTES_H
#include "my_types.h"

/***************************************************************************************************
 * SHADER DESCRIPTOR STRUCT
 **************************************************************************************************/
struct vertex_layout {
    uint8 loc;
    uint8 size;
    uint8 type;
    uint8 offset;
    uint8 idx;
};

/***************************************************************************************************
 * SHADER DESCRIPTOR STRUCT
 **************************************************************************************************/
struct shader_descriptor {
    const char* vshader_code;
    // struct generic_location* vshader_attribs;
    int16 vshader_attribs_idx;
    /// for uniforms
    /// for operations
    ///
    uint16 vshader_code_size;
    uint8 vshader_attribs_count;
};




/***************************************************************************************************
 * ATTRIBUTES
 **************************************************************************************************/
enum SHADER_ATTRIBUTES_INDEXING {
    SID_ATTR_OPACIT_IDX = 0,
    SID_ATTR_VPOSI2_IDX,
    SID_ATTR_WPOSI2_IDX,
    SID_ATTR_SCALE2_IDX,
    SID_ATTR_ROTAT2_IDX,
    SID_ATTR_PIVOT2_IDX,
    SID_ATTR_UVCOO2_IDX,
    SID_ATTR_VPOSI3_IDX,
    SID_ATTR_WPOSI3_IDX,
    SID_ATTR_SCALE3_IDX,
    SID_ATTR_ROTAT3_IDX,
    SID_ATTR_PIVOT3_IDX,
    SID_ATTR_COLOR3_IDX,
    SID_ATTR_NORMA3_IDX,
    SID_ATTR_COLOR4_IDX,
    SID_ATTR_PROJMAT4X4_IDX,

    SID_ATTRIBS_IDX_COUNT
};


/***************************************************************************************************
 * UNIFORMS
 **************************************************************************************************/
enum SHADER_UNIFORMS_INDEXING {
    SID_UNIF_OPACIT_IDX = 0,
    SID_UNIF_VPOSI2_IDX,
    SID_UNIF_WPOSI2_IDX,
    SID_UNIF_SCALE2_IDX,
    SID_UNIF_ROTAT2_IDX,
    SID_UNIF_PIVOT2_IDX,
    SID_UNIF_UVCOO2_IDX,
    SID_UNIF_VPOSI3_IDX,
    SID_UNIF_WPOSI3_IDX,
    SID_UNIF_SCALE3_IDX,
    SID_UNIF_ROTAT3_IDX,
    SID_UNIF_PIVOT3_IDX,
    SID_UNIF_COLOR3_IDX,
    SID_UNIF_NORMA3_IDX,
    SID_UNIF_COLOR4_IDX,
    SID_UNIF_PROJMAT4X4_IDX,

    SID_UNIF_IDX_COUNT
};

// here: create a .c file to store all programs vertex layouts


struct vertex_layout* shader_attributes_desc_get_byidx(const uint32 idx);

uint32 shader_attributes_desc_get_count();

uint32 shader_attributes_desc_set(struct vertex_layout attribs_desc[], const uint32 count);

void shader_attributes_enable();

#endif //GRAPHICS_TEST_SHADER_ATTRIBUTES_H