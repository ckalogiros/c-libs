/*
 * Created by ckalo on 4/8/26.
 */

#include "shader_builder.h"
#include <stdlib.h>
#include "file_sys_interface.h"
#include "logger.h"
#include "stringc.h"
#include "string_format.h"


const uint64 shader_variable_flags_table[] = {
    [SID_ATTR_OPACIT_IDX] = BIT_MASK_FROM_UINT(1),
    [SID_ATTR_VPOSI2_IDX] = BIT_MASK_FROM_UINT(2),
    [SID_ATTR_WPOSI2_IDX] = BIT_MASK_FROM_UINT(3),
    [SID_ATTR_SCALE2_IDX] = BIT_MASK_FROM_UINT(4),
    [SID_ATTR_ROTAT2_IDX] = BIT_MASK_FROM_UINT(5),
    [SID_ATTR_PIVOT2_IDX] = BIT_MASK_FROM_UINT(6),
    [SID_ATTR_UVCOO2_IDX] = BIT_MASK_FROM_UINT(7),
    [SID_ATTR_VPOSI3_IDX] = BIT_MASK_FROM_UINT(8),
    [SID_ATTR_WPOSI3_IDX] = BIT_MASK_FROM_UINT(9),
    [SID_ATTR_SCALE3_IDX] = BIT_MASK_FROM_UINT(10),
    [SID_ATTR_ROTAT3_IDX] = BIT_MASK_FROM_UINT(11),
    [SID_ATTR_PIVOT3_IDX] = BIT_MASK_FROM_UINT(12),
    [SID_ATTR_COLOR3_IDX] = BIT_MASK_FROM_UINT(13),
    [SID_ATTR_NORMA3_IDX] = BIT_MASK_FROM_UINT(14),
    [SID_ATTR_PROJMAT4X4_IDX] = BIT_MASK_FROM_UINT(15),
};

uint32 shader_variable_flags_count = 0;


sid_t
shader_create_flag_id(const uint64 idx)
{
    assert(idx < 64);
    sid_t sid = BIT_MASK_FROM_UINT(idx);
    return sid;
}

/***************************************************************************************************
 * ATTRIBUTES: IN
 **************************************************************************************************/
#define SHADER_ATTRIBUTES_ARRAY_COUNT 64
struct SHADER_VARIABLES_STRUCT shader_attributes_table[SHADER_ATTRIBUTES_ARRAY_COUNT] = {
    {.flagname = TO_STRING(SID_OPACIT), .flag = SID_OPACIT, .typesize = 4, .elems = 1, .selfidx = SID_ATTR_OPACIT_IDX, .type = FLOAT, .codename = "opacity",},
    {.flagname = TO_STRING(SID_VPOSI2), .flag = SID_VPOSI2, .typesize = 4, .elems = 2, .selfidx = SID_ATTR_VPOSI2_IDX, .type = VEC2F, .codename = "vpos2",},
    {.flagname = TO_STRING(SID_WPOSI2), .flag = SID_WPOSI2, .typesize = 4, .elems = 2, .selfidx = SID_ATTR_WPOSI2_IDX, .type = VEC2F, .codename = "wpos2",},
    {.flagname = TO_STRING(SID_SCALE2), .flag = SID_SCALE2, .typesize = 4, .elems = 2, .selfidx = SID_ATTR_SCALE2_IDX, .type = VEC2F, .codename = "scale2",},
    {.flagname = TO_STRING(SID_ROTAT2), .flag = SID_ROTAT2, .typesize = 4, .elems = 2, .selfidx = SID_ATTR_ROTAT2_IDX, .type = VEC2F, .codename = "rot2",},
    {.flagname = TO_STRING(SID_PIVOT2), .flag = SID_PIVOT2, .typesize = 4, .elems = 2, .selfidx = SID_ATTR_PIVOT2_IDX, .type = VEC2F, .codename = "pivot2",},
    {.flagname = TO_STRING(SID_UVCOO2), .flag = SID_UVCOO2, .typesize = 4, .elems = 2, .selfidx = SID_ATTR_UVCOO2_IDX, .type = VEC2F, .codename = "uv2",},
    {.flagname = TO_STRING(SID_VPOSI3), .flag = SID_VPOSI3, .typesize = 4, .elems = 3, .selfidx = SID_ATTR_VPOSI3_IDX, .type = VEC3F, .codename = "vpos3",},
    {.flagname = TO_STRING(SID_WPOSI3), .flag = SID_WPOSI3, .typesize = 4, .elems = 3, .selfidx = SID_ATTR_WPOSI3_IDX, .type = VEC3F, .codename = "wpos3",},
    {.flagname = TO_STRING(SID_SCALE3), .flag = SID_SCALE3, .typesize = 4, .elems = 3, .selfidx = SID_ATTR_SCALE3_IDX, .type = VEC3F, .codename = "scale3",},
    {.flagname = TO_STRING(SID_ROTAT3), .flag = SID_ROTAT3, .typesize = 4, .elems = 3, .selfidx = SID_ATTR_ROTAT3_IDX, .type = VEC3F, .codename = "rot3",},
    {.flagname = TO_STRING(SID_PIVOT3), .flag = SID_PIVOT3, .typesize = 4, .elems = 3, .selfidx = SID_ATTR_PIVOT3_IDX, .type = VEC3F, .codename = "pivot3",},
    {.flagname = TO_STRING(SID_COLOR3), .flag = SID_COLOR3, .typesize = 4, .elems = 3, .selfidx = SID_ATTR_COLOR3_IDX, .type = VEC3F, .codename = "color3",},
    {.flagname = TO_STRING(SID_NORMA3), .flag = SID_NORMA3, .typesize = 4, .elems = 3, .selfidx = SID_ATTR_NORMA3_IDX, .type = VEC3F, .codename = "normal3",},
    {.flagname = TO_STRING(SID_COLOR4), .flag = SID_COLOR4, .typesize = 4, .elems = 4, .selfidx = SID_ATTR_COLOR4_IDX, .type = VEC4F, .codename = "color4",},
    {.flagname = TO_STRING(SID_PROJMAT4X4), .flag = SID_PROJMAT4X4, .typesize = 4, .elems = 4 * 4, .selfidx = SID_UNIF_PROJMAT4X4_IDX, .type = MAT4X4F, .codename = "projection_matrix",},
};
uint32 shader_attributes_table_count = SID_ATTRIBS_IDX_COUNT;

