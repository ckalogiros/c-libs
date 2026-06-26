/*
 * Created by ckalo on 4/8/26.
 */

#include "shader_builder.h"
#include "my_macros.h"
#include "shader_descriptor.h"
#include <stdlib.h>
#include "file_sys_interface.h"
#include "logger.h"
// #include "sparse_arr.h"
#include "opengl_debug.h"
#include "stringc.h"
#include "string_format.h"


/***************************************************************************************************
 * static shader's input variables
 * a table to store each unique variable (name, flag, type etc.), the user creates, to be used as an
 * input in the vertex shader. Values, specific to a shader build, are stored
 * in "shader_descriptor_table", which is the storage for each shader program
 **************************************************************************************************/
#define SHADER_ATTRIBUTES_ARRAY_COUNT 64
struct shader_in_variables_struct shader_vars_table[SHADER_ATTRIBUTES_ARRAY_COUNT] = {
    {.flagname = TO_STRING(SID_OPACIT), .flag = SID_OPACIT, .typesize = SIZE_F32, .elems = 1, .var_table_idx = SID_OPACIT_IDI, .type = FLOAT, .codename = "opacity",},
    {.flagname = TO_STRING(SID_VPOSI2), .flag = SID_VPOSI2, .typesize = SIZE_F32, .elems = 2, .var_table_idx = SID_VPOSI2_IDI, .type = VEC2F, .codename = "vpos2",},
    {.flagname = TO_STRING(SID_WPOSI2), .flag = SID_WPOSI2, .typesize = SIZE_F32, .elems = 2, .var_table_idx = SID_WPOSI2_IDI, .type = VEC2F, .codename = "wpos2",},
    {.flagname = TO_STRING(SID_SCALE2), .flag = SID_SCALE2, .typesize = SIZE_F32, .elems = 2, .var_table_idx = SID_SCALE2_IDI, .type = VEC2F, .codename = "scale2",},
    {.flagname = TO_STRING(SID_ROTAT2), .flag = SID_ROTAT2, .typesize = SIZE_F32, .elems = 2, .var_table_idx = SID_ROTAT2_IDI, .type = VEC2F, .codename = "rot2",},
    {.flagname = TO_STRING(SID_PIVOT2), .flag = SID_PIVOT2, .typesize = SIZE_F32, .elems = 2, .var_table_idx = SID_PIVOT2_IDI, .type = VEC2F, .codename = "pivot2",},
    {.flagname = TO_STRING(SID_UVCOO2), .flag = SID_UVCOO2, .typesize = SIZE_F32, .elems = 2, .var_table_idx = SID_UVCOO2_IDI, .type = VEC2F, .codename = "uv2",},
    {.flagname = TO_STRING(SID_VPOSI3), .flag = SID_VPOSI3, .typesize = SIZE_F32, .elems = 3, .var_table_idx = SID_VPOSI3_IDI, .type = VEC3F, .codename = "vpos3",},
    {.flagname = TO_STRING(SID_WPOSI3), .flag = SID_WPOSI3, .typesize = SIZE_F32, .elems = 3, .var_table_idx = SID_WPOSI3_IDI, .type = VEC3F, .codename = "wpos3",},
    {.flagname = TO_STRING(SID_SCALE3), .flag = SID_SCALE3, .typesize = SIZE_F32, .elems = 3, .var_table_idx = SID_SCALE3_IDI, .type = VEC3F, .codename = "scale3",},
    {.flagname = TO_STRING(SID_ROTAT3), .flag = SID_ROTAT3, .typesize = SIZE_F32, .elems = 3, .var_table_idx = SID_ROTAT3_IDI, .type = VEC3F, .codename = "rot3",},
    {.flagname = TO_STRING(SID_PIVOT3), .flag = SID_PIVOT3, .typesize = SIZE_F32, .elems = 3, .var_table_idx = SID_PIVOT3_IDI, .type = VEC3F, .codename = "pivot3",},
    {.flagname = TO_STRING(SID_COLOR3), .flag = SID_COLOR3, .typesize = SIZE_F32, .elems = 3, .var_table_idx = SID_COLOR3_IDI, .type = VEC3F, .codename = "color3",},
    {.flagname = TO_STRING(SID_NORMA3), .flag = SID_NORMA3, .typesize = SIZE_F32, .elems = 3, .var_table_idx = SID_NORMA3_IDI, .type = VEC3F, .codename = "normal3",},
    {.flagname = TO_STRING(SID_COLOR4), .flag = SID_COLOR4, .typesize = SIZE_F32, .elems = 4, .var_table_idx = SID_COLOR4_IDI, .type = VEC4F, .codename = "color4",},
    {.flagname = TO_STRING(SID_PROJMAT4X4), .flag = SID_PROJMAT4X4, .typesize = SIZE_F32, .elems = 4 * 4, .var_table_idx = SID_PROJMAT4X4_IDI, .type = MAT4X4F, .codename = "projection_matrix",},
};
uint32 shader_vars_table_count = SID_IDI_COUNT;



