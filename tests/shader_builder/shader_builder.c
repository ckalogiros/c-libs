/*
 * Created by ckalo on 4/8/26.
 */

#include "shader_builder.h"
#include <stdlib.h>
#include "file_sys_interface.h"
#include "logger.h"
#include "my_macros.h"
#include "stringc.h"
#include "string_format.h"



char* shader_code = NULL;





/***************************************************************************************************
 * ATTRIBUTES: IN
 **************************************************************************************************/
#define SHADER_ATTRIBUTES_COUNT 64
struct SHADER_BUILDER_STRUCT shader_attribs_info_table[SHADER_ATTRIBUTES_COUNT];

sid_t shader_attributes_create_flag_id()
{
    static uint32 cnt = 0;
    sid_t sid = BIT_MASK_FROM_UINT(cnt);
    cnt++;
    return sid;
}

uint32 create_in_attribute(const char* name, const enum TYPES_INDEXING type, const bool send_out, const shader_flags geometry_space)
{
    static uint8 count = 0;
    uint8 idx = count++;

    const sid_t id = shader_attributes_create_flag_id();
    shader_attribs_info_table[idx].name = name;
    shader_attribs_info_table[idx].type_idx = type;
    shader_attribs_info_table[idx].attrib_flag = id;
    shader_attribs_info_table[idx].geometry_space = geometry_space;
    shader_attribs_info_table[idx].idx = idx;
    shader_attribs_info_table[idx].send_out = send_out;
    shader_attribs_info_table[idx].shaders = SHADER_VERTEX;

    return idx;
}




/***************************************************************************************************
 * UNIFORMS
 **************************************************************************************************/
#define SHADER_ATTRIBUTES_COUNT 64
struct SHADER_BUILDER_STRUCT shader_uniforms_info_table[SHADER_ATTRIBUTES_COUNT];

sid_t shader_uniforms_create_flag_id()
{
    static uint32 cnt = 0;
    sid_t sid = BIT_MASK_FROM_UINT(cnt);
    cnt++;
    return sid;
}

uint8 shader_uniforms_set(const char* name, const enum TYPES_INDEXING type, const sid_t id)
{
    static uint8 count = 0;
    uint8 idx = count++;

    shader_uniforms_info_table[idx].name = name;
    shader_uniforms_info_table[idx].type_idx = type;
    shader_uniforms_info_table[idx].attrib_flag = id;
    shader_uniforms_info_table[idx].idx = idx;

    return idx;
}

uint8 count = 0;
uint32 create_in_uniform(const char* name, const enum TYPES_INDEXING type, enum SHADERS shaders, const shader_flags geometry_space)
{
    // static uint8 count = 0;
    uint8 idx = count++;

    const sid_t id = shader_uniforms_create_flag_id();

    shader_uniforms_info_table[idx].name = name;
    shader_uniforms_info_table[idx].type_idx = type;
    shader_uniforms_info_table[idx].attrib_flag = id;
    shader_uniforms_info_table[idx].geometry_space = geometry_space;
    shader_uniforms_info_table[idx].idx = idx;
    shader_uniforms_info_table[idx].send_out = FALSE;
    shader_uniforms_info_table[idx].shaders = shaders;

    return idx;
}




/***************************************************************************************************
 * OUTPUTS
 **************************************************************************************************/
#define SHADER_ATTRIBUTES_COUNT 64
struct SHADER_BUILDER_STRUCT shader_outputs_info_table[SHADER_ATTRIBUTES_COUNT];

sid_t shader_outputs_create_flag_id()
{
    static uint32 cnt = 0;
    sid_t sid = BIT_MASK_FROM_UINT(cnt);
    cnt++;
    return sid;
}

uint32 create_output_variable(const char* name, const enum TYPES_INDEXING type)
{
    static uint8 count = 0;
    uint8 idx = count++;

    const sid_t id = shader_outputs_create_flag_id();
    shader_outputs_info_table[idx].name = name;
    shader_outputs_info_table[idx].type_idx = type;
    shader_outputs_info_table[idx].attrib_flag = id;
    shader_outputs_info_table[idx].idx = idx;
    shader_outputs_info_table[idx].send_out = FALSE;

    return idx;
}



/***************************************************************************************************
 * SHADER BUILDER
 **************************************************************************************************/