uint32
create_in_attribute(const char* name, const enum TYPES_INDEXING type)
{
    uint8 idx = shader_attributes_table_count;

    const sid_t id = shader_create_flag_id(idx);
    shader_attributes_table[idx].flagname = name;
    shader_attributes_table[idx].codename = name;
    shader_attributes_table[idx].type = type;
    shader_attributes_table[idx].flag = id;
    shader_attributes_table[idx].selfidx = idx;
    shader_attributes_table[idx].elems = shaderinfo_type_to_elems(type);
    // shader_attributes_table[idx].send_out = send_out;
    // shader_attributes_table[idx].shaders = SHADER_VERTEX;

    shader_attributes_table_count++;
    return id;
}


/***************************************************************************************************
 * UNIFORMS
 **************************************************************************************************/
#define SHADER_UNIFORMS_ARRAY_COUNT 64
struct SHADER_VARIABLES_STRUCT shader_uniforms_table[SHADER_UNIFORMS_ARRAY_COUNT] = {
    {.flagname = TO_STRING(SID_OPACIT), .flag = shader_variable_flags_table[SID_UNIF_OPACIT_IDX], .typesize = 4, .elems = 1, .selfidx = SID_UNIF_OPACIT_IDX, .type = FLOAT, .codename = "opacity",},
    {.flagname = TO_STRING(SID_VPOSI2), .flag = SID_VPOSI2, .typesize = 4, .elems = 2, .selfidx = SID_UNIF_VPOSI2_IDX, .type = VEC2F, .codename = "vpos2",},
    {.flagname = TO_STRING(SID_WPOSI2), .flag = SID_WPOSI2, .typesize = 4, .elems = 2, .selfidx = SID_UNIF_WPOSI2_IDX, .type = VEC2F, .codename = "wpos2",},
    {.flagname = TO_STRING(SID_SCALE2), .flag = SID_SCALE2, .typesize = 4, .elems = 2, .selfidx = SID_UNIF_SCALE2_IDX, .type = VEC2F, .codename = "scale2",},
    {.flagname = TO_STRING(SID_ROTAT2), .flag = SID_ROTAT2, .typesize = 4, .elems = 2, .selfidx = SID_UNIF_ROTAT2_IDX, .type = VEC2F, .codename = "rot2",},
    {.flagname = TO_STRING(SID_PIVOT2), .flag = SID_PIVOT2, .typesize = 4, .elems = 2, .selfidx = SID_UNIF_PIVOT2_IDX, .type = VEC2F, .codename = "pivot2",},
    {.flagname = TO_STRING(SID_UVCOO2), .flag = SID_UVCOO2, .typesize = 4, .elems = 2, .selfidx = SID_UNIF_UVCOO2_IDX, .type = VEC2F, .codename = "uv2",},
    {.flagname = TO_STRING(SID_VPOSI3), .flag = SID_VPOSI3, .typesize = 4, .elems = 3, .selfidx = SID_UNIF_VPOSI3_IDX, .type = VEC3F, .codename = "vpos3",},
    {.flagname = TO_STRING(SID_WPOSI3), .flag = SID_WPOSI3, .typesize = 4, .elems = 3, .selfidx = SID_UNIF_WPOSI3_IDX, .type = VEC3F, .codename = "wpos3",},
    {.flagname = TO_STRING(SID_SCALE3), .flag = SID_SCALE3, .typesize = 4, .elems = 3, .selfidx = SID_UNIF_SCALE3_IDX, .type = VEC3F, .codename = "scale3",},
    {.flagname = TO_STRING(SID_ROTAT3), .flag = SID_ROTAT3, .typesize = 4, .elems = 3, .selfidx = SID_UNIF_ROTAT3_IDX, .type = VEC3F, .codename = "rot3",},
    {.flagname = TO_STRING(SID_PIVOT3), .flag = SID_PIVOT3, .typesize = 4, .elems = 3, .selfidx = SID_UNIF_PIVOT3_IDX, .type = VEC3F, .codename = "pivot3",},
    {.flagname = TO_STRING(SID_COLOR3), .flag = SID_COLOR3, .typesize = 4, .elems = 3, .selfidx = SID_UNIF_COLOR3_IDX, .type = VEC3F, .codename = "color3",},
    {.flagname = TO_STRING(SID_NORMA3), .flag = SID_NORMA3, .typesize = 4, .elems = 3, .selfidx = SID_UNIF_NORMA3_IDX, .type = VEC3F, .codename = "normal3",},
    {.flagname = TO_STRING(SID_PROJMAT4X4), .flag = SID_PROJMAT4X4, .typesize = 4, .elems = 4 * 4, .selfidx = SID_UNIF_PROJMAT4X4_IDX, .type = MAT4X4F, .codename = "projection_matrix",},
};
uint32 shader_uniforms_table_count = SID_UNIF_IDX_COUNT;

