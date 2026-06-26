/*
 * Created by ckalo on 4/8/26.
 */
#ifndef SHADER_BUILDER_SHADER_BUILDER_H
#define SHADER_BUILDER_SHADER_BUILDER_H

#include "my_types.h"
#include "shader_descriptor.h"
#include "shader_id.h"



// struct shader_build {
//     struct gfx_sid sid;
//     struct shader_in_variables_struct entry[SHADER_INPUT_ENTRIES_MAX_COUNT];
//     uint32 entry_count;
// };


void shader_builder_init(void);

struct shader_descriptor* shader_builds_get_build(build_t idx);

struct shader_descriptor* shader_builder_build(const sididx_t sid_idx, const build_t build_idx);

build_t shader_build_exists(const sididx_t sid_idx);

build_t shader_build_is_set(const sididx_t sid_idx);

build_t shader_builder_create_new_build(const sididx_t sid_idx);

void shader_builder_destroy(void);

sid_t shader_attributes_create_flag_id();

build_t shader_create_input(const sididx_t sid_idx, const char* name, const enum TYPES_INDEXING type, enum SID_ID rename, enum SHADER_INPUT_FLAGS which);

sid_t shader_uniforms_create_flag_id();

uint32 create_in_uniform(const char* name, const enum TYPES_INDEXING type);

sid_t shader_outputs_create_flag_id(void);

uint32 create_output_variable(const char* name, const enum TYPES_INDEXING type);


/***************************************************************************************************
 * DEBUG-PRINT
 **************************************************************************************************/
// const char* shaderinfo_flag_to_flagname(const sid_t sid_flag);
//
// const char* shaderinfo_id_to_flagname(const enum SID_ID type);
//
// const char* shaderinfo_type_to_glsltype_string(const enum TYPES_INDEXING type);
//
// enum SID_ID shaderinfo_flag_to_id(const sid_t sid_flag);
//
// sid_t shaderinfo_id_to_flag(const enum SID_ID type);
//
// uint8 shaderinfo_flag_to_numelems(const sid_t sid_flag);
//
// uint8 shaderinfo_type_to_numelems(const enum TYPES_INDEXING type);
//
// uint8 shaderinfo_type_to_typesize(const enum TYPES_INDEXING type);

void shader_inputs_desc_table_print(void);

void shader_uniforms_desc_table_print(void);

void shader_desc_table_print_build_entry(const build_t idx, const uint8 entryidx);

void temp_print_builds(void);


#endif //SHADER_BUILDER_SHADER_BUILDER_H
