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

#define SHADER_MAX_VEC_SIZE 4


/***************************************************************************************************
 * shader input variables
 **************************************************************************************************/
#define SHADER_ATTRIBUTES_ARRAY_COUNT 64
struct SHADER_VARIABLES_STRUCT shader_vars_table[SHADER_ATTRIBUTES_ARRAY_COUNT] = {
    {.flagname = TO_STRING(SID_OPACIT), .flag = SID_OPACIT, .typesize = 4, .elems = 1, .var_table_idx = SID_OPACIT_IDI, .type = FLOAT, .codename = "opacity",},
    {.flagname = TO_STRING(SID_VPOSI2), .flag = SID_VPOSI2, .typesize = 4, .elems = 2, .var_table_idx = SID_VPOSI2_IDI, .type = VEC2F, .codename = "vpos2",},
    {.flagname = TO_STRING(SID_WPOSI2), .flag = SID_WPOSI2, .typesize = 4, .elems = 2, .var_table_idx = SID_WPOSI2_IDI, .type = VEC2F, .codename = "wpos2",},
    {.flagname = TO_STRING(SID_SCALE2), .flag = SID_SCALE2, .typesize = 4, .elems = 2, .var_table_idx = SID_SCALE2_IDI, .type = VEC2F, .codename = "scale2",},
    {.flagname = TO_STRING(SID_ROTAT2), .flag = SID_ROTAT2, .typesize = 4, .elems = 2, .var_table_idx = SID_ROTAT2_IDI, .type = VEC2F, .codename = "rot2",},
    {.flagname = TO_STRING(SID_PIVOT2), .flag = SID_PIVOT2, .typesize = 4, .elems = 2, .var_table_idx = SID_PIVOT2_IDI, .type = VEC2F, .codename = "pivot2",},
    {.flagname = TO_STRING(SID_UVCOO2), .flag = SID_UVCOO2, .typesize = 4, .elems = 2, .var_table_idx = SID_UVCOO2_IDI, .type = VEC2F, .codename = "uv2",},
    {.flagname = TO_STRING(SID_VPOSI3), .flag = SID_VPOSI3, .typesize = 4, .elems = 3, .var_table_idx = SID_VPOSI3_IDI, .type = VEC3F, .codename = "vpos3",},
    {.flagname = TO_STRING(SID_WPOSI3), .flag = SID_WPOSI3, .typesize = 4, .elems = 3, .var_table_idx = SID_WPOSI3_IDI, .type = VEC3F, .codename = "wpos3",},
    {.flagname = TO_STRING(SID_SCALE3), .flag = SID_SCALE3, .typesize = 4, .elems = 3, .var_table_idx = SID_SCALE3_IDI, .type = VEC3F, .codename = "scale3",},
    {.flagname = TO_STRING(SID_ROTAT3), .flag = SID_ROTAT3, .typesize = 4, .elems = 3, .var_table_idx = SID_ROTAT3_IDI, .type = VEC3F, .codename = "rot3",},
    {.flagname = TO_STRING(SID_PIVOT3), .flag = SID_PIVOT3, .typesize = 4, .elems = 3, .var_table_idx = SID_PIVOT3_IDI, .type = VEC3F, .codename = "pivot3",},
    {.flagname = TO_STRING(SID_COLOR3), .flag = SID_COLOR3, .typesize = 4, .elems = 3, .var_table_idx = SID_COLOR3_IDI, .type = VEC3F, .codename = "color3",},
    {.flagname = TO_STRING(SID_NORMA3), .flag = SID_NORMA3, .typesize = 4, .elems = 3, .var_table_idx = SID_NORMA3_IDI, .type = VEC3F, .codename = "normal3",},
    {.flagname = TO_STRING(SID_COLOR4), .flag = SID_COLOR4, .typesize = 4, .elems = 4, .var_table_idx = SID_COLOR4_IDI, .type = VEC4F, .codename = "color4",},
    {.flagname = TO_STRING(SID_PROJMAT4X4), .flag = SID_PROJMAT4X4, .typesize = 4, .elems = 4 * 4, .var_table_idx = SID_PROJMAT4X4_IDI, .type = MAT4X4F, .codename = "projection_matrix",},
};
uint32 shader_vars_table_count = SID_IDI_COUNT;


