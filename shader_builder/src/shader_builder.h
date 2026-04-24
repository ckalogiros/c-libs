/*
 * Created by ckalo on 4/8/26.
 */
#ifndef SHADER_BUILDER_SHADER_BUILDER_H
#define SHADER_BUILDER_SHADER_BUILDER_H

#include "my_macros.h"
#include "my_types.h"
#include "shader_descriptor.h"
#include "shader_id.h"

#define SHADER_ATTRIB_COLOR


struct SHADER_VARIABLES_STRUCT {
    /* flag | type | elems | typesize | flagname | codername | selfidx *
     * 0x1  | 1    | 3     | 4        | COL3     | color3    | 1       */
    const char* flagname;
    const char* codename;
    uint32 flag;
    uint32 empty;
    enum TYPES_INDEXING type;
    uint8 typesize;
    uint8 elems;
    uint8 selfidx;
    uint8 empty2;
};

enum SHADER_INPUT_FLAGS {
    SHADER_VERTEX = BIT_MASK_FROM_UINT(1), /// does input belong to vertex shader?
    SHADER_FRAGMENT = BIT_MASK_FROM_UINT(2), /// does input belong to vertex shader?
    SHADER_ATTRIBUTE = BIT_MASK_FROM_UINT(3), /// is input an attribute?
    SHADER_UNIFORM = BIT_MASK_FROM_UINT(4), /// is input a uniform?
};

typedef uint64 shader_flags;

struct SHADER_BUILDER_STRUCT {
    const char* name;
    shader_flags attrib_flag; /// flag id's: UNUSED-UNNECESSARY
    shader_flags geometry_space; /// flag to denote the implication with the vertex shader's gl_position
    enum SHADER_INPUT_FLAGS shaders; /// shaders in which 'this object' should be implemented in
    enum TYPES_INDEXING type_idx;
    uint32 idx;
    bool send_out;
};


void shader_builder_init();

struct shader_descriptor shader_builder_build(const sid_t sid_attr, const sid_t sid_attr_out,
                                              const sid_t sid_unif, const sid_t sid_other);

void shader_builder_destroy();

sid_t shader_attributes_create_flag_id();

uint32 create_in_attribute(const char* name, const enum TYPES_INDEXING type);

sid_t shader_uniforms_create_flag_id();

uint32 create_in_uniform(const char* name, const enum TYPES_INDEXING type);

sid_t shader_outputs_create_flag_id();

uint32 create_output_variable(const char* name, const enum TYPES_INDEXING type);


/***************************************************************************************************
 * DEBUG-PRINT
 **************************************************************************************************/
const char* shaderinfo_type_to_glsltype_string(const enum TYPES_INDEXING type);

const uint32 shaderinfo_type_to_elems(const enum TYPES_INDEXING type);

void shader_attribs_desc_table_print();

void shader_uniforms_desc_table_print();


#endif //SHADER_BUILDER_SHADER_BUILDER_H

/*
 *
    {
        {.id = DOUBLE   , .size = 8, .name = "double"  },
        {.id = FLOAT    , .size = 4, .name = "float"   },
        {.id = INT64    , .size = 8, .name = "int64_t" },
        {.id = UINT64   , .size = 8, .name = "uint64_t"},
        {.id = INT32    , .size = 4, .name = "int32_t" },
        {.id = UINT32   , .size = 4, .name = "uint32_t"},
        {.id = INT16    , .size = 2, .name = "int16_t" },
        {.id = UINT16   , .size = 2, .name = "uint16_t"},
        {.id = INT8     , .size = 1, .name = "int8_t"  },
        {.id = UINT8    , .size = 1, .name = "uint8_t" },

        {.id = VEC2F    , .size = 2*4, .name = "vec2"    },
        {.id = VEC2I64  , .size = 2*8, .name = "i64vec2" },
        {.id = VEC2U64  , .size = 2*8, .name = "u64vec2" },
        {.id = VEC2I32  , .size = 2*4, .name = "u32vec2" },
        {.id = VEC2U32  , .size = 2*4, .name = "u32vec2" },
        {.id = VEC2I16  , .size = 2*2, .name = "i16vec2" },
        {.id = VEC2U16  , .size = 2*2, .name = "u16vec2" },
        {.id = VEC2I8   , .size = 2*1, .name = "u8vec2"  },
        {.id = VEC2U8   , .size = 2*1, .name = "u8vec2"  },

        {.id = VEC3F    , .size = 3*4, .name = "vec3" },{},{},{},{},{},{},{},{},
        {.id = VEC4F    , .size = 4*4, .name = "vec4" },{},{},{},{},{},{},{},{},

        {.id = MAT2X2F  , .size = 2*2*4, .name = "mat2"   },{},{},{},{},{},{},{},{},
        {.id = MAT2X3F  , .size = 2*3*4, .name = "mat2x3" },{},{},{},{},{},{},{},{},
        {.id = MAT3X3F  , .size = 3*3*4, .name = "mat3"   },{},{},{},{},{},{},{},{},
        {.id = MAT3X4F  , .size = 3*4*4, .name = "mat3x4" },{},{},{},{},{},{},{},{},
        {.id = MAT4X4F  , .size = 4*4*4, .name = "mat4"   },{},{},{},{},{},{},{},{},
    };
 */