sid_t
shader_create_flag_id(const sid_t idx)
{
    assert(idx < 64);
    sid_t sid_flag = BIT_MASK_FROM_UINT(idx);
    return sid_flag;
}

sid_t
shader_var_get_flag(const enum SID_ID shader_var_id)
{
    if(shader_var_id >= shader_vars_table_count) return SID_NULL;
    return shader_vars_table[shader_var_id].flag;
}

struct shader_in_variables_struct*
shader_var_get_var(const enum SID_ID shader_var_id)
{
    if(shader_var_id >= shader_vars_table_count) return NULL;
    return &shader_vars_table[shader_var_id];
}

struct shader_in_variables_struct*
shader_create_new_var(enum SID_ID shader_var_id, const char* name, const enum TYPES_INDEXING type)
{
    const uint32 idx = shader_vars_table_count;
    shader_vars_table_count++;
    const sid_t sid_flag = shader_create_flag_id(idx);

    struct shader_in_variables_struct* var = &shader_vars_table[idx];

    var->flagname = name;
    var->codename = name;
    var->flag = sid_flag;
    var->type = type;
    var->typesize = shaderinfo_type_to_typesize(type);
    var->elems = shaderinfo_type_to_numelems(type);
    var->var_table_idx = idx;
    var->shader_var_id = shader_var_id;

    return var;
}

struct shader_descriptor*
shader_builds_get_build(build_t idx)
{
    struct shader_descriptor* buf = shader_descriptor_get(idx);
    return buf;
}

build_t
shader_builder_create_new_build(const sididx_t sid_idx)
{
    const build_t idx = shader_descriptor_create(sid_idx);
    return idx;
}

build_t
shader_build_exists(const sididx_t sid_idx)
{
    const uint32 count = shader_descriptor_get_count();
    for(build_t i = 0; i < count; i++) {
        struct shader_descriptor* desc = shader_descriptor_get(i);
        if(sid_check_match(desc->sid_idx, sid_idx)) { return i; }
    }
    return INT_NULL;
}
build_t
shader_build_is_set(const sididx_t sid_idx)
{
    /* Shader build  maybe created, but shader_builder_build() not yet called(not set)
     * In this function we check for both */
    const uint32 count = shader_descriptor_get_count();
    for(build_t i = 0; i < count; i++) {
        struct shader_descriptor* desc = shader_descriptor_get(i);
        if(desc->is_set && sid_check_match(desc->sid_idx, sid_idx)) { return i; }
    }
    return INT_NULL;
}

bool
shader_build_entry_exists(const build_t build_idx, const sid_t new_entry_attrib_flag)
{
    struct shader_descriptor* desc = shader_descriptor_get(build_idx);
    if(desc->added_entry_flags_renameme & new_entry_attrib_flag) {
        return TRUE;
    }
    return FALSE;
}