sid_t
shader_create_flag_id(const sid_t idx)
{
    assert(idx < 64);
    sid_t sid = BIT_MASK_FROM_UINT(idx);
    return sid;
}

sid_t
shader_var_get_flag(const enum SID_VAR_IDI var_idi)
{
    if(var_idi >= shader_vars_table_count) return SID_NULL;
    return shader_vars_table[var_idi].flag;
}

struct SHADER_VARIABLES_STRUCT*
shader_var_get_var(const enum SID_VAR_IDI var_idi)
{
    if(var_idi >= shader_vars_table_count) return NULL;
    return &shader_vars_table[var_idi];
}

struct SHADER_VARIABLES_STRUCT*
shader_create_new_var(const char* name, const enum TYPES_INDEXING type)
{
    const sid_t idx = shader_vars_table_count;
    shader_vars_table_count++;
    const sid_t flag = shader_create_flag_id(idx);

    struct SHADER_VARIABLES_STRUCT* var = &shader_vars_table[idx];

    var->flagname = name;
    var->codename = name;
    var->flag = flag;
    var->type = type;
    var->typesize = shaderinfo_type_to_typesize(type);
    var->elems = shaderinfo_type_to_numelems(type);
    var->var_table_idx = idx;

    return var;
}


struct shader_build shader_builds_table[8];
build_t shader_builds_table_count = 0;

struct shader_build
shader_builds_get_build(build_t idx) { return shader_builds_table[idx]; }

build_t
shader_builder_create_new_build(const struct gfx_sid sid)
{
    const build_t idx = shader_builds_table_count;
    sid_copy(&shader_builds_table[idx].sid, sid);
    shader_builds_table_count++;
    return idx;
}

build_t
build_exists(const struct gfx_sid sid)
{
    for(build_t i = 0; i < shader_builds_table_count; i++)
        if(sid_check_match(sid, shader_builds_table[i].sid)) { return i; }
    return INT_NULL;
}

void
shader_builder_push_entry(const build_t build_idx, const struct SHADER_VARIABLES_STRUCT* entry, const enum SHADER_INPUT_FLAGS which)
{
    const uint32 entry_idx = shader_builds_table[build_idx].entry_count;
    shader_builds_table[build_idx].entry[entry_idx].flagname = entry->flagname;
    shader_builds_table[build_idx].entry[entry_idx].codename = entry->codename;
    shader_builds_table[build_idx].entry[entry_idx].elems = entry->elems;
    shader_builds_table[build_idx].entry[entry_idx].flag = entry->flag;
    shader_builds_table[build_idx].entry[entry_idx].type = entry->type;
    shader_builds_table[build_idx].entry[entry_idx].typesize = entry->typesize;
    shader_builds_table[build_idx].entry[entry_idx].which_renameme = which;
    shader_builds_table[build_idx].entry[entry_idx].var_table_idx = entry->var_table_idx;
    shader_builds_table[build_idx].entry_count++; /// increment current build's entry count
    assert(shader_builds_table[build_idx].entry_count < SHADER_INPUT_ENTRIES_MAX_COUNT);
}

void
temp_print_builds(void)
{
    for(build_t i = 0; i < shader_builds_table_count; i++) {
        print("build:%u\n", i);
        for(uint32 j = 0; j < shader_builds_table[i].entry_count; j++) {
            print(" entry:%u\n", j);
            print("  %s %s typesize:%u elems:%u which:%llu\n"
                  "",
                  shader_builds_table[i].entry[j].codename,
                  shaderinfo_type_to_glsltype_string(shader_builds_table[i].entry[j].type),
                  shader_builds_table[i].entry[j].typesize,
                  shader_builds_table[i].entry[j].elems,
                  shader_builds_table[i].entry[j].which_renameme
            );
        }
    }
}


