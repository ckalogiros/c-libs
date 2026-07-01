/*
 * Created by ckalo on 6/11/26.
 *
 *
 * TODO: PASS INDEXED/!INDEXED GEOMETRY to SHAPE CREATION
 *
 */

#include "object.h"
#include <stdlib.h>
#include "fdebug_test.h"
#include "mesh.h"
#include "shader_builder.h"
#include "sparse_arr_internals.h"
#include "sparse_arr_struct.h"
#include "object_components.h"


#define obj_get(obj_id) (&((struct object*) object_table->data)[obj_id])


static uint32 object_id = 0;
sparse_arr_t* object_table = NULL;


void
object_table_init(void)
{
    object_table = sparse_arr_constructor_gentype(80, sizeof(struct object));
}

struct object*
object_create(const sididx_t sid_idx, enum MESH_TYPE mesh_type)
{
    struct object obj = {
        .id             = object_id++,
        .sid_idx        = sid_idx,
        .num_faces      = 0,
        .attrib_count   = 0,
        .comp_id        = 0,
        .verts_per_face = 0,
        .mesh_type      = mesh_type,
        .vb_start       = 0,
        .vertex_count   = 0,
    };

    const uint16 obj_idx = sparse_arr_set_for_push(object_table);
    struct object* object = obj_get(obj_idx);
    *object = obj;

    return object;
}

void object_destroy(const objid_t obj_id)
{
    sparse_arr_free_elem(object_table, obj_id);
}

/*************************************************************************************************
 * Getters
 */
struct object*
object_get(const objid_t obj_id)
{
    assert(obj_id > 0);
    const desc_t obj_idx = sparse_arr_set_for_push(object_table);
    struct object* object = obj_get(obj_idx);
    return object;
}
uint16
object_get_vertex_count(const objid_t obj_id)
{
    assert(obj_id > 0);
    const desc_t obj_idx = sparse_arr_set_for_push(object_table);
    struct object* object = obj_get(obj_idx);
    return object->vertex_count;
}
enum MESH_TYPE
object_get_vertex_mesh_type(const objid_t obj_id)
{
    assert(obj_id >= 0);
    const desc_t obj_idx = sparse_arr_set_for_push(object_table);
    struct object* object = obj_get(obj_idx);
    return object->mesh_type;
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

    sididx_t sid_idx = gfx_sid_find_match(sid);
    compid_t comp_group_id = INT_NULL;

    if(sid_idx == INT_NULL) {

        sid_idx = gfx_create_sid(sid);
        comp_group_id = obj_component_create_new_components_group(sid_idx);
    } else {

        comp_group_id = obj_component_group_find_match(sid_idx);
    }

    struct object* obj = object_create(sid_idx, MESH_TRIANGLE);
    obj->num_faces = 1;
    obj->verts_per_face = VERTICES_PER_TRI;
    obj->comp_id = comp_group_id;

    obj_component_set_obj_vertex_count(obj->comp_id, obj->num_faces * obj->verts_per_face);
    const uint32 obj_comp_idx = obj_component_create_new_obj(obj->comp_id, obj->num_faces * obj->verts_per_face, MESH_TRIANGLE);

    const build_t build_idx = shader_create_input(sid_idx, "VPOS2", VEC2F, SID_VPOSI2_ID, IN_VSHADER | IS_ATTR  | IS_TRANSFORM_VPOS);
    obj_component_set_build_idx(comp_group_id, build_idx);


    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: %s, id:%u  comp_id:%u  num_faces:%2u  verts_per_face:%3u   attrib_count:%u | ",
            mesh_get_name_from_type(MESH_TRIANGLE), obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);


    /* Create the necessary attribute descriptor and store the attributes data
    * Do this for all attributes */
    uint32 attrib_offset = 0;
    sid_t sid_attr = 0x0;
    {
        shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS);
        sid_attr = SID_WPOSI3;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, wpos, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE);
        sid_attr = SID_SCALE2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, scale, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT);
        sid_attr = SID_ROTAT2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, rot, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_ID, IN_VSHADER | IS_ATTR | IS_OUT);
        sid_attr = SID_COLOR4;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, color, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }

    obj_component_set_stride(obj->comp_id, attrib_offset);

    shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_ID, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ);

    print("%s- Create object: triangle -%s\n"
          "   id:%u\n   comp_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");

    return obj;
}