void
shader_builder_push_entry(const build_t idx, const struct shader_in_variables_struct* entry, const enum SHADER_INPUT_FLAGS which)
{
    struct shader_descriptor* desc = shader_descriptor_get(idx);
    const uint32 entry_idx = desc->entry_count;
    desc->entries[entry_idx].flagname = entry->flagname;
    desc->entries[entry_idx].codename = entry->codename;
    desc->entries[entry_idx].elems = entry->elems;
    desc->entries[entry_idx].flag = entry->flag;
    desc->entries[entry_idx].type = entry->type;
    desc->entries[entry_idx].typesize = entry->typesize;
    desc->entries[entry_idx].which_renameme = which;
    desc->entries[entry_idx].var_table_idx = entry->var_table_idx;
    desc->entries[entry_idx].this_idx = entry_idx;
    desc->added_entry_flags_renameme |=  entry->flag; /// this way we know what attributes have been created for this shader build descriptor
    desc->entry_count++; /// increment current build's entry count
    print("%s- shader add entry:%s, count:%u -%s\n", RED, entry->codename, desc->entry_count, CLEAR);
    assert(desc->entry_count < SHADER_INPUT_ENTRIES_MAX_COUNT);
}

build_t
shader_create_input(const sididx_t sid_idx, const char* name, const enum TYPES_INDEXING type, enum SID_ID shader_var_id, const enum SHADER_INPUT_FLAGS which)
{
    /// 1. find or create shader build
    build_t build_idx = shader_build_exists(sid_idx);
    if(build_idx == INT_NULL) build_idx = shader_builder_create_new_build(sid_idx);

    /// check if variable input exists, via "enum SID_VAR_IDI" index.
    struct shader_in_variables_struct* var = shader_var_get_var(shader_var_id);
    /// if not, create new variable input
    if(!var) var = shader_create_new_var(shader_var_id, name, type);

    /// 2. insert build entry
    sid_t sid_flag = shaderinfo_id_to_flag(shader_var_id);
    if(var->var_table_idx >= shader_vars_table_count) /// case new entry has not been created
        shader_builder_push_entry(build_idx, var, which);



    else if(!shader_build_entry_exists(build_idx, sid_flag))
        shader_builder_push_entry(build_idx, var, which);


    return build_idx;
}


/***************************************************************************************************
 * shader generic locations
 **************************************************************************************************/
// struct vertex_layout
// create_location(const uint8 loc, const uint8 size, const uint8 type, const uint8 offset)
// {
//     return (struct vertex_layout){.loc = loc, .size = size, .type = type, .loc_offset = offset};
// }