uint32
create_in_uniform(const char* name, const enum TYPES_INDEXING type)
{
    uint8 idx = shader_uniforms_table_count;

    const sid_t id = shader_create_flag_id(idx);

    shader_uniforms_table[idx].flagname = name;
    shader_uniforms_table[idx].codename = name;
    shader_uniforms_table[idx].type = type;
    shader_uniforms_table[idx].flag = id;
    shader_uniforms_table[idx].selfidx = idx;
    // shader_uniforms_table[idx].geometry_space = geometry_space;
    // shader_uniforms_table[idx].send_out = FALSE;
    // shader_uniforms_table[idx].shaders = shaders;

    shader_uniforms_table_count++;
    return id;
}


/***************************************************************************************************
 * OUTPUTS
 **************************************************************************************************/
#define SHADER_ATTRIBUTES_COUNT 64
struct SHADER_BUILDER_STRUCT shader_outputs_table[SHADER_ATTRIBUTES_COUNT];

sid_t
shader_outputs_create_flag_id()
{
    static uint32 cnt = 0;
    sid_t sid = BIT_MASK_FROM_UINT(cnt);
    cnt++;
    return sid;
}

uint32
create_output_variable(const char* name, const enum TYPES_INDEXING type)
{
    static uint8 count = 0;
    uint8 idx = count++;

    const sid_t id = shader_outputs_create_flag_id();
    shader_outputs_table[idx].name = name;
    shader_outputs_table[idx].type_idx = type;
    shader_outputs_table[idx].attrib_flag = id;
    shader_outputs_table[idx].idx = idx;
    shader_outputs_table[idx].send_out = FALSE;

    return idx;
}


/***************************************************************************************************
 * SHADER GENERIC LOCATION
 **************************************************************************************************/
struct generic_location {
    uint8 loc;
    uint8 size;
    uint8 type;
    uint8 offset;
};

struct generic_location
create_location(const uint8 loc, const uint8 size, const uint8 type, const uint8 offset)
{
    return (struct generic_location){.loc = loc, .size = size, .type = type, .offset = offset};
}


