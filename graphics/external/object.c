/*
 * Created by ckalo on 6/11/26.
 *
 *
 * TODO: PASS INDEXED/!INDEXED GEOMETRY to SHAPE CREATION
 *
 */

#include "object.h"
#include <stdlib.h>
#include <string.h>

#include "fdebug_test.h"
#include "mesh.h"
#include "shader_builder.h"
#include "buffer_arr_internals.h"
#include "buffer_arr_struct.h"
#include "attributes.h"
#include "bubble_sort.h"
#include "logger.h"


#define obj_get(obj_id) (&((struct object*) object_table->data)[obj_id])


static objid_t object_id = 0;
buffer_t* object_table = NULL;


void
object_table_init(void)
{
    object_table = buffer_arr_create_gentype(80, sizeof(struct object));
}

struct object*
object_create(const sididx_t sid_idx, const enum MESH_TYPE mesh_type)
{
    const struct object obj = {
        .id = object_id++,
        .sid_idx = sid_idx,
        .num_faces = 0,
        .attrib_count = 0,
        .attribs_group_id = 0,
        .verts_per_face = 0,
        .mesh_type = mesh_type,
        .vb_start = 0,
        .vertex_count = 0,
        .index_count = 0,
    };

    const uint16 obj_idx = buffer_arr_set_for_push(object_table);
    struct object* object = obj_get(obj_idx);
    *object = obj;

    return object;
}

void
object_destroy(const objid_t obj_id)
{
    buffer_arr_free_elem(object_table, obj_id);
}

/*************************************************************************************************
 * Getters
 */
struct object*
object_get(const objid_t obj_id)
{
    assert(obj_id >= 0);
    struct object* object = obj_get(obj_id);
    return object;
}

uint16
object_get_vertex_count(const objid_t obj_id)
{
    assert(obj_id >= 0);
    struct object* object = obj_get(obj_id);
    return object->vertex_count;
}

vind_t
object_get_vertex_index_count(const objid_t obj_id)
{
    assert(obj_id >= 0);
    struct object* object = obj_get(obj_id);
    return object->index_count;
}

void
object_set_vertex_count(const objid_t obj_id, const uint16 vertex_count)
{
    assert(obj_id >= 0);
    struct object* object = obj_get(obj_id);
    object->vertex_count = vertex_count;
    print("%s obj id:%d, vertex count:%u %s\n", RED, obj_id, vertex_count, CLEAR);
}

enum MESH_TYPE
object_get_vertex_mesh_type(const objid_t obj_id)
{
    assert(obj_id >= 0);
    const objid_t obj_idx = buffer_arr_set_for_push(object_table);
    struct object* object = obj_get(obj_idx);
    return object->mesh_type;
}

void object_add_attribute(const attrid_t attribs_group_id, const build_t build_idx, const enum SID_ID shader_input_id,
                          const uint16 divisor, enum SHADER_INPUT_FLAGS attrib_flags, const float* const src)
{
    struct attributes_descriptor* attribs_descr = attribsGroupDescr_get_attribs_descr(shader_input_id);
    struct shader_entry* entry = shader_input_get_entry(shader_input_id);

    shader_create_input(build_idx, shader_input_id, attrib_flags, divisor);

    attribsGroupDescr_add_attributes(attribs_group_id, src, entry->elems, attribs_descr->attr_offset, entry->flag, divisor, shader_input_id);

    uint32 stride = attribsGroupDescr_get_attribs_stride(attribs_group_id);
    attribsGroupDescr_set_stride(attribs_group_id, stride+entry->elems);
    attribs_descr->attr_offset += stride;

}


/*
 * If an object is defined initially by the sid_attr(and the rest sids),
 * then we could automate the creation of all primitive shapes
 * with a for loop somehow.
 */
