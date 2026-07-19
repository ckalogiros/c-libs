/*
 * Created by ckalo on 4/8/26.
 */
#ifndef SHADER_BUILDER_SHADER_BUILDER_H
#define SHADER_BUILDER_SHADER_BUILDER_H

#include "attributes.h"
#include "my_types.h"
#include "shader_descriptor.h"
#include "shader_id.h"

struct RET_attrib_input {
    build_t build_idx;
    attr_t shader_attrib_idx;
};

struct shader_entry* shader_input_get_entry(const enum SID_ID shader_input_id);


void shader_builder_init(void);

struct shader_descriptor* shader_builds_get_build(build_t idx);

struct shader_descriptor* shader_builder_build(const sididx_t sid_idx, const build_t build_idx, bool temp_divisor);

build_t shader_build_exists(const sididx_t sid_idx);

build_t shader_build_is_set(const sididx_t sid_idx);

build_t shader_builder_create_new_build(const sididx_t sid_idx);

void shader_builder_destroy(void);

sid_t shader_attributes_create_flag_id();

// struct RET_attrib_input shader_create_input(const sididx_t sid_idx, const char* name, const enum TYPE_ID type, enum SID_ID rename, enum SHADER_INPUT_FLAGS which, const uint16 divisor);
struct RET_attrib_input shader_create_input(const build_t build_idx, enum SID_ID shader_input_id, const enum SHADER_INPUT_FLAGS input_flag, const uint16 divisor);


struct shader_entry* shader_create_new_input(enum SID_ID shader_input_id, const char* name, const enum TYPE_ID type);

// sid_t shader_uniforms_create_flag_id();

// uint32 create_in_uniform(const char* name, const enum TYPES_INDEXING type);

uint32 shader_create_output(const char* name, const enum TYPE_ID type);


/***************************************************************************************************
 * DEBUG-PRINT
 **************************************************************************************************/
void shader_inputs_desc_table_print(void);

void shader_uniforms_desc_table_print(void);

void shader_desc_table_print_build_entries(const build_t idx);

void temp_print_builds(void);


#endif //SHADER_BUILDER_SHADER_BUILDER_H