/***************************************************************************************************
 * SHADER BUILDER
 **************************************************************************************************/
void
shader_builder_init()
{
    /// initialize 4 pages(4096 bytes) for shader code
    string_init_table(4);
}

#define SHADERBUILDERGLSL_VERSION "#version 330 core\n\n"
#define SHADERBUILDERGLSL_VS_MAIN "void main()\n"
#define SHADERBUILDERGLSL_VS_GL_POSITION "  gl_Position\n"

void
create_attribute_in(const sid_t sid, string_t** const str, const char* format, const enum SHADER_ATTRIBUTES_INDEXING attribidx)
{
    if(sid & shader_attributes_table[attribidx].flag) {
        char buf[64] = {0};
        const char* s = string_format(&buf[0], format,
                                      shaderinfo_type_to_glsltype_string(shader_attributes_table[attribidx].type),
                                      shader_attributes_table[attribidx].codename);
        string_append_cstr(str, s);
    }
}

void
create_uniform_in(const sid_t sid, string_t** const str, const char* format, const enum SHADER_ATTRIBUTES_INDEXING attribidx)
{
    if(sid & shader_uniforms_table[attribidx].flag) {
        char buf[64] = {0};
        const char* s = string_format(&buf[0], format,
                                      shaderinfo_type_to_glsltype_string(shader_uniforms_table[attribidx].type),
                                      shader_uniforms_table[attribidx].codename);
        string_append_cstr(str, s);
    }
}

void
create_attributes_assignment(const sid_t sid, string_t** const str, const char* format, const enum SHADER_ATTRIBUTES_INDEXING attribidx)
{
    if(sid & shader_attributes_table[attribidx].flag) {
        char buf[64] = {0};
        const char* s = string_format(&buf[0], format,
                                      shader_attributes_table[attribidx].codename,
                                      shader_attributes_table[attribidx].codename);
        string_append_cstr(str, s);
    }
}

