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


typedef uint64 shader_flags;
typedef int32 build_t;


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

struct SHADER_VARIABLES_STRUCT {
    /* flag | type | elems | typesize | flagname | codername | selfidx *
     * 0x1  | 1    | 3     | 4        | COL3     | color3    | 1       */
    const char* flagname;
    const char* codename;
    uint32 flag;
    enum TYPES_INDEXING type;
    enum SHADER_INPUT_FLAGS which_renameme;
    uint8 typesize;
    uint8 elems;
    uint8 var_table_idx;
};


#define SHADER_INPUT_ENTRIES_MAX_COUNT 16

struct shader_build {
    struct gfx_sid sid;
    struct SHADER_VARIABLES_STRUCT entry[SHADER_INPUT_ENTRIES_MAX_COUNT];
    uint32 entry_count;
};


void shader_builder_init(void);

struct shader_build shader_builds_get_build(build_t idx);

struct shader_descriptor shader_builder_build(const struct gfx_sid sid);

void shader_builder_destroy(void);

sid_t shader_attributes_create_flag_id();

build_t shader_create_input(const struct gfx_sid sid, const char* name, const enum TYPES_INDEXING type, enum SID_VAR_IDI rename, enum SHADER_INPUT_FLAGS which);

sid_t shader_uniforms_create_flag_id();

uint32 create_in_uniform(const char* name, const enum TYPES_INDEXING type);

sid_t shader_outputs_create_flag_id(void);

uint32 create_output_variable(const char* name, const enum TYPES_INDEXING type);


/***************************************************************************************************
 * DEBUG-PRINT
 **************************************************************************************************/
const char* shaderinfo_rename_me(const enum SID_VAR_IDI type);

const char* shaderinfo_type_to_glsltype_string(const enum TYPES_INDEXING type);

uint32 shaderinfo_type_to_numelems(const enum TYPES_INDEXING type);

uint32 shaderinfo_type_to_typesize(const enum TYPES_INDEXING type);

void shader_inputs_desc_table_print(void);

void shader_uniforms_desc_table_print(void);

void temp_print_builds(void);


#endif //SHADER_BUILDER_SHADER_BUILDER_H