build_t
shader_create_input(const struct gfx_sid sid, const char* name, const enum TYPES_INDEXING type, enum SID_VAR_IDI var_idi, const enum SHADER_INPUT_FLAGS which)
{
    /// 1. find or create shader build
    build_t build_idx = build_exists(sid);
    if(build_idx == INT_NULL) build_idx = shader_builder_create_new_build(sid);

    /// check if variable input exists, via "enum SID_VAR_IDI" index
    struct SHADER_VARIABLES_STRUCT* var = shader_var_get_var(var_idi);
    /// if not, create new variable input
    if(!var) var = shader_create_new_var(name, type);

    /// 2. insert build entry
    shader_builder_push_entry(build_idx, var, which);

    return build_idx;
}


/***************************************************************************************************
 * shader generic location
 **************************************************************************************************/
struct vertex_layout
create_location(const uint8 loc, const uint8 size, const uint8 type, const uint8 offset)
{
    return (struct vertex_layout){.loc = loc, .size = size, .type = type, .offset = offset};
}

void
code_create_vector4(string_t** str, struct SHADER_VARIABLES_STRUCT vec, const char* vecname, const char* defval)
{
    // const char vec4[4] = {'x', 'y', 'z', 'w'};
    char buf[128] = {0};
    char* ptr = &buf[0];
    string_format(&ptr, 128, "  vec4 %s = vec4(", vecname);
    string_append_cstr(str, buf);

    for(uint32 dimention = 0; dimention < SHADER_MAX_VEC_SIZE; dimention++) {
        if(dimention < vec.elems) {
            char buf2[32] = {0};
            char* ptr2 = &buf2[0];
            // string_format(&ptr2, 32, "%s_%c, ", vec.codename, vec4[dimention]);
            string_format(&ptr2, 32, "%s_%u, ", vec.codename, dimention);
            string_append_cstr(str, buf2);
        } else string_append_cstr(str, defval);
    }

    (*str)->len -= 2; /// REMOVE the last space and ','
    string_append_cstr(str, ");\n");
}

void
code_create_vector(string_t** str, const char* codename, const char* vname, const enum TYPES_INDEXING vtype, const uint32 velems)
{
    // const char vec4[4] = {'x', 'y', 'z', 'w'};
    const char* type = shaderinfo_type_to_glsltype_string(vtype);
    char buf[128] = {0};
    char* ptr = &buf[0];
    string_format(&ptr, 128, "  %s %s = %s(", type, vname, type);
    string_append_cstr(str, buf);

    for(uint32 dimention = 0; dimention < velems; dimention++) {
        char buf2[32] = {0};
        char* ptr2 = &buf2[0];
        // string_format(&ptr2,  32, "%s_%c, ", codename, vec4[dimention]);
        string_format(&ptr2, 32, "%s_%c, ", codename, dimention);
        string_append_cstr(str, buf2);
    }

    (*str)->len -= 2; /// REMOVE the last space and ','
    string_append_cstr(str, ");\n");
}

void
code_create_matrix(string_t** str, const char* codename, const char* vname, const enum TYPES_INDEXING vtype, const uint32 velems)
{
    /*
     * uniform vec4 u_rotation; // (cosθ, sinθ, cosθ, sinθ) or (cosθ, sinθ, 0, 0)
     *  // In shader:
     *  vec2 rotated_xy = vec2(
     *      final_pos.x * u_rotation.x - final_pos.y * u_rotation.y,
     *      final_pos.x * u_rotation.y + final_pos.y * u_rotation.x
     *  );
     *
     *  TODO: implement 3D rotations
     * 3D rotations
     * axis: X              Y                   Z
     * _________________   _________________   _________________
     * | 1 | 0   | 0   |   | cos | 0 | sin |   | cos |-sin | 0 |
     * | 0 | cos |-sin |   | 0   | 1 | 0   |   | sin | cos | 0 |
     * | 0 | sin | cos |   |-sin | 0 | cos |   | 0   | 0   | 1 |
     * |________________   |________________   |________________
     */
    char rot_dim = (vtype == VEC2F) ? '2' : '3';
    assert(vtype == VEC2F);

    char buf[128] = {0};
    char* ptr = &buf[0];
    string_format(&ptr, 128,
                  "  mat%c %s = mat%c(%s_x, %s_y, -%s_y, %s_x);\n",
                  rot_dim, vname, rot_dim, codename, codename, codename, codename);
    string_append_cstr(str, buf);
}