void
shader_builder_build(const sid_t sid_attr, const sid_t sid_attr_out, const sid_t sid_unif, const sid_t sid_other)
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

    string_append_cstr(&str, SHADERBUILDERGLSL_VERSION);

    // attributes: in
    string_append_cstr(&str, "/// attributes: in\n");
    // for(uint32 i = 0; i < shader_attributes_table_count; i++) {
    //     create_attribute_in(sid_attr, &str, "in %s %s;\n", i);
    //     /// TODO: create generic attribute locations
    // }
    uint32 loc_offset = 0; /// counts the locations element by element
    uint32 loc_cnt = 0; /// counts the locations
    {
        char buf[64] = {0};
        const char* s = string_format(&buf[0], "layout (location = %u) in vec4 a_generic%u;\n", loc_cnt, loc_cnt);
        string_append_cstr(&str, s);
    }
    for(uint32 i = 0; i < shader_attributes_table_count; i++) {

        // if(sid_attr & shader_attributes_table[i].flag)
        {

            uint32 remaining_elems = shader_attributes_table[i].elems;
            int32 excceding_elems = (remaining_elems + loc_offset) >= 4 ? (remaining_elems + loc_offset) - 4 : 0;
            if(excceding_elems > 0) {

                // if(loc_offset >= 4) loc_offset = loc_offset - 4;
                struct generic_location loc = create_location(loc_cnt, remaining_elems, shader_attributes_table[i].typesize, loc_offset);
                print("loc:%u elems:%u offset:%u\n", loc.loc, loc.size, loc.offset);
                loc_cnt++;

                char buf[64] = {0};
                const char* s = string_format(&buf[0], "layout (location = %u) in vec4 a_generic%u;\n", loc_cnt, loc_cnt);
                string_append_cstr(&str, s);
                // loc_offset += remaining_elems;
                loc_offset += shader_attributes_table[i].elems;
                if(loc_offset >= 4) loc_offset = loc_offset - 4;


            } else {
                struct generic_location loc = create_location(loc_cnt, shader_attributes_table[i].elems, shader_attributes_table[i].typesize, loc_offset);
                print("loc:%u elems:%u offset:%u\n", loc.loc, loc.size, loc.offset);
                loc_offset += shader_attributes_table[i].elems;
                // if(loc_offset >= 4) loc_offset = loc_offset - 4;
            }

        }
    }
    // for(uint32 i = 0; i < shader_attributes_table_count; i++) {
    //     if(sid_attr & shader_attributes_table[i].flag) {
    //
    //         int32 excceding_elems = (shader_attributes_table[i].elems + loc_offset) >= 4 ? (shader_attributes_table[i].elems + loc_offset) - 4 : 0;
    //         if(excceding_elems > 0) {
    //
    //             if(loc_offset >= 4) loc_offset = loc_offset - 4;
    //             loc_cnt++;
    //             struct generic_location loc = create_location(loc_cnt, shader_attributes_table[i].elems, shader_attributes_table[i].typesize, loc_offset);
    //             print("loc:%u elems:%u offset:%u\n", loc.loc, loc.size, loc.offset);
    //
    //             char buf[64] = {0};
    //             const char* s = string_format(&buf[0], "layout (location = %u) in vec4 a_generic%u;\n", loc_cnt, loc_cnt);
    //             string_append_cstr(&str, s);
    //             loc_offset += shader_attributes_table[i].elems;
    //         } else {
    //             struct generic_location loc = create_location(loc_cnt, shader_attributes_table[i].elems, shader_attributes_table[i].typesize, loc_offset);
    //             print("loc:%u elems:%u offset:%u\n", loc.loc, loc.size, loc.offset);
    //             loc_offset += shader_attributes_table[i].elems;
    //         }
    //     }
    // }
    string_append_cstr(&str, "\n");


    /// attributes: out
    string_append_cstr(&str, "/// attributes: out\n");
    for(uint32 i = 0; i < shader_attributes_table_count; i++) {
        create_attribute_in(sid_attr_out, &str, "out %s out_%s;\n", i);
    }
    string_append_cstr(&str, "\n");


    // uniforms: in
    string_append_cstr(&str, "/// uniforms: in\n");
    for(uint32 i = 0; i < shader_uniforms_table_count; i++) {
        create_uniform_in(sid_unif, &str, "uniform %s %s;\n", i);
    }
    string_append_cstr(&str, "\n");


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

    /// TODO: create attribute/uniform assignments

    // gl_position
    string_append_cstr(&str, "  vec4 final_pos = vec4(0.0f);\n");

    if((sid_attr & shader_attributes_table[SID_ATTR_VPOSI2_IDX].flag) || (sid_unif & shader_uniforms_table[SID_UNIF_VPOSI2_IDX].flag)) {
        char buf[64] = {0};
        const char* s = string_format(&buf[0], "  final_pos = %s;\n", shader_attributes_table[SID_ATTR_VPOSI2_IDX].codename);
        string_append_cstr(&str, s);
    }
    if((sid_attr & shader_attributes_table[SID_ATTR_ROTAT2_IDX].flag) || (sid_unif & shader_uniforms_table[SID_UNIF_ROTAT2_IDX].flag)) {
        {
            char buf[64] = {0};
            const char* codename = shader_attributes_table[SID_ATTR_ROTAT2_IDX].codename;
            const char* s = string_format(&buf[0], "  mat2 rot_matrix = mat2(%s.x, -%s.y, %s.x, %s.y);\n",
                                          codename, codename, codename, codename);
            string_append_cstr(&str, s);
            string_append_cstr(&str, "  final_pos = rot_matrix * final_pos;\n");
        }
    }
    if((sid_attr & shader_attributes_table[SID_ATTR_SCALE2_IDX].flag) || (sid_unif & shader_uniforms_table[SID_UNIF_SCALE2_IDX].flag)) {
        char buf[64] = {0};
        const char* s = string_format(&buf[0], "  final_pos = final_pos * %s;\n", shader_attributes_table[SID_UNIF_SCALE2_IDX].codename);
        string_append_cstr(&str, s);
    }
    if((sid_attr & shader_attributes_table[SID_ATTR_WPOSI3_IDX].flag) || (sid_unif & shader_uniforms_table[SID_UNIF_WPOSI3_IDX].flag)) {
        char buf[64] = {0};
        const char* s = string_format(&buf[0], "  final_pos = final_pos + %s;\n", shader_attributes_table[SID_UNIF_WPOSI3_IDX].codename);
        string_append_cstr(&str, s);
    }


    if((sid_attr & shader_attributes_table[SID_ATTR_PROJMAT4X4_IDX].flag) || (sid_unif & shader_uniforms_table[SID_UNIF_PROJMAT4X4_IDX].flag)) {
        char buf[64] = {0};
        const char* s = string_format(&buf[0], "  gl_Position = %s * final_pos;\n", shader_attributes_table[SID_UNIF_PROJMAT4X4_IDX].codename);
        string_append_cstr(&str, s);
    } else string_append_cstr(&str, "  gl_Position = final_pos;\n");

    string_append_cstr(&str, "\n  /// outputs assignment\n");
    // output assignments
    for(uint32 i = 0; i < SID_ATTRIBS_IDX_COUNT; i++) {
        create_attributes_assignment(sid_attr_out, &str, "  out_%s = %s;\n", i);
    }

    string_append_cstr(&str, "}\n");

    print("%s\n", str->data);
}