struct object*
object_create_rectangle(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
{
    sid.attr |= SID_VPOSI2 | SID_SCALE2 | SID_COLOR4;
    sid.unif |= SID_PROJMAT4X4;
    sid.out  |= SID_COLOR4;
    sid.pass |= SID_COLOR4;
    sid.oper |= SID_NULL;

    sididx_t sid_idx = gfx_sid_find_match(sid);
    compid_t comp_group_id = INT_NULL;

    if(sid_idx == INT_NULL) {

        sid_idx = gfx_create_sid(sid);
        comp_group_id = obj_component_create_new_components_group(sid_idx);
    } else {

        comp_group_id = obj_component_group_find_match(sid_idx);
    }
    assert(comp_group_id >= 0);

    struct object* obj = object_create(sid_idx, MESH_RECTANGLE);
    obj->num_faces = 1;
    obj->verts_per_face = VERTICES_PER_QUAD;
    obj->comp_id = comp_group_id;


    obj_component_set_obj_vertex_count(obj->comp_id, obj->num_faces * obj->verts_per_face);
    const uint32 obj_comp_idx = obj_component_create_new_obj(obj->comp_id, obj->num_faces * obj->verts_per_face, MESH_RECTANGLE);

    const build_t build_idx = shader_create_input(sid_idx, "VPOS2", VEC2F, SID_VPOSI2_ID, IN_VSHADER | IS_ATTR  | IS_TRANSFORM_VPOS);
    obj_component_set_build_idx(comp_group_id, build_idx);


    // engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: RECTANGLE, id: %u\n", obj->id);
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: %s, id:%u  comp_id:%u  num_faces:%2u  verts_per_face:%3u   attrib_count:%u | ",
            mesh_get_name_from_type(MESH_RECTANGLE), obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);


    /* Create the necessary attribute descriptor and store the attributes data
    * Do this for all attributes */
    uint32 attrib_offset = 0;
    sid_t sid_attr = 0x0; /// attribute flag
    {
        shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS);
        sid_attr = SID_WPOSI3;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, wpos, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE);
        sid_attr = SID_SCALE2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, scale, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT);
        sid_attr = SID_ROTAT2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, rot, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_ID, IN_VSHADER | IS_ATTR | IS_OUT);
        sid_attr = SID_COLOR4;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, color, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }


    obj_component_set_stride(obj->comp_id, attrib_offset);

    shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_ID, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ);

    print("%s- Create object: rectangle -%s\n"
          "   id:%u\n   comp_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
    return obj;
}

struct object*
object_create_pentagon(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
{
    sid.attr |= SID_VPOSI2 | SID_SCALE2 | SID_COLOR4;
    sid.unif |= SID_PROJMAT4X4;
    sid.out  |= SID_COLOR4;
    sid.pass |= SID_COLOR4;
    sid.oper |= SID_NULL;

    sididx_t sid_idx = gfx_sid_find_match(sid);
    compid_t comp_group_id = INT_NULL;

    if(sid_idx == INT_NULL) {

        sid_idx = gfx_create_sid(sid);
        comp_group_id = obj_component_create_new_components_group(sid_idx);
    } else {

        comp_group_id = obj_component_group_find_match(sid_idx);
    }
    assert(comp_group_id >= 0);

    struct object* obj = object_create(sid_idx, MESH_PENTAGON);
    obj->num_faces = 1;
    obj->verts_per_face = VERTICES_PER_PENTAGON;
    obj->comp_id = comp_group_id;


    obj_component_set_obj_vertex_count(obj->comp_id, obj->num_faces * obj->verts_per_face);
    const uint32 obj_comp_idx = obj_component_create_new_obj(obj->comp_id, obj->num_faces * obj->verts_per_face, MESH_PENTAGON);

    const build_t build_idx = shader_create_input(sid_idx, "VPOS2", VEC2F, SID_VPOSI2_ID, IN_VSHADER | IS_ATTR  | IS_TRANSFORM_VPOS);
    obj_component_set_build_idx(comp_group_id, build_idx);

    // engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: PENTAGON, id: %u\n", obj->id);
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: %s, id:%u  comp_id:%u  num_faces:%2u  verts_per_face:%3u   attrib_count:%u | ",
            mesh_get_name_from_type(MESH_PENTAGON), obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);


    /* Create the necessary attribute descriptor and store the attributes data
    * Do this for all attributes */
    uint32 attrib_offset = 0;
    sid_t sid_attr = 0x0; /// attribute flag
    {
        shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS);
        sid_attr = SID_WPOSI3;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, wpos, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE);
        sid_attr = SID_SCALE2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, scale, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT);
        sid_attr = SID_ROTAT2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, rot, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_ID, IN_VSHADER | IS_ATTR | IS_OUT);
        sid_attr = SID_COLOR4;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, color, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }


    obj_component_set_stride(obj->comp_id, attrib_offset);

    shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_ID, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ);

    print("%s- Create object: pentagon -%s\n"
          "   id:%u\n   comp_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
    return obj;
}