struct object*
object_create_triangle(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
{
    sid.attr |= SID_VPOSI2 | SID_SCALE2 | SID_COLOR4;
    sid.unif |= SID_PROJMAT4X4;
    sid.out  |= SID_COLOR4;
    sid.pass |= SID_COLOR4;
    sid.oper |= SID_NULL;

    enum MESH_TYPE mesh_type = MESH_TRIANGLE;

    sididx_t sid_idx = gfx_sid_find_match(sid);
    attrid_t attribs_group_id = INT_NULL;

    if(sid_idx != INT_NULL) { /// 1. attribute descriptor group exists
        attribs_group_id = attribsGroupDescr_group_find_match(sid_idx);
    } else { /// 2. create new attribute descriptor group
        sid_idx = gfx_create_sid(sid);
        attribs_group_id = attribsGroupDescr_create_new_attribs_group(sid_idx);
    }

    struct object* obj  = object_create(sid_idx, mesh_type);
    obj->num_faces      = 1;
    obj->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
    obj->index_count    = mesh_get_mesh_indices_count_from_type(mesh_type);
    obj->attribs_group_id     = attribs_group_id;

    object_set_vertex_count(obj->id, obj->num_faces * obj->verts_per_face);
    attribsGroupDescr_create_new_obj(obj->attribs_group_id, obj->id, obj->num_faces * obj->verts_per_face);

    /*
     * TODO:
     *
     * sort all attributes per the divisor's value
     *      we need buffers for:
     *          TYPE_ID
     *          SID_ID
     *
     *      all buffers are of length: attributes count
     *
     *
     * break apart the:
     *      1. does shader build exist? via sid_idx
     *          . create new build
     *          . store build's id
     *
     *      -- then, in a loop: for all SID_FLAGS --
     *      2. does shader attribute input exist?
     *          . create new shader attribute input
     *          .
     *      3.
     *
     *
     * the object can create new shader inputs. this is only feasible if the user passes the "NAME, TYPE, etc".
     * The best way to create new shader inputs is one by one, otherwise we would have to pass an array of these "NAMES, TYPES, etc"
     * So we need to:
     *      1. create all shader inputs in the global buffer "shader_inputs_table"
     *      2. for new entries: through NAME create the flag and the SID_ID
     *      The quesion is: If SID_ID is not compile time, how to access an entry by SID_ID ????
     *      Maybe we "keep" the SID_ID enumeration for the "known" shader inputs
     *      and do a search for any user defined.
     *
     */


    /// 1. find or create shader build
    build_t build_idx = shader_build_exists(sid_idx);
    if(build_idx == INT_NULL) build_idx = shader_builder_create_new_build(sid_idx);
    attribsGroupDescr_set_build_idx(attribs_group_id, build_idx);


    struct attributes_descriptor temp_buffer[4];
    enum SHADER_INPUT_FLAGS attrib_flags = 0x0;
    enum SID_ID shader_input_id = 0;
    uint16 divisor = 0; /// instanced rendering

    /**********************************************************************************************
     * ATTRIBUTES */
    // add vpos attribute
    attrib_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS;
    shader_input_id = SID_VPOSI2_ID;
    shader_create_input(build_idx, shader_input_id, attrib_flags, 0);

    // /// add wpos attribute
    // uint32 cnt = 0;
    // temp_buffer[cnt].src = &wpos[0];
    // temp_buffer[cnt].attrib_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS;
    // temp_buffer[cnt].shader_input_id = SID_WPOSI3_ID;
    // temp_buffer[cnt++].divisor = 1;
    // /// add scale attribute
    // temp_buffer[cnt].src = &scale[0];
    // temp_buffer[cnt].attrib_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE;
    // temp_buffer[cnt].shader_input_id = SID_SCALE2_ID;
    // temp_buffer[cnt++].divisor = 1;
    // /// add rotation attribute
    // temp_buffer[cnt].src = &rot[0];
    // temp_buffer[cnt].attrib_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT;
    // temp_buffer[cnt].shader_input_id = SID_ROTAT2_ID;
    // temp_buffer[cnt++].divisor = 1;
    // /// add color attribute
    // temp_buffer[cnt].src = &color[0];
    // temp_buffer[cnt].attrib_flags = IN_VSHADER | IS_ATTR | IS_OUT;
    // temp_buffer[cnt].shader_input_id = SID_COLOR4_ID;
    // temp_buffer[cnt++].divisor = 0;

    /// sort all attributes by their divisor
    /// TODO: only if there are any different divisors???
    bubble_sort_generic_fast((void*)temp_buffer, 4, sizeof(struct attributes_descriptor), compare_struct_attribs_per_attrib_descriptor);
    print_struct_attribs_per_attrib_descriptor(temp_buffer, 4);


    /// store sorted attributes to a buffer for rendering
    struct attributes_group* group = attribsGroupDescr_get_attribs_group(attribs_group_id);
    struct attributes_descriptor* attribs_descr = attribsGroupDescr_get_attribs_descr(attribs_group_id);
    struct shader_entry* entry = shader_input_get_entry(shader_input_id);

    for(uint32 i = 0; i < 4; i++) {
        print("attr_offset: %u attr_num_elems: %u\n", attribs_descr[i].attr_offset, attribs_descr[i].attr_num_elems);
        // object_add_attribute(obj->attribs_group_id, build_idx, temp_buffer[i].shader_input_id, temp_buffer[i].divisor, temp_buffer[i].attrib_flags, temp_buffer[i].src);
    }


    /***********************************************************************************************
     * UNIFORMS */
    /// add projection matrix uniform
    attrib_flags = IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ;
    shader_input_id = SID_PROJMAT4X4_ID;
    shader_create_input(build_idx, shader_input_id, attrib_flags, divisor);

    uint32 stride = attribsGroupDescr_get_attribs_stride(attribs_group_id);
    attribsGroupDescr_set_stride(attribs_group_id, stride+entry->elems);
    attribs_descr->attr_offset += stride;


    print("%s- Create object: triangle -%s\n"
          "   id:%u\n   attribs_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->attribs_group_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");

    return obj;
}



//
// struct object*
// object_create_rectangle(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
// {
//     sid.attr |= SID_VPOSI2 | SID_SCALE2 | SID_COLOR4;
//     sid.unif |= SID_PROJMAT4X4;
//     sid.out |= SID_COLOR4;
//     sid.pass |= SID_COLOR4;
//     sid.oper |= SID_NULL;
//
//     enum MESH_TYPE mesh_type = MESH_RECTANGLE;
//
//     sididx_t sid_idx = gfx_sid_find_match(sid);
//     attrid_t attribs_group_id = INT_NULL;
//
//     if(sid_idx == INT_NULL) {
//         sid_idx = gfx_create_sid(sid);
//         attribs_group_id = attribsGroupDescr_create_new_components_group(sid_idx);
//     } else {
//         attribs_group_id = attribsGroupDescr_group_find_match(sid_idx);
//     }
//     assert(attribs_group_id >= 0);
//
//     struct object* obj = object_create(sid_idx, mesh_type);
//     obj->num_faces = 1;
//     obj->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
//     obj->index_count = mesh_get_mesh_indices_count_from_type(mesh_type);
//     obj->attribs_id = attribs_group_id;
//
//     object_set_vertex_count(obj->id, obj->num_faces * obj->verts_per_face);
//     attribsGroupDescr_create_new_obj(obj->attribs_id, obj->id, obj->num_faces * obj->verts_per_face);
//
//
//     /* Create the necessary attribute descriptor and store the attribute data
//     * Do this for all attributes */
//     uint32 attrib_offset = 0;
//     sid_t sid_attr = 0x0; /// which attribute is it
//     uint16 inst_divisor = 0; /// instanced rendering
//
//     struct RET_attrib_input shader_ret = shader_create_input(sid_idx, "VPOS2", VEC2F, SID_VPOSI2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS, inst_divisor);
//     attribsGroupDescr_set_build_idx(attribs_group_id, shader_ret.build_idx);
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: %s, id:%3u  attribs_id:%u  num_faces:%2u  verts_per_face:%3u   attrib_count:%u | ",
//                                        mesh_get_name_from_type(mesh_type), obj->id, obj->attribs_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     {
//         shader_ret = shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS, inst_divisor);
//         sid_attr = SID_WPOSI3;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, wpos, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE, inst_divisor);
//         sid_attr = SID_SCALE2;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, scale, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT, inst_divisor);
//         sid_attr = SID_ROTAT2;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, rot, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_ID, IN_VSHADER | IS_ATTR | IS_OUT, inst_divisor);
//         sid_attr = SID_COLOR4;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, color, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//
//
//     attribsGroupDescr_set_stride(obj->attribs_id, attrib_offset);
//
//     shader_ret = shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_ID, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ, inst_divisor);
//
//     print("%s- Create object: rectangle -%s\n"
//           "   id:%u\n   attribs_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
//           BLUE, CLEAR, obj->id, obj->attribs_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
//     return obj;
// }
//
// struct object*
// object_create_pentagon(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
// {
//     sid.attr |= SID_VPOSI2 | SID_SCALE2 | SID_COLOR4;
//     sid.unif |= SID_PROJMAT4X4;
//     sid.out |= SID_COLOR4;
//     sid.pass |= SID_COLOR4;
//     sid.oper |= SID_NULL;
//
//     enum MESH_TYPE mesh_type = MESH_PENTAGON;
//
//     sididx_t sid_idx = gfx_sid_find_match(sid);
//     attrid_t attribs_group_id = INT_NULL;
//
//     if(sid_idx == INT_NULL) {
//         sid_idx = gfx_create_sid(sid);
//         attribs_group_id = attribsGroupDescr_create_new_components_group(sid_idx);
//     } else {
//         attribs_group_id = attribsGroupDescr_group_find_match(sid_idx);
//     }
//     assert(attribs_group_id >= 0);
//
//     struct object* obj = object_create(sid_idx, mesh_type);
//     obj->num_faces = 1;
//     obj->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
//     obj->index_count = mesh_get_mesh_indices_count_from_type(mesh_type);
//     obj->attribs_id = attribs_group_id;
//
//     object_set_vertex_count(obj->id, obj->num_faces * obj->verts_per_face);
//     attribsGroupDescr_create_new_obj(obj->attribs_id, obj->id, obj->num_faces * obj->verts_per_face);
//
//
//     /* Create the necessary attribute descriptor and store the attribute data
//     * Do this for all attributes */
//     uint32 attrib_offset = 0;
//     sid_t sid_attr = 0x0; /// which attribute is it
//     uint16 inst_divisor = 0; /// instanced rendering
//
//     struct RET_attrib_input shader_ret = shader_create_input(sid_idx, "VPOS2", VEC2F, SID_VPOSI2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS, inst_divisor);
//     attribsGroupDescr_set_build_idx(attribs_group_id, shader_ret.build_idx);
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: %s, id:%3u  attribs_id:%u  num_faces:%2u  verts_per_face:%3u   attrib_count:%u | ",
//                                        mesh_get_name_from_type(mesh_type), obj->id, obj->attribs_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     {
//         shader_ret = shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS, inst_divisor);
//         sid_attr = SID_WPOSI3;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, wpos, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE, inst_divisor);
//         sid_attr = SID_SCALE2;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, scale, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT, inst_divisor);
//         sid_attr = SID_ROTAT2;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, rot, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_ID, IN_VSHADER | IS_ATTR | IS_OUT, inst_divisor);
//         sid_attr = SID_COLOR4;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, color, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//
//
//     attribsGroupDescr_set_stride(obj->attribs_id, attrib_offset);
//
//     shader_ret = shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_ID, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ, inst_divisor);
//
//     print("%s- Create object: pentagon -%s\n"
//           "   id:%u\n   attribs_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
//           BLUE, CLEAR, obj->id, obj->attribs_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
//     return obj;
// }
//
// struct object*
// object_create_hexagon(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
// {
//     sid.attr |= SID_VPOSI2 | SID_SCALE2 | SID_COLOR4;
//     sid.unif |= SID_PROJMAT4X4;
//     sid.out |= SID_COLOR4;
//     sid.pass |= SID_COLOR4;
//     sid.oper |= SID_NULL;
//
//     enum MESH_TYPE mesh_type = MESH_HEXAGON;
//
//     sididx_t sid_idx = gfx_sid_find_match(sid);
//     attrid_t attribs_group_id = INT_NULL;
//
//     if(sid_idx == INT_NULL) {
//         sid_idx = gfx_create_sid(sid);
//         attribs_group_id = attribsGroupDescr_create_new_components_group(sid_idx);
//     } else {
//         attribs_group_id = attribsGroupDescr_group_find_match(sid_idx);
//     }
//     assert(attribs_group_id >= 0);
//
//     struct object* obj = object_create(sid_idx, mesh_type);
//     obj->num_faces = 1;
//     obj->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
//     obj->index_count = mesh_get_mesh_indices_count_from_type(mesh_type);
//     obj->attribs_id = attribs_group_id;
//
//     object_set_vertex_count(obj->id, obj->num_faces * obj->verts_per_face);
//     attribsGroupDescr_create_new_obj(obj->attribs_id, obj->id, obj->num_faces * obj->verts_per_face);
//
//
//     /* Create the necessary attribute descriptor and store the attribute data
//     * Do this for all attributes */
//     uint32 attrib_offset = 0;
//     sid_t sid_attr = 0x0; /// which attribute is it
//     uint16 inst_divisor = 0; /// instanced rendering
//
//     struct RET_attrib_input shader_ret = shader_create_input(sid_idx, "VPOS2", VEC2F, SID_VPOSI2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS, inst_divisor);
//     attribsGroupDescr_set_build_idx(attribs_group_id, shader_ret.build_idx);
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: %s, id:%3u  attribs_id:%u  num_faces:%2u  verts_per_face:%3u   attrib_count:%u | ",
//                                        mesh_get_name_from_type(mesh_type), obj->id, obj->attribs_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     {
//         shader_ret = shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS, inst_divisor);
//         sid_attr = SID_WPOSI3;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, wpos, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE, inst_divisor);
//         sid_attr = SID_SCALE2;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, scale, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT, inst_divisor);
//         sid_attr = SID_ROTAT2;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, rot, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_ID, IN_VSHADER | IS_ATTR | IS_OUT, inst_divisor);
//         sid_attr = SID_COLOR4;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, color, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//
//
//     attribsGroupDescr_set_stride(obj->attribs_id, attrib_offset);
//
//     shader_ret = shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_ID, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ, inst_divisor);
//
//     print("%s- Create object: pentagon -%s\n"
//           "   id:%u\n   attribs_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
//           BLUE, CLEAR, obj->id, obj->attribs_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
//     return obj;
// }
//
// struct object*
// object_create_octagon(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
// {
//     sid.attr = sid.attr | SID_VPOSI2 | SID_SCALE2 | SID_COLOR4;
//     sid.unif = sid.unif | SID_PROJMAT4X4;
//     sid.out = sid.out | SID_COLOR4;
//     sid.pass = sid.pass | SID_COLOR4;
//     sid.oper = sid.oper | SID_NULL;
//
//     enum MESH_TYPE mesh_type = MESH_OCTAGON;
//
//     sididx_t sid_idx = gfx_sid_find_match(sid);
//     attrid_t attribs_group_id = INT_NULL;
//
//     if(sid_idx == INT_NULL) {
//         sid_idx = gfx_create_sid(sid);
//         attribs_group_id = attribsGroupDescr_create_new_components_group(sid_idx);
//     } else {
//         attribs_group_id = attribsGroupDescr_group_find_match(sid_idx);
//     }
//     assert(attribs_group_id >= 0);
//
//     struct object* obj = object_create(sid_idx, mesh_type);
//     obj->num_faces = 1;
//     obj->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
//     obj->index_count = mesh_get_mesh_indices_count_from_type(mesh_type);
//     obj->attribs_id = attribs_group_id;
//
//     object_set_vertex_count(obj->id, obj->num_faces * obj->verts_per_face);
//     attribsGroupDescr_create_new_obj(obj->attribs_id, obj->id, obj->num_faces * obj->verts_per_face);
//
//
//     /* Create the necessary attribute descriptor and store the attribute data
//     * Do this for all attributes */
//     uint32 attrib_offset = 0;
//     sid_t sid_attr = 0x0; /// which attribute is it
//     uint16 inst_divisor = 0; /// instanced rendering
//
//     struct RET_attrib_input shader_ret = shader_create_input(sid_idx, "VPOS2", VEC2F, SID_VPOSI2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS, inst_divisor);
//     attribsGroupDescr_set_build_idx(attribs_group_id, shader_ret.build_idx);
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: %s, id:%3u  attribs_id:%u  num_faces:%2u  verts_per_face:%3u   attrib_count:%u | ",
//                                        mesh_get_name_from_type(mesh_type), obj->id, obj->attribs_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     {
//         shader_ret = shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS, inst_divisor);
//         sid_attr = SID_WPOSI3;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, wpos, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE, inst_divisor);
//         sid_attr = SID_SCALE2;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, scale, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT, inst_divisor);
//         sid_attr = SID_ROTAT2;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, rot, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_ID, IN_VSHADER | IS_ATTR | IS_OUT, inst_divisor);
//         sid_attr = SID_COLOR4;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, color, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//
//
//     attribsGroupDescr_set_stride(obj->attribs_id, attrib_offset);
//
//     shader_ret = shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_ID, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ, inst_divisor);
//
//     print("%s- Create object: rectangle -%s\n"
//           "   id:%u\n   attribs_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
//           BLUE, CLEAR, obj->id, obj->attribs_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
//     return obj;
// }
//
// struct object*
// object_create_cube(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
// {
//     sid.attr |= SID_VPOSI3 | SID_SCALE2 | SID_COLOR4;
//     sid.unif |= SID_PROJMAT4X4;
//     sid.out |= SID_COLOR4;
//     sid.pass |= SID_COLOR4;
//     sid.oper |= SID_NULL;
//
//     enum MESH_TYPE mesh_type = MESH_CUBE;
//
//     sididx_t sid_idx = gfx_sid_find_match(sid);
//     attrid_t attribs_group_id = INT_NULL;
//
//     if(sid_idx == INT_NULL) {
//         sid_idx = gfx_create_sid(sid);
//         attribs_group_id = attribsGroupDescr_create_new_components_group(sid_idx);
//     } else {
//         attribs_group_id = attribsGroupDescr_group_find_match(sid_idx);
//     }
//     assert(attribs_group_id >= 0);
//
//     struct object* obj = object_create(sid_idx, mesh_type);
//     obj->num_faces = 1;
//     obj->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
//     obj->index_count = mesh_get_mesh_indices_count_from_type(mesh_type);
//     obj->attribs_id = attribs_group_id;
//
//     object_set_vertex_count(obj->id, obj->num_faces * obj->verts_per_face);
//     attribsGroupDescr_create_new_obj(obj->attribs_id, obj->id, obj->num_faces * obj->verts_per_face);
//
//
//     /* Create the necessary attribute descriptor and store the attribute data
//     * Do this for all attributes */
//     uint32 attrib_offset = 0;
//     sid_t sid_attr = 0x0; /// which attribute is it
//     uint16 inst_divisor = 0; /// instanced rendering
//
//     struct RET_attrib_input shader_ret = shader_create_input(sid_idx, "VPOS3", VEC3F, SID_VPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS, inst_divisor);
//     attribsGroupDescr_set_build_idx(attribs_group_id, shader_ret.build_idx);
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: %s, id:%3u  attribs_id:%u  num_faces:%2u  verts_per_face:%3u   attrib_count:%u | ",
//                                        mesh_get_name_from_type(mesh_type), obj->id, obj->attribs_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     {
//         shader_ret = shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS, inst_divisor);
//         sid_attr = SID_WPOSI3;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, wpos, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE, inst_divisor);
//         sid_attr = SID_SCALE2;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, scale, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT, inst_divisor);
//         sid_attr = SID_ROTAT2;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, rot, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_ID, IN_VSHADER | IS_ATTR | IS_OUT, inst_divisor);
//         sid_attr = SID_COLOR4;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, color, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//
//
//     attribsGroupDescr_set_stride(obj->attribs_id, attrib_offset);
//
//     shader_ret = shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_ID, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ, inst_divisor);
//
//     print("%s- Create object: cube -%s\n"
//           "   id:%u\n   attribs_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
//           BLUE, CLEAR, obj->id, obj->attribs_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
//     return obj;
// }
//
// struct object*
// object_create_octahedron(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
// {
//     sid.attr |= SID_VPOSI3 | SID_SCALE2 | SID_COLOR4;
//     sid.unif |= SID_PROJMAT4X4;
//     sid.out |= SID_COLOR4;
//     sid.pass |= SID_COLOR4;
//     sid.oper |= SID_NULL;
//
//     enum MESH_TYPE mesh_type = MESH_OCTAHEDRON;
//
//     sididx_t sid_idx = gfx_sid_find_match(sid);
//     attrid_t attribs_group_id = INT_NULL;
//
//     if(sid_idx == INT_NULL) {
//         sid_idx = gfx_create_sid(sid);
//         attribs_group_id = attribsGroupDescr_create_new_components_group(sid_idx);
//     } else {
//         attribs_group_id = attribsGroupDescr_group_find_match(sid_idx);
//     }
//     assert(attribs_group_id >= 0);
//
//     struct object* obj = object_create(sid_idx, mesh_type);
//     obj->num_faces = 1;
//     obj->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
//     obj->index_count = mesh_get_mesh_indices_count_from_type(mesh_type);
//     obj->attribs_id = attribs_group_id;
//
//     object_set_vertex_count(obj->id, obj->num_faces * obj->verts_per_face);
//     attribsGroupDescr_create_new_obj(obj->attribs_id, obj->id, obj->num_faces * obj->verts_per_face);
//
//
//     /* Create the necessary attribute descriptor and store the attribute data
//     * Do this for all attributes */
//     uint32 attrib_offset = 0;
//     sid_t sid_attr = 0x0; /// which attribute is it
//     uint16 inst_divisor = 0; /// instanced rendering
//
//     struct RET_attrib_input shader_ret = shader_create_input(sid_idx, "VPOS3", VEC3F, SID_VPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS, inst_divisor);
//     attribsGroupDescr_set_build_idx(attribs_group_id, shader_ret.build_idx);
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: %s, id:%3u  attribs_id:%u  num_faces:%2u  verts_per_face:%3u   attrib_count:%u | ",
//                                        mesh_get_name_from_type(mesh_type), obj->id, obj->attribs_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     {
//         shader_ret = shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS, inst_divisor);
//         sid_attr = SID_WPOSI3;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, wpos, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE, inst_divisor);
//         sid_attr = SID_SCALE2;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, scale, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT, inst_divisor);
//         sid_attr = SID_ROTAT2;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, rot, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_ID, IN_VSHADER | IS_ATTR | IS_OUT, inst_divisor);
//         sid_attr = SID_COLOR4;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, color, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//
//
//     attribsGroupDescr_set_stride(obj->attribs_id, attrib_offset);
//
//     shader_ret = shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_ID, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ, inst_divisor);
//
//     print("%s- Create object: octahedron -%s\n"
//           "   id:%u\n   attribs_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
//           BLUE, CLEAR, obj->id, obj->attribs_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
//     return obj;
// }
//
// struct object*
// object_create_dodecahedron(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
// {
//     sid.attr |= SID_VPOSI3 | SID_SCALE2 | SID_COLOR4;
//     sid.unif |= SID_PROJMAT4X4;
//     sid.out |= SID_COLOR4;
//     sid.pass |= SID_COLOR4;
//     sid.oper |= SID_NULL;
//
//     enum MESH_TYPE mesh_type = MESH_DODECAHEDRON;
//
//     sididx_t sid_idx = gfx_sid_find_match(sid);
//     attrid_t attribs_group_id = INT_NULL;
//
//     if(sid_idx == INT_NULL) {
//         sid_idx = gfx_create_sid(sid);
//         attribs_group_id = attribsGroupDescr_create_new_components_group(sid_idx);
//     } else {
//         attribs_group_id = attribsGroupDescr_group_find_match(sid_idx);
//     }
//     assert(attribs_group_id >= 0);
//
//     struct object* obj = object_create(sid_idx, mesh_type);
//     obj->num_faces = 1;
//     obj->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
//     obj->index_count = mesh_get_mesh_indices_count_from_type(mesh_type);
//     obj->attribs_id = attribs_group_id;
//
//     object_set_vertex_count(obj->id, obj->num_faces * obj->verts_per_face);
//     attribsGroupDescr_create_new_obj(obj->attribs_id, obj->id, obj->num_faces * obj->verts_per_face);
//
//
//     /* Create the necessary attribute descriptor and store the attribute data
//     * Do this for all attributes */
//     uint32 attrib_offset = 0;
//     sid_t sid_attr = 0x0; /// which attribute is it
//     uint16 inst_divisor = 0; /// instanced rendering
//
//     struct RET_attrib_input shader_ret = shader_create_input(sid_idx, "VPOS3", VEC3F, SID_VPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS, inst_divisor);
//     attribsGroupDescr_set_build_idx(attribs_group_id, shader_ret.build_idx);
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: %s, id:%3u  attribs_id:%u  num_faces:%2u  verts_per_face:%3u   attrib_count:%u | ",
//                                        mesh_get_name_from_type(mesh_type), obj->id, obj->attribs_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     {
//         shader_ret = shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS, inst_divisor);
//         sid_attr = SID_WPOSI3;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, wpos, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE, inst_divisor);
//         sid_attr = SID_SCALE2;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, scale, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT, inst_divisor);
//         sid_attr = SID_ROTAT2;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, rot, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//     {
//         shader_ret = shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_ID, IN_VSHADER | IS_ATTR | IS_OUT, inst_divisor);
//         sid_attr = SID_COLOR4;
//         const uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
//         attribsGroupDescr_add_attributes(obj->attribs_id, color, attrib_size, attrib_offset, sid_attr, inst_divisor, shader_ret.shader_attrib_idx);
//         attrib_offset += attrib_size;
//         obj->attrib_count++;
//     }
//
//
//     attribsGroupDescr_set_stride(obj->attribs_id, attrib_offset);
//
//     shader_ret = shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_ID, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ, inst_divisor);
//
//     print("%s- Create object: dodecahedron -%s\n"
//           "   id:%u\n   attribs_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
//           BLUE, CLEAR, obj->id, obj->attribs_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
//     return obj;
// }