void
shader_builder_destroy()
{
    string_destroy_table();
}


/* DEBUG-PRINT */
void
shader_attribs_info_table_print()
{
    print("-- %s ATTRIBUTES ARRAY %s --\n", YELLOW, CLEAR);
    for(uint32 i = 0; i < SHADER_ATTRIBUTES_COUNT; i++) {
        struct SHADER_VARIABLES_STRUCT a = shader_attributes_table[i];
        if(a.flag)
            print("name: %s %-8s %s "
                  "type idx: %s%3u%s "
                  "type name: %s%7s%s "
                  "attrib_flag: %s%8llu%s "
                  "type size: %s%u%s "
                  "type num elems: %s%2u%s "
                  "self idx: %s%2u%s\n",
                  ry, shader_attributes_table[i].codename, CLEAR,
                  ry, shader_attributes_table[i].type, CLEAR,
                  ry, shaderinfo_type_to_glsltype_string(shader_attributes_table[i].type), CLEAR,
                  ry, shader_attributes_table[i].flag, CLEAR,
                  ry, shader_attributes_table[i].typesize, CLEAR,
                  ry, shader_attributes_table[i].elems, CLEAR,
                  ry, shader_attributes_table[i].selfidx, CLEAR);
    }
    print("\n");
}

void
shader_uniforms_info_table_print()
{
    print("-- %s UNIFORMS ARRAY %s --\n", YELLOW, CLEAR);
    for(uint32 i = 0; i < SHADER_ATTRIBUTES_COUNT; i++) {
        struct SHADER_VARIABLES_STRUCT a = shader_uniforms_table[i];
        if(a.flag)
            print("name: %s %-8s %s "
                  "type idx: %s%3u%s "
                  "type name: %s%7s%s "
                  "attrib_flag: %s%8llu%s "
                  "type size: %s%u%s "
                  "type num elems: %s%2u%s "
                  "self idx: %s%2u%s\n",
                  ry, shader_uniforms_table[i].codename, CLEAR,
                  ry, shader_uniforms_table[i].type, CLEAR,
                  ry, shaderinfo_type_to_glsltype_string(shader_uniforms_table[i].type), CLEAR,
                  ry, shader_uniforms_table[i].flag, CLEAR,
                  ry, shader_uniforms_table[i].typesize, CLEAR,
                  ry, shader_uniforms_table[i].elems, CLEAR,
                  ry, shader_uniforms_table[i].selfidx, CLEAR);
        // else print("ERROR: attribute at:%u not set\n", i);
    }
    print("\n");
}

const char*
shaderinfo_type_to_glsltype_string(const enum TYPES_INDEXING type)
{
    switch(type) {
        case FLOAT: return "FLOAT";
        case INT64: return "INT64";
        case UINT64: return "UINT64";
        case UINT32: return "UINT32";
        case UINT16: return "UINT16";
        case INT8: return "INT8";
        case UINT8: return "UINT8";
        case VEC2F: return "VEC2F";
        case VEC3F: return "VEC3F";
        case VEC4F: return "VEC4F";
        case MAT2X2F: return "MAT2X2F";
        case MAT3X3F: return "MAT3X3F";
        case MAT4X4F: return "MAT4X4F";
        default: return "NONE";
    }
}
const uint32
shaderinfo_type_to_elems(const enum TYPES_INDEXING type)
{
    switch(type) {
        case FLOAT:   return 1;
        case INT64:   return 1;
        case UINT64:  return 1;
        case UINT32:  return 1;
        case UINT16:  return 1;
        case INT8:    return 1;
        case UINT8:   return 1;
        case VEC2F:   return 2;
        case VEC3F:   return 3;
        case VEC4F:   return 4;
        case MAT2X2F: return 2*2;
        case MAT3X3F: return 3*3;
        case MAT4X4F: return 4*4;
        default: assert(1);
    }
}