void
code_operate_vector(string_t** str, struct SHADER_VARIABLES_STRUCT oper1, struct SHADER_VARIABLES_STRUCT oper2, const char* operator, const float defval1, const float defval2)
{
    const char vec4[4] = {'x', 'y', 'z', 'w'};
    char buf[256] = {0};
    char* ptr = &buf[0];
    const uint32 buffer_len = 128;
    uint32 len = string_format(&ptr, buffer_len, "       %s = vec4(", oper1.codename);

    for(uint32 dimention = 0; dimention < SHADER_MAX_VEC_SIZE; dimention++) {
        if(dimention < oper1.elems) {
            ptr = &buf[len];
            // len += string_format(&ptr, 128, "%s.%c", oper1.codename, vec4[dimention],  '+', oper1.codename, vec4[dimention]);
            len += string_format(&ptr, buffer_len, "%s.%c", oper1.codename, vec4[dimention], '+', oper1.codename, dimention);
        } else {
            ptr = &buf[len];
            // len += string_format(&ptr, 128, "0.0");
            len += string_format(&ptr, buffer_len, "%.1f", defval1);
        }

        ptr = &buf[len];
        len += string_format(&ptr, buffer_len, "%s", operator);

        if(dimention < oper2.elems) {
            ptr = &buf[len];
            len += string_format(&ptr, buffer_len, "%s_%u, ", oper2.codename, dimention);
        } else {
            ptr = &buf[len];
            // len += string_format(&ptr, 128, "0.0, ");
            len += string_format(&ptr, buffer_len, "%.1f, ", defval2);
        }
    }

    string_append_cstr(str, buf);
    (*str)->len -= 2; /// REMOVE the last space and ','
    string_append_cstr(str, ");\n");
}

void
code_operate_mat2x2(string_t** str, struct SHADER_VARIABLES_STRUCT oper1, struct SHADER_VARIABLES_STRUCT oper2, const char* operator)
{
    // const char vec4[4] = {'x', 'y', 'z', 'w'};
    const uint32 buf_len = 512;
    char buf[512] = {0};
    char* ptr = &buf[0];
    uint32 len = string_format(&ptr, buf_len, "       %s = vec4(", oper1.codename);

    // Handle dimensions 0 and 1 (x and y) with rotation
    for(uint32 dim = 0; dim < 2; dim++) {
        // ptr = &buf[len];
        // len += string_buffer_push(&ptr, buf_len, "\n                   ");

        if(dim == 0) { // x component
            ptr = &buf[len];
            len += string_format(&ptr, buf_len, "(%s.%c * %s_%u) + (%s.%c * -%s_%u)",
                                 oper1.codename, 'x', // vpos.x
                                 oper2.codename, 0, // * rot_x
                                 oper1.codename, 'y', // vpos.y
                                 oper2.codename, 1); // * -rot_y
        } else { // y component
            ptr = &buf[len];
            len += string_format(&ptr, buf_len, "(%s.%c * %s_%u) + (%s.%c * %s_%u)",
                                 oper1.codename, 'x', // vpos.y
                                 oper2.codename, 1, // * rot_y
                                 oper1.codename, 'y', // vpos.x
                                 oper2.codename, 0); // * rot_x
        }

        ptr = &buf[len];
        len += string_buffer_push(&ptr, buf_len, ", ");
    }

    // Handle dimensions 2 and 3 (z and w) - always 0.0
    for(uint32 dim = 2; dim < 4; dim++) {
        // ptr = &buf[len];
        // len += string_buffer_push(&ptr, buf_len, "\n                   ");

        ptr = &buf[len];
        len += string_buffer_push(&ptr, 128, "0.0, ");
    }

    string_append_cstr(str, buf);
    (*str)->len -= 2; /// REMOVE the last space and coma ', '
    // Close the vec4 constructor
    string_append_cstr(str, ");\n");
}


/***************************************************************************************************
 * shader builder
 **************************************************************************************************/
