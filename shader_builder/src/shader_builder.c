/*
 * Created by ckalo on 4/8/26.
 */

#include "shader_builder.h"
#include "shader_descriptor.h"
#include <stdlib.h>
#include "file_sys_interface.h"
#include "logger.h"
#include "stringc.h"
#include "string_format.h"




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
    {.flagname = TO_STRING(SID_OPACIT), .flag = SID_OPACIT, .typesize = 4, .elems = 1, .selfidx = SID_UNIF_OPACIT_IDX, .type = FLOAT, .codename = "opacity",},
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
    {.flagname = TO_STRING(SID_COLOR4), .flag = SID_COLOR4, .typesize = 4, .elems = 3, .selfidx = SID_UNIF_COLOR4_IDX, .type = VEC4F, .codename = "color4",},
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
struct vertex_layout
create_location(const uint8 loc, const uint8 size, const uint8 type, const uint8 offset)
{
    return (struct vertex_layout){.loc = loc, .size = size, .type = type, .offset = offset};
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
create_attribute_out(const sid_t sid, string_t** const str, const char* format, const enum SHADER_ATTRIBUTES_INDEXING attribidx)
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
create_attributes_assignment(const sid_t sid, const sid_t sid_pass, string_t** const str, const char* format, const enum SHADER_ATTRIBUTES_INDEXING attribidx)
{
    if( (sid_pass & shader_attributes_table[attribidx].flag) &&
        (sid & shader_attributes_table[attribidx].flag)) {
        char buf[64] = {0};
        const char* s = string_format(&buf[0], format,
                                      shader_attributes_table[attribidx].codename,
                                      shader_attributes_table[attribidx].codename);
        string_append_cstr(str, s);
    }
}

#define MAX_NUM_ELEMS_PER_LOCATION 4
#define GENERIC_ATTR_LOC "GENERIC_ATTR_LOC"

struct shader_descriptor
shader_builder_build(const sid_t sid_attr, const sid_t sid_attr_out, const sid_t sid_unif, const sid_t sid_pass)
{
    struct shader_descriptor desc = {
        .vshader_attribs_idx = INT_NULL,
        .vshader_attribs_count = 0,
        .vshader_code = NULL,
    };

    string_t* str = string_create_empty(2048);

    string_append_cstr(&str, SHADERBUILDERGLSL_VERSION);

    // attributes: in
    string_append_cstr(&str, "/// attributes: in\n");

    uint8 loc_offset = 0; /// the start offset of each attribute
    uint8 loc_cnt = 0; /// locations index
    int8 prev_loc_idx = -1;




    /***********************************************************************************************
     * INPUTS */
    // storage of current shader's attributes info (location, offset, typesize, ...)
    struct vertex_layout attributes_desc[SID_ATTRIBS_IDX_COUNT] = {0};
    uint32 attr_desc_cnt = 0; /// attributes_desc counter

    /* algorithm 1: create generic locations */
    for(uint32 i = 0; i < shader_attributes_table_count; i++) {

        if(sid_attr & shader_attributes_table[i].flag) {

            const uint8 num_elems = shader_attributes_table[i].elems;
            const uint8 type = shader_attributes_table[i].type;
            const char* name = shader_attributes_table[i].codename;

            // Record the start position for this attribute
            struct vertex_layout loc = (struct vertex_layout){.loc = loc_cnt, .size = num_elems, .type = type, .offset = loc_offset, .idx = i};
            attributes_desc[attr_desc_cnt++] = loc; // store by attributes, not by locations
            print("loc:%u elems:%u offset:%u {%s}\n", loc.loc, loc.size, loc.offset, name);

            if(prev_loc_idx != loc_cnt) {
                char buf[64] = {0};
                const char* s = string_format(&buf[0], "layout (location = %u) in vec4 generic_%u;\n", loc.loc, loc.loc);
                string_append_cstr(&str, s);
                prev_loc_idx = loc_cnt;
            }

            // Advance by the number of elements this attribute occupies
            loc_offset += num_elems;

            while(loc_offset >= 4) {
                loc_offset -= 4;
                loc_cnt++;
                if(loc_offset >= 4) {
                    loc = (struct vertex_layout){.loc = loc_cnt, .size = num_elems, .type = type, .offset = 0, .idx = i};
                    print("+loc:%u elems:%u offset:%u {%s}\n", loc.loc, loc.size, loc.offset, name);
                    if(prev_loc_idx != loc_cnt) {
                        char buf[64] = {0};
                        const char* s = string_format(&buf[0], "layout (location = %u) in vec4 generic_%u;\n", loc.loc, loc.loc);
                        string_append_cstr(&str, s);
                        prev_loc_idx = loc_cnt;
                    }
                }
            }

        }
    }

    string_append_cstr(&str, "\n");


    /// attributes: out
    string_append_cstr(&str, "/// attributes: out\n");
    for(uint32 i = 0; i < shader_attributes_table_count; i++) {
        create_attribute_out(sid_attr_out, &str, "out %s out_%s;\n", i);
    }
    string_append_cstr(&str, "\n");


    // uniforms: in
    string_append_cstr(&str, "/// uniforms: in\n");
    for(uint32 i = 0; i < shader_uniforms_table_count; i++) {
        create_uniform_in(sid_unif, &str, "uniform %s %s;\n", i);
    }
    string_append_cstr(&str, "\n");




    /***********************************************************************************************
     * MAIN() */
    string_append_cstr(&str, SHADERBUILDERGLSL_VS_MAIN);
    string_append_cstr(&str, "{\n");




    /***********************************************************************************************
     * REASSEMBLE */
    const char vec4[4] = {'x', 'y', 'z', 'w'};
    uint32 gen_loc_cnt = 0;
    for(uint32 i = 0; i < attr_desc_cnt; i++) {

        uint32 num_elems = attributes_desc[i].size;
        if(num_elems <= MAX_NUM_ELEMS_PER_LOCATION) {

            uint32 j = 0;
            for(; j < num_elems; j++) {
                char buf[128] = {0};
                const char* s =
                        string_format(&buf[0],
                                      "  float %s_%c = generic_%u.%c;\n",
                                      shader_attributes_table[attributes_desc[i].idx].codename,
                                      vec4[j], attributes_desc[i].loc, vec4[gen_loc_cnt++]
                        );
                if(gen_loc_cnt >= MAX_NUM_ELEMS_PER_LOCATION) gen_loc_cnt = 0;
                string_append_cstr(&str, s);
            }
        }
    }
    string_append_cstr(&str, "\n");




    /***********************************************************************************************
     * TRANSFORM */
    const uint8 transform_case_1 = 1;
    const uint8 transform_case_2 = 2;
    const uint8 transform_case_3 = 3;
    uint8 transform_case = 0;

    if((sid_attr & shader_attributes_table[SID_ATTR_VPOSI2_IDX].flag)) {
        if((sid_attr & shader_attributes_table[SID_ATTR_SCALE2_IDX].flag)) {
            if((sid_attr & shader_attributes_table[SID_ATTR_ROTAT2_IDX].flag)) {
                transform_case = transform_case_3;
            } else {
                transform_case = transform_case_2;
            }
        } else {
            transform_case = transform_case_1;
        }
    }

    /// VPOS - SCALE - ROTATE
    if(transform_case == transform_case_1) {
        char buf[128] = {0};
        const char* type =
                shaderinfo_type_to_glsltype_string(shader_attributes_table[SID_ATTR_VPOSI2_IDX].type);
        const char* s =
                string_format(&buf[0],
                              "  %s transform = %s(%s_x, %s_y );\n",
                              type, type,
                              shader_attributes_table[SID_ATTR_VPOSI2_IDX].codename,
                              shader_attributes_table[SID_ATTR_VPOSI2_IDX].codename
                );
        string_append_cstr(&str, s);
    }
    else if(transform_case == transform_case_2) {
        char buf[128] = {0};
        const char* type =
                shaderinfo_type_to_glsltype_string(shader_attributes_table[SID_ATTR_VPOSI2_IDX].type);
        const char* s =
                string_format(&buf[0],
                              "  %s transform = %s( (%s_x*%s_x), (%s_y*%s_y) );\n",
                              type, type,
                              shader_attributes_table[SID_ATTR_SCALE2_IDX].codename,
                              shader_attributes_table[SID_ATTR_VPOSI2_IDX].codename,
                              shader_attributes_table[SID_ATTR_SCALE2_IDX].codename,
                              shader_attributes_table[SID_ATTR_VPOSI2_IDX].codename
                );
        string_append_cstr(&str, s);
    }
    else if(transform_case == transform_case_3) {
        char buf[256] = {0};
        const char* type =
                shaderinfo_type_to_glsltype_string(shader_attributes_table[SID_ATTR_VPOSI2_IDX].type);
        const char* s =
                string_format(&buf[0],
                              "  %s transform = %s( (%s_x*(%s_x*%s_x)) - (%s_y*(%s_y*%s_y)), \n"
                              "                         (%s_y*(%s_x*%s_x)) + (%s_x*(%s_y*%s_y)) );\n",
                              type, type,
                              shader_attributes_table[SID_ATTR_ROTAT2_IDX].codename,
                              shader_attributes_table[SID_ATTR_SCALE2_IDX].codename,
                              shader_attributes_table[SID_ATTR_VPOSI2_IDX].codename,
                              shader_attributes_table[SID_ATTR_ROTAT2_IDX].codename,
                              shader_attributes_table[SID_ATTR_SCALE2_IDX].codename,
                              shader_attributes_table[SID_ATTR_VPOSI2_IDX].codename,
                              shader_attributes_table[SID_ATTR_ROTAT2_IDX].codename,
                              shader_attributes_table[SID_ATTR_SCALE2_IDX].codename,
                              shader_attributes_table[SID_ATTR_VPOSI2_IDX].codename,
                              shader_attributes_table[SID_ATTR_ROTAT2_IDX].codename,
                              shader_attributes_table[SID_ATTR_SCALE2_IDX].codename,
                              shader_attributes_table[SID_ATTR_VPOSI2_IDX].codename

                );
        string_append_cstr(&str, s);
    }



    /***********************************************************************************************
     * GL_POSITION
     * // 2D
     * final_pos = vec4(transform.x + wpos_x, transform.y + wposy, 0.0, 0.0);
     * final_pos = vec4(transform.x + wpos_x, transform.y + wposy, wpos_z, 0.0);
     * // 3D
     * final_pos = vec4(transform.x + wpos_x, transform.y + wposy, transform.z + wpos_z, 0.0);
     */
    uint8 gl_pos_case_1 = 1;
    uint8 gl_pos_case_2 = 2;
    uint8 gl_pos_case = 0;

    if(sid_attr & shader_attributes_table[SID_ATTR_WPOSI2_IDX].flag) {
        transform_case = transform_case_1;
    }
    else if(sid_attr & shader_attributes_table[SID_ATTR_WPOSI3_IDX].flag) {
        transform_case = transform_case_2;
    }

    if(gl_pos_case == gl_pos_case_1) {
        string_append_cstr(&str, "  vec4 final_pos = vec4(transform.x + wpos_x, transform.y + wposy, 0.0, 1.0);\n");
    }
    else if(gl_pos_case == gl_pos_case_2) {
        string_append_cstr(&str, "  vec4 final_pos = vec4(transform.x + wpos_x, transform.y + wposy, wpos_z, 1.0);\n");
    }
    // else if(gl_pos_case == gl_pos_case_3) {
    //     string_append_cstr(&str, "  vec4 final_pos = vec4(transform.x + wpos_x, transform.y + wposy, transform.z + wpos_z, 0.0);\n");
    // }
    else {
        string_append_cstr(&str, "  vec4 final_pos = vec4(transform.x, transform.y, 0.0, 1.0);\n");
    }


    if( (sid_attr & shader_attributes_table[SID_ATTR_PROJMAT4X4_IDX].flag) ||
        (sid_unif & shader_uniforms_table[SID_UNIF_PROJMAT4X4_IDX].flag)) {
        char buf[64] = {0};
        const char* s = string_format(&buf[0], "  gl_Position = %s * final_pos;\n",
                                      shader_attributes_table[SID_UNIF_PROJMAT4X4_IDX].codename);
        string_append_cstr(&str, s);
    } else {
        string_append_cstr(&str, "  gl_Position = final_pos;\n");
    }




    /***********************************************************************************************
     * OUTPUTS ASSIGNMENT */
    string_append_cstr(&str, "\n  /// outputs assignment\n");
    for(uint32 i = 0; i < SID_ATTRIBS_IDX_COUNT; i++) {
        const char* type =
                shaderinfo_type_to_glsltype_string(shader_attributes_table[i].type);
        const char* name = shader_attributes_table[i].codename;

        // for(uint32 i = 0; i < attr_desc_cnt; i++) {
        //
        //     uint32 num_elems = attributes_desc[i].size;
        //     if(num_elems <= MAX_NUM_ELEMS_PER_LOCATION) {
        //
        //         uint32 j = 0;
        //         for(; j < num_elems; j++) {
        //             char buf[128] = {0};
        //             const char* s =
        //                 string_format(&buf[0],
        //                     "  float %s_%c = generic_%u.%c;\n",
        //                     shader_attributes_table[attributes_desc[i].idx].codename,
        //                     vec4[j], attributes_desc[i].loc, vec4[gen_loc_cnt++]
        //                     );
        //             if(gen_loc_cnt >= MAX_NUM_ELEMS_PER_LOCATION) gen_loc_cnt = 0;
        //             string_append_cstr(&str, s);
        //         }
        //     }
        // }
        if( (sid_pass & shader_attributes_table[i].flag) &&
            (sid_attr & shader_attributes_table[i].flag)) {
            char buf[64] = {0};
            // TODO: do for num elems(x,y,z,w) of the type
            // const char* s = string_format(&buf[0], "  out_%s = %s(%s_x, %s_y, %s_z, %s_w);\n",
            const char* s = string_format(&buf[0], "  out_%s = %s(%s_x, %s_y, %s_z);\n",
                                          name, type, name, name, name, name);
            string_append_cstr(&str, s);
        }
    }


    string_append_cstr(&str, "}\n");

    print("%s\n", str->data);



    desc.vshader_attribs_idx = shader_attributes_desc_set(attributes_desc, SID_ATTRIBS_IDX_COUNT);
    /// TODO: IMPORTANT! copy shader code to a new string_t with the minimum required size(power of 2, if slaballocator is used)
    desc.vshader_code = str->data;

    return desc;
}

void
shader_builder_destroy()
{
    string_destroy_table();
}


const char*
shaderinfo_type_to_glsltype_string(const enum TYPES_INDEXING type)
{
    switch(type) {
        case FLOAT:   return "float";
        case INT64:   return "INT64";
        case UINT64:  return "UINT64";
        case UINT32:  return "int";
        case INT32:   return "uint";
        case UINT16:  return "uint16";
        case INT16:   return "int16";
        case INT8:    return "int8";
        case UINT8:   return "uint8";
        case VEC2F:   return "vec2";
        case VEC3F:   return "vec3";
        case VEC4F:   return "vec4";
        case MAT2X2F: return "mat2";
        case MAT3X3F: return "mat3";
        case MAT4X4F: return "mat4";
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

/* DEBUG-PRINT */
void
shader_attribs_desc_table_print()
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
shader_uniforms_desc_table_print()
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



/* UNUSED-CODE */