struct object*
object_create_octagon(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
{
    sid.attr = sid.attr | SID_VPOSI2 | SID_SCALE2 | SID_COLOR4;
    sid.unif = sid.unif | SID_PROJMAT4X4;
    sid.out  = sid.out  | SID_COLOR4;
    sid.pass = sid.pass | SID_COLOR4;
    sid.oper = sid.oper | SID_NULL;

    sididx_t sid_idx = gfx_sid_find_match(sid);
    compid_t comp_group_id = INT_NULL;

    if(sid_idx == INT_NULL) {

        sid_idx = gfx_create_sid(sid);
        comp_group_id = obj_component_create_new_components_group(sid_idx);
    } else {

        comp_group_id = obj_component_group_find_match(sid_idx);
    }
    assert(comp_group_id >= 0);

    struct object* obj = object_create(sid_idx, MESH_OCTAGON);
    obj->num_faces = 1;
    obj->verts_per_face = VERTICES_PER_OCTAGON;
    obj->comp_id = comp_group_id;

    obj_component_set_obj_vertex_count(obj->comp_id, obj->num_faces * obj->verts_per_face);
    const uint32 obj_comp_idx = obj_component_create_new_obj(obj->comp_id, obj->num_faces * obj->verts_per_face, MESH_OCTAGON);

    const build_t build_idx = shader_create_input(sid_idx, "VPOS2", VEC2F, SID_VPOSI2_ID, IN_VSHADER | IS_ATTR  | IS_TRANSFORM_VPOS);
    obj_component_set_build_idx(comp_group_id, build_idx);

    // engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: OCTAGON, id: %u\n", obj->id);
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: %s, id:%u  comp_id:%u  num_faces:%2u  verts_per_face:%3u   attrib_count:%u | ",
            mesh_get_name_from_type(MESH_OCTAGON), obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);


    /* Create the necessary attribute descriptor and store the attributes data
    * Do this for all attributes */
    uint32 attrib_offset = 0;
    sid_t sid_attr = 0x0;

    {
        shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS);
        sid_attr = SID_WPOSI3;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, wpos, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE);
        sid_attr = SID_SCALE2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, scale, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT);
        sid_attr = SID_ROTAT2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, rot, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_ID, IN_VSHADER | IS_ATTR | IS_OUT);
        sid_attr = SID_COLOR4;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, color, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }


    obj_component_set_stride(obj->comp_id, attrib_offset);

    shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_ID, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ);

    print("%s- Create object: rectangle -%s\n"
          "   id:%u\n   comp_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
    return obj;
}

struct object*
object_create_cube(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
{
    sid.attr |= SID_VPOSI3 | SID_SCALE2 | SID_COLOR4;
    sid.unif |= SID_PROJMAT4X4;
    sid.out  |= SID_COLOR4;
    sid.pass |= SID_COLOR4;
    sid.oper |= SID_NULL;

    sididx_t sid_idx = gfx_sid_find_match(sid);
    compid_t comp_group_id = INT_NULL;

    if(sid_idx == INT_NULL) {

        sid_idx = gfx_create_sid(sid);
        comp_group_id = obj_component_create_new_components_group(sid_idx);
    } else {

        comp_group_id = obj_component_group_find_match(sid_idx);
    }
    assert(comp_group_id >= 0);

    struct object* obj = object_create(sid_idx, MESH_CUBE);
    obj->num_faces = FACES_PER_CUBE;
    obj->verts_per_face = VERTICES_PER_QUAD;
    obj->comp_id = comp_group_id;


    obj_component_set_obj_vertex_count(obj->comp_id, obj->num_faces * obj->verts_per_face);
    const uint32 obj_comp_idx = obj_component_create_new_obj(obj->comp_id, obj->num_faces * obj->verts_per_face, MESH_CUBE);

    const build_t build_idx = shader_create_input(sid_idx, "VPOS3", VEC3F, SID_VPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS);
    obj_component_set_build_idx(comp_group_id, build_idx);

    // engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: CUBE, id: %u\n", obj->id);
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: %s, id:%u  comp_id:%u  num_faces:%2u  verts_per_face:%3u   attrib_count:%u | ",
            mesh_get_name_from_type(MESH_CUBE), obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);


    /* Create the necessary attribute descriptor and store the attribute data
    * Do this for all attributes */
    uint32 attrib_offset = 0;
    sid_t sid_attr = 0x0;
    {
        shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS);
        sid_attr = SID_WPOSI3;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, wpos, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE);
        sid_attr = SID_SCALE2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, scale, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT);
        sid_attr = SID_ROTAT2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, rot, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_ID, IN_VSHADER | IS_ATTR | IS_OUT);
        sid_attr = SID_COLOR4;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, color, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }


    obj_component_set_stride(obj->comp_id, attrib_offset);

    shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_ID, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ);

    print("%s- Create object: cube -%s\n"
          "   id:%u\n   comp_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
    return obj;
}