void
shader_builder_init(void)
{
    /// initialize 4 pages(4096 bytes) for shader code
    string_init_table(4);

    for(uint32 i = 0; i < 1; i++) {
        // shader_builds_table[i].entry = ;
        // shader_builds_table[i].entry_count = 0;
        // shader_builds_table[i].sid = (struct gfx_sid){0x0, 0x0, 0x0, 0x0, 0x0};
        // for(uint32 j = 0; j < 1; j++) {
        //     shader_builds_table[i].entry[j].
        // }
    }
}

#define SHADERBUILDERGLSL_VERSION "#version 330 core\n\n"
#define SHADERBUILDERGLSL_VS_MAIN "void main()\n"
#define SHADERBUILDERGLSL_VS_GL_POSITION "  gl_Position\n"
#define MAX_NUM_ELEMS_PER_LOCATION 4
#define GENERIC_ATTR_LOC "GENERIC_ATTR_LOC"

struct shader_descriptor
shader_builder_build(const struct gfx_sid sid)
{
    // TODO: descriptor is not builders responsibility, rather gfx's, so maybe program should pass a desc as param
    struct shader_descriptor desc = {
        .vshader_attribs_idx = INT_NULL,
        .vshader_attribs_count = 0,
        .vshader_attrib_locations_count = 0,
        .vshader_code = NULL,
        .vshader_code_size = 0,
        .progidx = INT_NULL,
        .vshader_attribs_stride = 0,
        .last_loc_num_elems = 0
    };

    string_t* str = string_create_empty(2048);

    string_append_cstr(&str, SHADERBUILDERGLSL_VERSION);

    // attributes: in
    string_append_cstr(&str, "/// attributes: in\n");

    uint8 loc_offset = 0; /// the start offset of each attribute
    uint16 total_offset = 0; /// the total elements count. Used to find the last location's enabled number of elements
    uint8 loc_cnt = 0; /// locations index
    int8 prev_loc_idx = -1;

    // shader_builds_table_init();
    const uint32 build_idx = 0; // TODO: get the next free build table index
    struct shader_build build = shader_builds_table[build_idx];
    // vertex_attribute_ids


    /***********************************************************************************************
     * shader input variables */
    // storage of current shader's attributes info (location, offset, typesize, ...)
    struct vertex_layout vertex_layout[SID_IDI_COUNT] = {0};
    uint32 attr_desc_cnt = 0; /// attributes_desc counter

    /***********************************************************************************************
    * attributes: in
    * algorithm 1: create generic locations */
    uint8 num_elems = 0;
    uint8 type = 0;
    const char* name = NULL;
    uint32 in_var_idx = 0;
    for(; in_var_idx < build.entry_count; in_var_idx++) {
        struct SHADER_VARIABLES_STRUCT entry = build.entry[in_var_idx];

        if(build.entry[in_var_idx].which_renameme & IS_ATTR) { // only if input variable is a vertex attribute

            num_elems = entry.elems;
            type = entry.type;
            name = entry.codename;

            /// Record the start position offset for this attribute
            struct vertex_layout loc = (struct vertex_layout){.loc = loc_cnt, .size = num_elems, .type = type, .offset = loc_offset, .idx = in_var_idx};
            vertex_layout[attr_desc_cnt] = loc; // storage by attributes, not by locations
            print("loc:%u elems:%u offset:%u {%s}\n", loc.loc, loc.size, loc.offset, name);

            if(prev_loc_idx != loc_cnt) {
                char buf[64] = {0};
                char* ptr = &buf[0];
                string_format(&ptr, 64, "layout (location = %u) in vec4 generic_%u;\n", loc.loc, loc.loc);
                string_append_cstr(&str, buf);
                prev_loc_idx = loc_cnt;
                desc.vshader_attrib_locations_count++;
            }

            // Advance the offset by the size of this attribute
            loc_offset += num_elems;
            total_offset += num_elems;

            /*
             * if {current location offset} + {current processed attribute size} extends the max 4 elems per location
             * 1. create new location (loc_cnt++)
             * 2. reduce {current location offset} by 4 elems
             * 3. if attribute size is > 4 elems(matrix),
             *    we continue the while loop until we create all new locations that fit the attribute size.
             */
            while(loc_offset >= SHADER_MAX_VEC_SIZE) {
                loc_offset -= SHADER_MAX_VEC_SIZE;
                loc_cnt++;
                if(loc_offset >= SHADER_MAX_VEC_SIZE) {
                    loc = (struct vertex_layout){.loc = loc_cnt, .size = num_elems, .type = type, .offset = 0, .idx = in_var_idx};
                    print("+loc:%u elems:%u offset:%u {%s}\n", loc.loc, loc.size, loc.offset, name);
                    if(prev_loc_idx != loc_cnt) {
                        char buf[64] = {0};
                        char* ptr = &buf[0];
                        string_format(&ptr, 64, "layout (location = %u) in vec4 generic_%u;\n", loc.loc, loc.loc);
                        string_append_cstr(&str, buf);
                        prev_loc_idx = loc_cnt;
                        desc.vshader_attrib_locations_count++;
                    }
                }
            }

            /// store shader varible index to vertex_layout struct. TODO: ?? is that useful ??
            // vertex_layout[attr_desc_cnt].idx = build.entry[in_var_idx].var_table_idx;

            attr_desc_cnt++; /// increment vertex_layout counter
        }
    }
    // HERE: 1. last_loc_num_elems is 0 if loc 2 is vec4 color, it should be 4
    // 2. last location should not be created if the last attribute fits exactly;
    desc.last_loc_num_elems = total_offset % SHADER_MAX_VEC_SIZE;
    desc.vshader_attribs_count = attr_desc_cnt;
    if(prev_loc_idx != loc_cnt && desc.last_loc_num_elems != 0) {
        struct vertex_layout loc = (struct vertex_layout){.loc = loc_cnt, .size = num_elems, .type = type, .offset = 0, .idx = in_var_idx};
        print("  {%s} expands to:  loc:%u offset:%u \n", name, loc.loc, loc.offset);
        char buf[64] = {0};
        char* ptr = &buf[0];
        string_format(&ptr, 64, "layout (location = %u) in vec4 generic_%u;\n", loc.loc, loc.loc);
        string_append_cstr(&str, buf);
        desc.vshader_attrib_locations_count++;
    } else if (desc.last_loc_num_elems == 0){desc.last_loc_num_elems = SHADER_MAX_VEC_SIZE;}

    string_append_cstr(&str, "\n");

    /***********************************************************************************************
     * attributes: out */
    string_append_cstr(&str, "/// attributes: out\n");
    for(uint32 i = 0; i < build.entry_count; i++) {
        if(build.entry[i].which_renameme & IS_OUT) {
            const char* format = "out %s out_%s;\n";
            char buf[64] = {0};
            char* ptr = &buf[0];
            string_format(&ptr, 64, format,
                          shaderinfo_type_to_glsltype_string(build.entry[i].type),
                          build.entry[i].codename);
            string_append_cstr(&str, buf);
        }
    }
    string_append_cstr(&str, "\n");

    /***********************************************************************************************
     * uniforms: in */
    string_append_cstr(&str, "/// uniforms: in\n");
    for(uint32 i = 0; i < build.entry_count; i++) {
        // create_uniform_in(sid_unif, &str, "uniform %s %s;\n", i);
        if(build.entry[i].which_renameme & IS_UNIF) {
            const char* format = "uniform %s %s;\n";
            char buf[64] = {0};
            char* ptr = &buf[0];
            string_format(&ptr, 64, format,
                          shaderinfo_type_to_glsltype_string(build.entry[i].type),
                          build.entry[i].codename);
            string_append_cstr(&str, buf);
        }
    }
    string_append_cstr(&str, "\n");

    /***********************************************************************************************
     * main() */
    string_append_cstr(&str, SHADERBUILDERGLSL_VS_MAIN);
    string_append_cstr(&str, "{\n");

    /***********************************************************************************************
     * attributes: reassemble generic attributes to variables */
    string_append_cstr(&str, "\n  /// reassemble generic attributes\n");
    const char vec4[4] = {'x', 'y', 'z', 'w'};
    uint32 gen_loc_cnt = 0;
    uint8 dimention = 0;
    uint8 k = 0;
    for(uint32 i = 0; i < attr_desc_cnt; i++) {
        uint32 num_elems = vertex_layout[i].size;
        uint32 j = 0;
        for(; j < num_elems; j++) {
            char buf[128] = {0};
            char* ptr = &buf[0];
            string_format(&ptr, 128,
                          "  float %s_%u = generic_%u.%c;\n",
                          build.entry[vertex_layout[i].idx].codename,
                          dimention++, k, vec4[gen_loc_cnt++]
            );
            if(gen_loc_cnt >= MAX_NUM_ELEMS_PER_LOCATION) {
                gen_loc_cnt = 0;
                k++;
            }
            string_append_cstr(&str, buf);
        }
        desc.vshader_attribs_stride += vertex_layout[i].size;
        dimention = 0;
    }
    string_append_cstr(&str, "\n");


    /***********************************************************************************************
     * vertex transform */
    string_append_cstr(&str, "\n  /// calculate transform\n");


    /*
     * vec4 vpos = vec4(vpos_x, vpos_y, 0.0, 0.0);
     *      vpos = vec4((rot_x * vpos.x)+(-rot_y * vpos.y), (rot_y * vpos.x)+(rot_x * vpos.y), 0.0, 0.0);
     *      vpos = vec4(vpos.x * scale_x, vpos.y * scale_y, 0.0, 0.0);
     *      vpos = vec4(vpos.x + wpos_x,  vpos.y + wpos_y,  0.0, 0.0);
     * gl_position = vpos;
     */
    struct SHADER_VARIABLES_STRUCT oper1 = {.codename = "vpos", .elems = build.entry[0].elems};
    code_create_vector4(&str, build.entry[0], "vpos", "1.0, ");
    // string_append_cstr(&str, "  vec4 vpos = vec4(vpos2_0, vpos2_1, 0.0, 1.0);\n");

    for(uint32 i = 0; i < build.entry_count; i++) {
        if(build.entry[i].flag & SID_ROTAT2) {
            code_operate_mat2x2(&str, oper1, build.entry[i], "*");
        }
    }
    // string_append_cstr(&str, "       vpos = vec4((vpos.x * rot2_0) + (vpos.y * (-rot2_1)), (vpos.x * rot2_1) + (vpos.y * rot2_0), 0.0, 0.0);\n");


    for(uint32 i = 0; i < build.entry_count; i++) {
        if(build.entry[i].flag & SID_SCALE2) {
            code_operate_vector(&str, oper1, build.entry[i], "*", 1.0, 1.0);
        }
    }
    // string_append_cstr(&str, "       vpos = vec4(vpos.x*scale2_0, vpos.y*scale2_1, 0.0, 1.0);\n");


    for(uint32 i = 0; i < build.entry_count; i++) {
        if(build.entry[i].flag & SID_WPOSI3) {
            code_operate_vector(&str, oper1, build.entry[i], "+", 1.0, 0.0);
        }
    }
    // string_append_cstr(&str, "       vpos = vec4(vpos.x*1.0, vpos.y*1.0, 0.0, 1.0);\n");
    // string_append_cstr(&str, "       vpos = vec4(vpos.x+wpos3_0, vpos.y+wpos3_1, wpos3_2, 1.0);\n");



    string_append_cstr(&str, "  gl_Position = vpos;\n");


    /***********************************************************************************************
     * outputs: assignment */
    string_append_cstr(&str, "\n  /// outputs assignment\n");
    for(uint32 i = 0; i < build.entry_count; i++) {
        if(build.entry[i].which_renameme & IS_OUT) {
            const char* type = shaderinfo_type_to_glsltype_string(build.entry[i].type);
            const char* name = build.entry[i].codename;
            char buf[64] = {0};
            char* ptr = &buf[0];
            string_format(&ptr, 64, "  out_%s = %s(",
                          name, type, name, name, name, name);
            string_append_cstr(&str, buf);


            for(uint32 dimention = 0; dimention < SHADER_MAX_VEC_SIZE; dimention++) {
                char buf2[32] = {0};
                char* ptr2 = &buf2[0];
                if(dimention < build.entry[i].elems) {
                    string_format(&ptr2, 32, "%s_%u, ", build.entry[i].codename, dimention);
                    string_append_cstr(&str, buf2);
                } else string_append_cstr(&str, "0.0, ");
            }
            str->len -= 2; /// REMOVE the last space and ','
            string_append_cstr(&str, ");\n");
        }
    }


    string_append_cstr(&str, "}\n");
    print("%s\n", str->data);


    desc.vshader_attribs_idx = shader_attributes_desc_set(vertex_layout, desc.vshader_attribs_count);
    /// TODO: IMPORTANT! copy shader code to a new string_t with the minimum required size(power of 2 if slaballocator is used)
    desc.vshader_code = str->data;
    desc.vshader_code_size = str->len;

    return desc;
}