void shader_builder_init()
{
    shader_code = (char*)malloc(4096);
    string_init_table(4);
}
#define SHADERBUILDERGLSL_VERSION "#version 330 core\n"
#define SHADERBUILDERGLSL_VS_MAIN "void main()\n"
#define SHADERBUILDERGLSL_VS_GL_POSITION "  gl_Position\n"
void shader_builder_build()
{
    /*
        --- VETREX ---
        #version 330 core
        layout (location = 0) in vec4 vCol;
        layout (location = 1) in vec2 vPos;
        layout (location = 1) in vec2 vSca;
        layout (location = 1) in vec2 vRot;
        out vec4 color;
        uniform mat4 MVP;
        void main()
        {
            gl_Position = MVP * vec4(vPos, 0.0, 1.0);
            color = vCol;
        }

        --- FRAGMENT ---
        #version 330 core
        in vec3 color;
        out vec4 fragment;
        void main()
        {
            fragment = vec4(color, 1.0);
        }
    */
    string_t* str = string_create_empty(2048);
    uint32 loc = 0;

    string_append_cstr(&str, SHADERBUILDERGLSL_VERSION);


    /// attributes: in
    const uint32 attribs_len = ARRAY_LENGTH(shader_attribs_info_table);
    for(uint32 i = 0; i < attribs_len; i++) {

        if (shader_attribs_info_table[i].attrib_flag != 0) {

            char buf[64] = {0};
            const char* s = string_format(
                &buf[0],
                "layout (location = %u) in %s %s;\n",
                loc++, shaderinfo_type_to_string(shader_attribs_info_table[i].type_idx),
                shader_attribs_info_table[i].name
            );
            string_append_cstr(&str, s);
        }
    }

    /// uniforms: in
    const uint32 uniforms_len = ARRAY_LENGTH(shader_uniforms_info_table);
    for(uint32 i = 0; i < uniforms_len; i++) {

        if (shader_uniforms_info_table[i].attrib_flag != 0) {
            char buf[64] = {0};
            const char* s = string_format(
                &buf[0],
                "uniform %s %s;\n",
                shaderinfo_type_to_string(shader_uniforms_info_table[i].type_idx),
                shader_uniforms_info_table[i].name
            );
            string_append_cstr(&str, s);
        }
    }

    // variables: out
    const uint32 outputs_len = ARRAY_LENGTH(shader_outputs_info_table);
    for(uint32 i = 0; i < outputs_len; i++) {

        if (shader_outputs_info_table[i].attrib_flag != 0) {
            char buf[64] = {0};
            const char* s = string_format(
            &buf[0],
            "out %s %s;\n",
            shaderinfo_type_to_string(shader_outputs_info_table[i].type_idx),
            shader_outputs_info_table[i].name
            );
            string_append_cstr(&str, s);
        }

    }

    /// main()
    string_append_cstr(&str, SHADERBUILDERGLSL_VS_MAIN);
    string_append_cstr(&str, "{\n");


    /*
        Local Space → World Space → View Space → Clip Space
        gl_Position = u_projection * u_viewMatrix * u_modelMatrix * a_position;

        vec2 finalPos = rotationMatrix * (vertex_pos * scale) + world_pos.xy;

        vec2 transformed2D = rotationMatrix * (vertex_pos * scale) + world_pos.xy;
        vec4 finalPos = vec4(transformed2D, world_pos.z, 1.0);


        // 1. Start with vertex in local/model space
        vec2 final_pos = vertexPosition;

        // 2. if there is scale: Apply scale
        final_pos = final_pos * scale;  // or scale * final_pos depending on convention

        // 3. if there is rotation: Apply rotation (around origin)
        final_pos = rotationMatrix * final_pos;

        // 4. if there is world position: Translate to world position
        final_pos = final_pos + worldPosition;

        // 5. Apply projection matrix (and view if combined)
        gl_Position = u_projectionMatrix * vec4(final_pos, 0.0, 1.0);
     */

    // gl_position
    string_append_cstr(&str, "  vec4 final_pos = vec4(0.0f);\n");

    for(uint32 i = 0; i < attribs_len; i++) {

        if (shader_attribs_info_table[i].geometry_space) {

            char buf[64] = {0};
            char buf2[32] = {0};
            const char* name = shader_attribs_info_table[i].name;

            if(shader_attribs_info_table[i].type_idx == VEC2F) {
                string_format(&buf2[0], "vec4(%s, 0.0f, 0.0f)", name);
            }
            if(shader_attribs_info_table[i].type_idx == VEC3F) {
                string_format(&buf2[0], "vec4(%s, 0.0f)", name);
            }

            if(buf2[0]) {
                const shader_flags space = shader_attribs_info_table[i].geometry_space;
                char telestis[2] = {' ','\0'};
                if(space & TRANSFORM_LOCAL_SPACE) telestis[0] = '*';
                else if(space & TRANSFORM_WORLD_SPACE) telestis[0] = '+';
                const char* s = string_format(&buf[0], "  final_pos %s= %s;\n", &telestis, buf2);
                string_append_cstr(&str, s);
            }

        }
    }

    for(uint32 i = 0; i < uniforms_len; i++) {

        if (shader_uniforms_info_table[i].geometry_space) {

            char buf[64] = {0};
            char buf2[32] = {0};
            const char* name = shader_uniforms_info_table[i].name;
            const char* code_type = shaderinfo_type_to_string(shader_uniforms_info_table[i].type_idx);

            if(shader_uniforms_info_table[i].type_idx == VEC2F) {
                string_format(&buf2[0], "vec4(%s, 0.0f, 0.0f)", name);
            }
            if(shader_uniforms_info_table[i].type_idx == VEC3F) {
                string_format(&buf2[0], "vec4(%s, 0.0f)", name);
            }

            if(buf2[0]) {
                const shader_flags space = shader_uniforms_info_table[i].geometry_space;
                char telestis[2] = {' ','\0'};
                if(space & TRANSFORM_LOCAL_SPACE) telestis[0] = '*';
                else if(space & TRANSFORM_WORLD_SPACE) telestis[0] = '+';
                const char* s = string_format(&buf[0], "  final_pos %s= %s;\n", &telestis, buf2);
                string_append_cstr(&str, s);
            }

        }
    }

    string_append_cstr(&str, "}\n");

    print("%s\n", str->data);

}
void shader_builder_destroy()
{
    free(shader_code);

    string_destroy_table();
}