struct object*
object_create_octahedron(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
{
    sid.attr |= SID_VPOSI3 | SID_SCALE2 | SID_COLOR4;
    sid.unif |= SID_PROJMAT4X4;
    sid.out  |= SID_COLOR4;
    sid.pass |= SID_COLOR4;
    sid.oper |= SID_NULL;

    sididx_t sid_idx = gfx_sid_find_match(sid);
    compid_t comp_group_id = INT_NULL;

    if(sid_idx == INT_NULL) {

        sid_idx = gfx_create_sid(sid);
        comp_group_id = obj_component_create_new_components_group(sid_idx);
    } else {

        comp_group_id = obj_component_group_find_match(sid_idx);
    }
    assert(comp_group_id >= 0);

    struct object* obj = object_create(sid_idx, MESH_OCTAHEDRON);
    obj->num_faces = FACES_PER_OCTAHEDRON;
    obj->verts_per_face = VERTICES_PER_TRI;
    obj->comp_id = comp_group_id;


    obj_component_set_obj_vertex_count(obj->comp_id, obj->num_faces * obj->verts_per_face);
    const uint32 obj_comp_idx = obj_component_create_new_obj(obj->comp_id, obj->num_faces * obj->verts_per_face, MESH_OCTAHEDRON);

    const build_t build_idx = shader_create_input(sid_idx, "VPOS3", VEC3F, SID_VPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS);
    obj_component_set_build_idx(comp_group_id, build_idx);

    // engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: OCTAHEDRON, id: %u\n", obj->id);
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: %s, id:%u  comp_id:%u  num_faces:%2u  verts_per_face:%3u   attrib_count:%u | ",
            mesh_get_name_from_type(MESH_OCTAHEDRON), obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);


    /* Create the necessary attribute descriptor and store the attribute data
    * Do this for all attributes */
    uint32 attrib_offset = 0;
    sid_t sid_attr = 0x0;
    {
        shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS);
        sid_attr = SID_WPOSI3;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, wpos, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE);
        sid_attr = SID_SCALE2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, scale, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT);
        sid_attr = SID_ROTAT2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, rot, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_ID, IN_VSHADER | IS_ATTR | IS_OUT);
        sid_attr = SID_COLOR4;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, color, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }


    obj_component_set_stride(obj->comp_id, attrib_offset);

    shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_ID, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ);

    print("%s- Create object: octahedron -%s\n"
          "   id:%u\n   comp_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
    return obj;
}

struct object*
object_create_dodecahedron(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
{
    sid.attr |= SID_VPOSI3 | SID_SCALE2 | SID_COLOR4;
    sid.unif |= SID_PROJMAT4X4;
    sid.out  |= SID_COLOR4;
    sid.pass |= SID_COLOR4;
    sid.oper |= SID_NULL;

    sididx_t sid_idx = gfx_sid_find_match(sid);
    compid_t comp_group_id = INT_NULL;

    if(sid_idx == INT_NULL) {

        sid_idx = gfx_create_sid(sid);
        comp_group_id = obj_component_create_new_components_group(sid_idx);
    } else {

        comp_group_id = obj_component_group_find_match(sid_idx);
    }
    assert(comp_group_id >= 0);

    struct object* obj  = object_create(sid_idx, MESH_DODECAHEDRON);
    obj->num_faces      = FACES_PER_DODECAHEDRON;
    obj->verts_per_face = VERTICES_PER_PENTAGON;
    obj->comp_id = comp_group_id;


    obj_component_set_obj_vertex_count(obj->comp_id, obj->num_faces * obj->verts_per_face);
    const uint32 obj_comp_idx = obj_component_create_new_obj(obj->comp_id, obj->num_faces * obj->verts_per_face, MESH_DODECAHEDRON);

    const build_t build_idx = shader_create_input(sid_idx, "VPOS3", VEC3F, SID_VPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS);;
    obj_component_set_build_idx(comp_group_id, build_idx);

    // engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: DODECAHEDRON, id: %u\n", obj->id);
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "--- CREATE OBJ: %s, id:%u  comp_id:%u  num_faces:%2u  verts_per_face:%3u   attrib_count:%u | ",
            mesh_get_name_from_type(MESH_DODECAHEDRON), obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);


    /* Create the necessary attribute descriptor and store the attribute data
    * Do this for all attributes */
    uint32 attrib_offset = 0;
    sid_t sid_attr = 0x0;
    {
        shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS);
        sid_attr = SID_WPOSI3;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, wpos, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE);
        sid_attr = SID_SCALE2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, scale, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_ID, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT);
        sid_attr = SID_ROTAT2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, rot, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_ID, IN_VSHADER | IS_ATTR | IS_OUT);
        sid_attr = SID_COLOR4;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        obj_component_add_comp(obj->comp_id, obj_comp_idx, color, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }


    obj_component_set_stride(obj->comp_id, attrib_offset);

    shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_ID, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ);

    print("%s- Create object: dodecahedron -%s\n"
          "   id:%u\n   comp_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
    return obj;
}