void
shader_builder_destroy(void)
{
    string_destroy_table();
}

const char*
shaderinfo_rename_me(const enum SID_VAR_IDI type)
{
    switch(type) {
        // case SID_NULL_:   return "";
        case SID_OPACIT_IDI: return "OPACIT";
        case SID_VPOSI2_IDI: return "VPOSI2";
        case SID_WPOSI2_IDI: return "WPOSI2";
        case SID_SCALE2_IDI: return "SCALE2";
        case SID_ROTAT2_IDI: return "ROTAT2";
        case SID_PIVOT2_IDI: return "PIVOT2";
        case SID_UVCOO2_IDI: return "UVCOO2";
        case SID_VPOSI3_IDI: return "VPOSI3";
        case SID_WPOSI3_IDI: return "WPOSI3";
        case SID_SCALE3_IDI: return "SCALE3";
        case SID_ROTAT3_IDI: return "ROTAT3";
        case SID_PIVOT3_IDI: return "PIVOT3";
        case SID_COLOR3_IDI: return "COLOR3";
        case SID_NORMA3_IDI: return "NORMA3";
        case SID_COLOR4_IDI: return "NORMA3";
        case SID_PROJMAT4X4_IDI: return "PROJMAT4X4";
        default: return "NONE";
    }
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

uint32
shaderinfo_type_to_numelems(const enum TYPES_INDEXING type)
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
        case MAT2X2F: return 2 * 2;
        case MAT3X3F: return 3 * 3;
        case MAT4X4F: return 4 * 4;
        default: assert(1);
    }
}