/* DEBUG-PRINT */
void shader_attribs_info_table_print()
{
    for(uint32 i = 0; i < SHADER_ATTRIBUTES_COUNT; i++) {
        struct SHADER_BUILDER_STRUCT a  = shader_attribs_info_table[i];
        if(a.name)
        print("name: %s%s%s "
          "type idx: %s%u%s "
          "type name: %s%s%s "
          "attrib_flag: %s%u%s "
          "self idx: %s%u%s\n",
          ry, shader_attribs_info_table[i].name, CLEAR,
          ry, shader_attribs_info_table[i].type_idx, CLEAR,
          ry, shaderinfo_type_to_string(shader_attribs_info_table[i].type_idx), CLEAR,
          ry, shader_attribs_info_table[i].attrib_flag, CLEAR,
          ry, shader_attribs_info_table[i].idx, CLEAR);
        // else print("ERROR: attribute at:%u not set\n", i);
    }
}
void shader_uniforms_info_table_print()
{
    for(uint32 i = 0; i < SHADER_ATTRIBUTES_COUNT; i++) {
        struct SHADER_BUILDER_STRUCT a  = shader_uniforms_info_table[i];
        if(a.name)
        print("name: %s%s%s "
          "type idx: %s%u%s "
          "type name: %s%s%s "
          "attrib_flag: %s%u%s "
          "geometry_space: %s%u%s "
          "self idx: %s%u%s\n",
          ry, shader_uniforms_info_table[i].name, CLEAR,
          ry, shader_uniforms_info_table[i].type_idx, CLEAR,
          ry, shaderinfo_type_to_string(shader_uniforms_info_table[i].type_idx), CLEAR,
          ry, shader_uniforms_info_table[i].attrib_flag, CLEAR,
          ry, shader_uniforms_info_table[i].geometry_space, CLEAR,
          ry, shader_uniforms_info_table[i].idx, CLEAR);
        // else print("ERROR: attribute at:%u not set\n", i);
    }
}

const char* shaderinfo_type_to_string(const enum TYPES_INDEXING type)
{
    switch(type) {
        case FLOAT:  return "FLOAT";
        case INT64:  return "INT64";
        case UINT64: return "UINT64";
        case UINT32: return "UINT32";
        case UINT16: return "UINT16";
        case INT8:   return "INT8";
        case UINT8:  return "UINT8";
        case VEC2F:  return "VEC2F";
        case VEC3F:  return "VEC3F";
        case VEC4F:  return "VEC4F";
        case MAT2X2F:  return "MAT2X2F";
        case MAT3X3F:  return "MAT3X3F";
        case MAT4X4F:  return "MAT4X4F";
        default:     return "NONE";
    }
}