void
code_create_vector4(string_t** str, struct shader_in_variables_struct vec, const char* vecname, const char* defval)
{
    char buf[128] = {0};
    char* ptr = &buf[0];
    string_format(&ptr, 128, "  vec4 %s = vec4(", vecname);
    string_append_cstr(str, buf);

    for(uint32 dimention = 0; dimention < SHADER_MAX_LOCATION_SIZE; dimention++) {
        if(dimention < vec.elems) {
            char buf2[32] = {0};
            char* ptr2 = &buf2[0];
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
code_create_matrix(string_t** str, const char* codename, const char* vname, const enum TYPES_INDEXING vtype)
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
    assert((vtype == VEC2F) || (vtype == VEC3F));

    char buf[128] = {0};
    char* ptr = &buf[0];
    string_format(&ptr, 128,
                  "  mat%c %s = mat%c(%s_x, %s_y, -%s_y, %s_x);\n",
                  rot_dim, vname, rot_dim, codename, codename, codename, codename);
    string_append_cstr(str, buf);
}

void
code_operate_vector(string_t** str, struct shader_in_variables_struct oper1, struct shader_in_variables_struct oper2, const char* operator, const float defval1, const float defval2)
{
    const char vec4[4] = {'x', 'y', 'z', 'w'};
    char buf[256] = {0};
    char* ptr = &buf[0];
    const uint32 buffer_len = 128;
    uint32 len = string_format(&ptr, buffer_len, "       %s = vec4(", oper1.codename);

    for(uint32 dimention = 0; dimention < SHADER_MAX_LOCATION_SIZE; dimention++) {
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
code_operate_mat2x2(string_t** str, struct shader_in_variables_struct oper1, struct shader_in_variables_struct oper2)
{
    // const char vec4[4] = {'x', 'y', 'z', 'w'};
    const uint32 buf_len = 512;
    char buf[512] = {0};
    char* ptr = &buf[0];
    uint32 len = string_format(&ptr, buf_len, "       %s = vec4(", oper1.codename);

    // Handle dimensions 0 and 1 (x and y) with rotation
    for(uint32 dim = 0; dim < 2; dim++) {

        if(dim == 0) { // x component
            ptr = &buf[len];
            len += string_format(&ptr, buf_len, "(%s.%c * %s_%u) + (%s.%c * -%s_%u)",
                                 oper1.codename, 'x', //  vpos.x
                                 oper2.codename, 0,   //  rot_x
                                 oper1.codename, 'y', //  vpos.y
                                 oper2.codename, 1);  // -rot_y
        } else { // y component
            ptr = &buf[len];
            len += string_format(&ptr, buf_len, "(%s.%c * %s_%u) + (%s.%c * %s_%u)",
                                 oper1.codename, 'x', // vpos.y
                                 oper2.codename, 1,   // rot_y
                                 oper1.codename, 'y', // vpos.x
                                 oper2.codename, 0);  // rot_x
        }

        ptr = &buf[len];
        len += string_buffer_push(&ptr, buf_len, ", ");
    }

    // Handle dimensions 2 and 3 (z and w) - always 0.0
    for(uint32 dim = 2; dim < 4; dim++) {

        ptr = &buf[len];
        len += string_buffer_push(&ptr, 128, "1.0, ");
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
    string_init_table(8);

    for(uint32 i = 0; i < 1; i++) {
        // shader_descriptor_table->data[i].entry = ;
        struct shader_descriptor* desc = shader_descriptor_get(i);
        desc->entry_count = 0;
        // shader_descriptor_table->data[i].sid = (struct gfx_sid){0x0, 0x0, 0x0, 0x0, 0x0};
        // for(uint32 j = 0; j < 1; j++) {
        //     shader_descriptor_table->data[i].entry[j].
        // }
    }
}

#define SHADERBUILDERGLSL_VERSION "#version 330 core\n\n"
#define SHADERBUILDERGLSL_VS_MAIN "void main()\n"
#define SHADERBUILDERGLSL_VS_GL_POSITION "  gl_Position\n"
#define GENERIC_ATTR_LOC "GENERIC_ATTR_LOC"

struct shader_descriptor*
shader_builder_build(const sididx_t sid_idx, const build_t build_idx)
{
    print("\n\n%s- SHADER BUILDER -%s\n", YELLOW, CLEAR);
    /// Get the already builded shader program descriptor, where the attributes have been registered at an erlier stage(mesh creation)
    struct shader_descriptor* desc = shader_descriptor_get(build_idx);

    // initialize the uninitialized fields
    desc->sid_idx = sid_idx;
    desc->vshader_attribs_idx = INT_NULL;
    desc->vshader_attribs_count = 0;
    desc->vshader_attrib_locations_count = 0;
    desc->vshader_code = NULL;
    desc->vshader_code_size = 0;
    desc->build_idx = build_idx;
    desc->vshader_attribs_stride = 0;
    desc->last_loc_num_elems = 0;
    desc->is_set = TRUE;

    string_t* str = string_create_empty(2048);

    string_append_cstr(&str, SHADERBUILDERGLSL_VERSION);

    /// code generation: attributes: in
    string_append_cstr(&str, "/// attributes: in\n");

    uint8 loc_offset = 0; /// the start offset of each attribute at the generic locations
    uint8 attrib_offset = 0; /// the start offset of each attribute per vertex
    uint16 total_offset = 0; /// the total elements count. Used to find the last location's enabled number of elements
    int8 loc_cnt = 0; /// locations index
    int8 prev_loc_idx = -1;


    /***********************************************************************************************
     *  code generation: shader input variables */
    // storage of current shader's attributes info (location, offset, typesize, ...)
    struct vertex_layout* v_layout = desc->v_layout;

    /***********************************************************************************************
    * code generation: create generic locations, algorithm 1 */
    {

        print("%s   create generic locations\n   build_idx:%u%s\n", ry, build_idx, CLEAR);
        uint8 num_elems = 0;
        uint8 type = 0;
        const char* name = NULL;
        uint32 in_var_idx = 0;
        for(; in_var_idx < SHADER_INPUT_ENTRIES_MAX_COUNT; in_var_idx++) {
            struct shader_in_variables_struct entry = desc->entries[in_var_idx];

            if(desc->entries[in_var_idx].which_renameme & IS_ATTR) { // only if input variable is a vertex attribute

                num_elems = entry.elems;
                type = entry.type;
                name = entry.codename;

                /// Record the start position offset for this attribute
                // struct vertex_layout loc = (struct vertex_layout){.loc = loc_cnt, .size = num_elems, .type = type, .offset = total_offset, .idx = in_var_idx};
                struct vertex_layout loc = (struct vertex_layout){.loc = loc_cnt, .size = num_elems, .type = type, .loc_offset = loc_offset, .offset = total_offset, .idx = in_var_idx};
                shader_descriptor_create_vertex_layout(build_idx, loc); // store by attributes, not by locations
                print("   loc:%u elems:%u offset:%u {%s}\n", loc.loc, loc.size, loc.loc_offset, name);

                enum SID_ID attribute_id = shaderinfo_flag_to_id(entry.flag);
                /// store key-value pair of: shader attribute index location with its id (enum SID_VAR_IDI). E.x.
                ///   attrib:vpos: index=0(1st attribute), enum SID_VAR_IDI attribute id=SID_VPOSI2_IDI
                ///   attrib:vpos: index=1(2nd attribute), enum SID_VAR_IDI attribute id=SID_VSCALE2_IDI
                ///   attrib:vpos: index=2(3rd attribute), enum SID_VAR_IDI attribute id=SID_VSCALE2_IDI
                shader_descriptor_set_attribute_ids(build_idx, attribute_id, desc->vlayout_count-1);


                /// create new vec4 generic location
                if(prev_loc_idx != loc_cnt) {
                    char buf[64] = {0};
                    char* ptr = &buf[0];
                    string_format(&ptr, 64, "layout (location = %u) in vec4 generic_%u;\n", loc.loc, loc.loc);
                    string_append_cstr(&str, buf);
                    prev_loc_idx = loc_cnt;
                    desc->vshader_attrib_locations_count++;
                    print("   %s attrib_count:%u %s\n", RED, desc->vshader_attrib_locations_count, CLEAR);
                }

                // Advance the offset by the size of this attribute
                loc_offset += num_elems;
                attrib_offset += num_elems;
                total_offset += num_elems;

                /*
                 * if {current location offset} + {current processed attribute size} extends the max 4 elems per location
                 * 1. create new location (loc_cnt++)
                 * 2. reduce {current location offset} by 4 elems
                 * 3. if attribute size is still > 4 elems(e.x. matrices),
                 *    continue the while loop until we create all new locations that fit the attribute's size.
                 */
                while(loc_offset >= SHADER_MAX_LOCATION_SIZE) {
                    loc_offset -= SHADER_MAX_LOCATION_SIZE;
                    loc_cnt++;
                    if(loc_offset >= SHADER_MAX_LOCATION_SIZE) {
                        loc = (struct vertex_layout){.loc = loc_cnt, .size = num_elems, .type = type, .loc_offset = 0, .idx = in_var_idx};
                        print("   +loc:%u elems:%u offset:%u {%s}\n", loc.loc, loc.size, loc.loc_offset, name);
                        if(prev_loc_idx != loc_cnt) {
                            char buf[64] = {0};
                            char* ptr = &buf[0];
                            string_format(&ptr, 64, "layout (location = %u) in vec4 generic_%u;\n", loc.loc, loc.loc);
                            string_append_cstr(&str, buf);
                            prev_loc_idx = loc_cnt;
                            desc->vshader_attrib_locations_count++;
                        }
                    }
                }

                // attr_desc_cnt++; /// increment vertex_layout counter
            }
        }
        /// create the last generic location, if last attribute's elements span to the next location
        desc->last_loc_num_elems = total_offset % SHADER_MAX_LOCATION_SIZE;
        desc->vshader_attribs_count = desc->vlayout_count;
        if(prev_loc_idx != loc_cnt && desc->last_loc_num_elems != 0) {
            struct vertex_layout loc = (struct vertex_layout){.loc = loc_cnt, .size = num_elems, .type = type, .loc_offset = 0, .idx = in_var_idx};
            print("   {%s} expands to:  loc:%u offset:%u \n", name, loc.loc, loc.loc_offset);
            char buf[64] = {0};
            char* ptr = &buf[0];
            string_format(&ptr, 64, "layout (location = %u) in vec4 generic_%u;\n", loc.loc, loc.loc);
            string_append_cstr(&str, buf);
            desc->vshader_attrib_locations_count++;
        } else if(desc->last_loc_num_elems == 0) { desc->last_loc_num_elems = SHADER_MAX_LOCATION_SIZE; }

        string_append_cstr(&str, "\n");
    }

    /***********************************************************************************************
     * code generation: attributes: out */
    string_append_cstr(&str, "/// attributes: out\n");
    for(uint32 i = 0; i < desc->entry_count; i++) {
        if(desc->entries[i].which_renameme & IS_OUT) {
            const char* format = "out %s out_%s;\n";
            char buf[64] = {0};
            char* ptr = &buf[0];
            string_format(&ptr, 64, format,
                          shaderinfo_type_to_glsltype_string(desc->entries[i].type),
                          desc->entries[i].codename);
            string_append_cstr(&str, buf);
        }
    }
    string_append_cstr(&str, "\n");

    /***********************************************************************************************
     * code generation: uniforms: in */
    string_append_cstr(&str, "/// uniforms: in\n");
    for(uint32 i = 0; i < desc->entry_count; i++) {
        if(desc->entries[i].which_renameme & IS_UNIF) {
            const char* format = "uniform %s %s;\n";
            char buf[64] = {0};
            char* ptr = &buf[0];
            string_format(&ptr, 64, format,
                          shaderinfo_type_to_glsltype_string(desc->entries[i].type),
                          desc->entries[i].codename);
            string_append_cstr(&str, buf);
        }
    }
    string_append_cstr(&str, "\n");

    /***********************************************************************************************
     * code generation: main() */
    string_append_cstr(&str, SHADERBUILDERGLSL_VS_MAIN);
    string_append_cstr(&str, "{\n");

    /***********************************************************************************************
     * code generation: attributes: reassemble generic attributes to variables */
    string_append_cstr(&str, "\n  /// reassemble generic attributes\n");
    const char vec4[4] = {'x', 'y', 'z', 'w'};
    uint32 gen_loc_cnt = 0;
    uint8 dimention = 0;
    uint8 k = 0;
    for(uint32 i = 0; i < desc->vlayout_count; i++) {
        uint32 elems = desc->v_layout[i].size;
        uint32 j = 0;
        for(; j < elems; j++) {
            char buf[128] = {0};
            char* ptr = &buf[0];
            string_format(&ptr, 128,
                          "  float %s_%u = generic_%u.%c;\n",
                          desc->entries[desc->v_layout[i].idx].codename,
                          dimention++, k, vec4[gen_loc_cnt++]
            );
            if(gen_loc_cnt >= SHADER_MAX_LOCATION_SIZE) {
                gen_loc_cnt = 0;
                k++;
            }
            string_append_cstr(&str, buf);
        }
        desc->vshader_attribs_stride += desc->v_layout[i].size;
        dimention = 0;
    }
    string_append_cstr(&str, "\n");


    /***********************************************************************************************
     * code generation: vertex transform */
    string_append_cstr(&str, "\n  /// calculate transform\n");


    /* transformations mock
     * vec4 vpos = vec4(vpos_x, vpos_y, 0.0, 0.0);
     *      vpos = vec4((rot_x * vpos.x)+(-rot_y * vpos.y), (rot_y * vpos.x)+(rot_x * vpos.y), 0.0, 0.0);
     *      vpos = vec4(vpos.x * scale_x, vpos.y * scale_y, 0.0, 0.0);
     *      vpos = vec4(vpos.x + wpos_x,  vpos.y + wpos_y,  0.0, 0.0);
     * gl_position = vpos;
     */
    struct shader_in_variables_struct oper1 = {.codename = "vpos", .elems = desc->entries[0].elems};
    code_create_vector4(&str, desc->entries[0], "vpos", "1.0, ");
    // string_append_cstr(&str, "vec4 vpos = vec4(vpos2_0, vpos2_1, 0.0, 1.0);\n");

    /// scale
    for(uint32 i = 0; i < desc->entry_count; i++) {
        if(desc->entries[i].flag & SID_SCALE2) {
            code_operate_vector(&str, oper1, desc->entries[i], "*", 0.0, 1.0);
        }
    }

    string_append_cstr(&str,
                       "       vpos = mat4("
                       "1.0, 0.0, 0.0, 0.0, "
                       "0.0, rot2_0, -rot2_1, 0.0, "
                       "0.0, rot2_1, rot2_0, 0.0, "
                       "0.0, 0.0, 0.0, 1.0) * vpos;\n");
    // string_append_cstr(&str,
    //                    "       vpos = mat4("
    //                    "rot2_0, 0.0,-rot2_1, 0.0, "
    //                    "0.0,    1.0, 0.0, 0.0, "
    //                    "rot2_1, 0.0, rot2_0, 0.0, "
    //                    "0.0,    0.0, 0.0, 1.0) * vpos;\n");
    string_append_cstr(&str,
                       "       vpos = mat4("
                       "rot2_0, -rot2_1, 0.0, 0.0, "
                       "rot2_1,  rot2_0, 0.0, 0.0, "
                       "0.0,     0.0,    1.0, 0.0, "
                       "0.0,     0.0,    0.0, 1.0) * vpos;\n");


    /// translation
    for(uint32 i = 0; i < desc->entry_count; i++) {
        if(desc->entries[i].flag & SID_WPOSI3) {
            code_operate_vector(&str, oper1, desc->entries[i], "+", 1.0, 0.0);
        }
    }

    string_append_cstr(&str, "  gl_Position = projection_matrix*vpos;\n");
    // string_append_cstr(&str, "  gl_Position = projection_matrix*rotx*vpos;\n");
    // string_append_cstr(&str, "  gl_Position = projection_matrix*rotx*roty*vpos;\n");
    // string_append_cstr(&str, "  gl_Position = projection_matrix*rotz*rotx*roty*vpos;\n");
    // string_append_cstr(&str, "  gl_Position = projection_matrix*rotz*roty*vpos;\n");
    // string_append_cstr(&str, "  gl_Position = projection_matrix*roty*vpos;\n");
    // string_append_cstr(&str, "  gl_Position = roty*vpos;\n");
    // string_append_cstr(&str, "  gl_Position = rotz*rotx*vpos;\n");
    // string_append_cstr(&str, "  gl_Position = rotx * rotz * vpos;\n");
    // string_append_cstr(&str, "  gl_Position = rotz * roty * rotx * vpos;\n");
    // string_append_cstr(&str, "  gl_Position = rotx*rotz* vpos;\n");
    // string_append_cstr(&str, "  gl_Position = rotz*roty*rotx* vpos;\n");


    /***********************************************************************************************
     * code generation: outputs: assignment */
    string_append_cstr(&str, "\n  /// outputs assignment\n");
    for(uint32 i = 0; i < desc->entry_count; i++) {
        if(desc->entries[i].which_renameme & IS_OUT) {
            const char* type_c = shaderinfo_type_to_glsltype_string(desc->entries[i].type);
            const char* name = desc->entries[i].codename;
            char buf[64] = {0};
            char* ptr = &buf[0];
            string_format(&ptr, 64, "  out_%s = %s(",
                          name, type_c, name, name, name, name);
            string_append_cstr(&str, buf);

            const uint32 num_elems = shaderinfo_type_to_numelems(desc->entries[i].type);
            // for(uint32 dim = 0; dim < SHADER_MAX_LOCATION_SIZE; dim++) {
            for(uint32 dim = 0; dim < num_elems; dim++) {
                char buf2[32] = {0};
                char* ptr2 = &buf2[0];
                if(dim < desc->entries[i].elems) {
                    string_format(&ptr2, 32, "%s_%u, ", desc->entries[i].codename, dim);
                    string_append_cstr(&str, buf2);
                } else string_append_cstr(&str, "0.0, ");
            }
            str->len -= 2; /// REMOVE the last space and ','
            string_append_cstr(&str, ");\n");
        }
    }

    /* temporarily create vertex position out */
    string_append_cstr(&str, "  vec2 out_vpos = gl_Position.xy;\n");


    string_append_cstr(&str, "}\n");
    print("\n\n%s- VERTEX SHADER CODE -%s\n%s%s%s\n", YELLOW, CLEAR, ry, str->data, CLEAR);

    /// debug-print
    print("%s--- attribute ids ---%s\n", ry, CLEAR);
    for(uint32 i = 0; i < desc->entry_count; i++) {
        shader_descriptor_print_attribute_ids(build_idx, i);
    }

    desc->vshader_attribs_idx = shader_descriptor_vertex_layout_set(build_idx, v_layout, desc->vshader_attribs_count);
    /// TODO: IMPORTANT! copy shader code to a new string_t with the minimum required size(power of 2 if slaballocator is used)
    desc->vshader_code = str->data;
    desc->vshader_code_size = str->len;

    return desc;
}

void
shader_builder_destroy(void)
{
    string_destroy_table();
}


/* DEBUG-PRINT */
void
shader_inputs_desc_table_print(void)
{
    print("-- %s ATTRIBUTES ARRAY %s --\n", YELLOW, CLEAR);
    for(uint32 i = 0; i < shader_vars_table_count; i++) {
        const struct shader_in_variables_struct a = shader_vars_table[i];
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

void
shader_desc_table_print_build_entry(const build_t idx, const uint8 entryidx)
{
    struct shader_descriptor* build = shader_descriptor_get(idx);
    struct shader_in_variables_struct entry = build->entries[entryidx];
    print(
        "entry: %u\n"
        // "flagname:%s, "
        "codename:%s, "
        "flag:%u, "
        "type:%d, "
        "which_renameme:%d, "
        "typesize:%u, "
        "elems:%u, "
        "var_table_idx:%u, "
        "shader_var_id:%u, \n",
        entryidx,
        // entry.flagname ,
        entry.codename,
        entry.flag,
        entry.type,
        entry.which_renameme,
        entry.typesize,
        entry.elems,
        entry.var_table_idx,
        entry.shader_var_id
    );
}

void
temp_print_builds(void)
{
    print("\n%s- SHADER BUILDS -%s\n", YELLOW, CLEAR);
    const uint32 desc_count = shader_descriptor_get_count();
    for(build_t i = 0; i < desc_count; i++) {
        print("%s build:%u %s\n", ry, i, CLEAR);
        struct shader_descriptor* desc = shader_descriptor_get(i);
        for(uint32 j = 0; j < desc->entry_count; j++) {
            print("%s  entry:%u %s", ry, j, CLEAR);
            print("  %6s, %5s, typesize:%u, elems:%u, flags:%llu\n"
                  "",
                  desc->entries[j].codename,
                  shaderinfo_type_to_glsltype_string(desc->entries[j].type),
                  desc->entries[j].typesize,
                  desc->entries[j].elems,
                  desc->entries[j].which_renameme
            );
        }
    }
}