uint32
shaderinfo_type_to_typesize(const enum TYPES_INDEXING type)
{
    switch(type) {
        case FLOAT:   return SIZE_F32;
        case INT64:   return SIZE_I64;
        case UINT64:  return SIZE_I64;
        case UINT32:  return SIZE_I32;
        case UINT16:  return SIZE_I16;
        case INT8:    return SIZE_I8;
        case UINT8:   return SIZE_I8;
        case VEC2F:   return SIZE_F32;
        case VEC3F:   return SIZE_F32;
        case VEC4F:   return SIZE_F32;
        case MAT2X2F: return SIZE_F32;
        case MAT3X3F: return SIZE_F32;
        case MAT4X4F: return SIZE_F32;
        default: assert(1);
    }
    return 0;
}

/* DEBUG-PRINT */
void
shader_inputs_desc_table_print(void)
{
    print("-- %s ATTRIBUTES ARRAY %s --\n", YELLOW, CLEAR);
    for(uint32 i = 0; i < shader_vars_table_count; i++) {
        const struct SHADER_VARIABLES_STRUCT a = shader_vars_table[i];
        if(a.flag)
            print("name: %s %-8s %s "
                  "type idx: %s%3u%s "
                  "type name: %s%7s%s "
                  "attrib_flag: %s%8llu%s "
                  "type size: %s%u%s "
                  "type num elems: %s%2u%s "
                  "self idx: %s%2u%s\n",
                  ry, shader_vars_table[i].codename, CLEAR,
                  ry, shader_vars_table[i].type, CLEAR,
                  ry, shaderinfo_type_to_glsltype_string(shader_vars_table[i].type), CLEAR,
                  ry, shader_vars_table[i].flag, CLEAR,
                  ry, shader_vars_table[i].typesize, CLEAR,
                  ry, shader_vars_table[i].elems, CLEAR,
                  ry, shader_vars_table[i].var_table_idx, CLEAR);
    }
    print("\n");
}
