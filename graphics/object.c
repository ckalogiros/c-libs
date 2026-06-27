/*
 * Created by ckalo on 6/11/26.
 *
 *
 * TODO: PASS INDEXED/!INDEXED GEOMETRY to SHAPE CREATION
 *
 */

#include "object.h"
#include <stdlib.h>
#include "mesh.h"
#include "shader_builder.h"
#include "test.h"



/// TODO: Use dynamic array for removing and reusing objects in the table
#define OBJECTS_TABLE_MAX_COUNT 128
struct object object_table[OBJECTS_TABLE_MAX_COUNT];
uint32 object_table_count = 0;
static uint32 object_id = 0;


struct object*
object_create(const sididx_t sid_idx)
{
    struct object obj = {
        .id         = object_id++,
        .sid_idx = sid_idx,
        .num_faces  = 0,
        .attrib_count   = 0,
        .comp_id   = 0,
        .verts_per_face = 0,
    };
    
    uint32 idx = object_table_count;
    object_table_count++;
    object_table[idx] = obj;
    
    return &object_table[idx];
}

/*
 * If an object is defined initially by the sid_attr(and the rest sids),
 * then we could automate the creation of all primitive shapes
 * with a for loop somehow.
 */
struct object*
object_create_triangle(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4])
{
    // sid.attr |= SID_VPOSI2 | SID_SCALE2 | SID_COLOR4 | SID_DIF;
    sid.attr |= SID_VPOSI2 | SID_SCALE2 | SID_COLOR4;
    sid.unif |= SID_PROJMAT4X4;
    sid.out  |= SID_COLOR4;
    sid.pass |= SID_COLOR4;
    sid.oper |= SID_NULL;

    const sididx_t sid_idx = gfx_create_sid(sid);

    struct object* obj = object_create(sid_idx);
    obj->num_faces = 1;
    obj->verts_per_face = VERTICES_PER_TRI;

    /* Create the necessary attribute descriptor and store the attributes data
    * Do this for all attributes */
    uint32 attrib_offset = 0;
    sid_t sid_attr = 0x0;

    uint32 comp_group_id = test_component_group_find_match(sid_idx);
    obj->comp_id = comp_group_id;
    test_set_obj_vertex_count(obj->comp_id, obj->num_faces * obj->verts_per_face);
    const uint32 obj_comp_idx = test_create_new_obj(obj->comp_id, obj->num_faces * obj->verts_per_face, MESH_TRIANGLE);


    const build_t build_idx = shader_create_input(sid_idx, "VPOS2", VEC2F, SID_VPOSI2_IDI, IN_VSHADER | IS_ATTR  | IS_TRANSFORM_VPOS);
    test_set_build_idx(comp_group_id, build_idx);

    {
        shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS);
        sid_attr = SID_WPOSI3;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, wpos, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE);
        sid_attr = SID_SCALE2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, scale, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT);
        sid_attr = SID_ROTAT2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, rot, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_IDI, IN_VSHADER | IS_ATTR | IS_OUT);
        sid_attr = SID_COLOR4;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, color, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }

    test_set_stride(obj->comp_id, attrib_offset);

    shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_IDI, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ);

    print("%s- Create object: triangle -%s\n"
          "   id:%u\n   comp_id:%u\n   num_faces:%u\n   verts_per_face:%u\n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

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

    const sididx_t sid_idx = gfx_create_sid(sid);

    struct object* obj = object_create(sid_idx);
    obj->num_faces = 1;
    obj->verts_per_face = VERTICES_PER_QUAD;

    /* Create the necessary attribute descriptor and store the attributes data
    * Do this for all attributes */
    uint32 attrib_offset = 0;
    sid_t sid_attr = 0x0; /// attribute flag

    uint32 comp_group_id = test_component_group_find_match(sid_idx);
    obj->comp_id = comp_group_id;
    test_set_obj_vertex_count(obj->comp_id, obj->num_faces * obj->verts_per_face);
    const uint32 obj_comp_idx = test_create_new_obj(obj->comp_id, obj->num_faces * obj->verts_per_face, MESH_RECTANGLE);

    const build_t build_idx = shader_create_input(sid_idx, "VPOS2", VEC2F, SID_VPOSI2_IDI, IN_VSHADER | IS_ATTR  | IS_TRANSFORM_VPOS);
    test_set_build_idx(comp_group_id, build_idx);

    {
        shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS);
        sid_attr = SID_WPOSI3;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, wpos, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE);
        sid_attr = SID_SCALE2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, scale, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT);
        sid_attr = SID_ROTAT2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, rot, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_IDI, IN_VSHADER | IS_ATTR | IS_OUT);
        sid_attr = SID_COLOR4;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, color, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }


    test_set_stride(obj->comp_id, attrib_offset);

    shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_IDI, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ);

    print("%s- Create object: rectangle -%s\n"
          "   id:%u\n   comp_id:%u\n   num_faces:%u\n   verts_per_face:%u\n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

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

    const sididx_t sid_idx = gfx_create_sid(sid);

    struct object* obj = object_create(sid_idx);
    obj->num_faces = 1;
    obj->verts_per_face = VERTICES_PER_PENTAGON;


    /* Create the necessary attribute descriptor and store the attributes data
    * Do this for all attributes */
    uint32 attrib_offset = 0;
    sid_t sid_attr = 0x0; /// attribute flag

    uint32 comp_group_id = test_component_group_find_match(sid_idx);
    obj->comp_id = comp_group_id;
    test_set_obj_vertex_count(obj->comp_id, obj->num_faces * obj->verts_per_face);
    const uint32 obj_comp_idx = test_create_new_obj(obj->comp_id, obj->num_faces * obj->verts_per_face, MESH_PENTAGON);

    const build_t build_idx = shader_create_input(sid_idx, "VPOS2", VEC2F, SID_VPOSI2_IDI, IN_VSHADER | IS_ATTR  | IS_TRANSFORM_VPOS);
    test_set_build_idx(comp_group_id, build_idx);

    {
        shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS);
        sid_attr = SID_WPOSI3;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, wpos, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE);
        sid_attr = SID_SCALE2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, scale, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT);
        sid_attr = SID_ROTAT2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, rot, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_IDI, IN_VSHADER | IS_ATTR | IS_OUT);
        sid_attr = SID_COLOR4;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, color, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }


    test_set_stride(obj->comp_id, attrib_offset);

    shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_IDI, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ);

    print("%s- Create object: pentagon -%s\n"
          "   id:%u\n   comp_id:%u\n   num_faces:%u\n   verts_per_face:%u\n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

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

    const sididx_t sid_idx = gfx_create_sid(sid);

    struct object* obj = object_create(sid_idx);
    obj->num_faces = 1;
    obj->verts_per_face = VERTICES_PER_OCTAGON;

    /* Create the necessary attribute descriptor and store the attributes data
    * Do this for all attributes */
    uint32 attrib_offset = 0;
    sid_t sid_attr = 0x0;

    uint32 comp_group_id = test_component_group_find_match(sid_idx);
    obj->comp_id = comp_group_id;
    test_set_obj_vertex_count(obj->comp_id, obj->num_faces * obj->verts_per_face);
    const uint32 obj_comp_idx = test_create_new_obj(obj->comp_id, obj->num_faces * obj->verts_per_face, MESH_OCTAGON);

    const build_t build_idx = shader_create_input(sid_idx, "VPOS2", VEC2F, SID_VPOSI2_IDI, IN_VSHADER | IS_ATTR  | IS_TRANSFORM_VPOS);
    test_set_build_idx(comp_group_id, build_idx);

    {
        shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS);
        sid_attr = SID_WPOSI3;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, wpos, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE);
        sid_attr = SID_SCALE2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, scale, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT);
        sid_attr = SID_ROTAT2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, rot, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_IDI, IN_VSHADER | IS_ATTR | IS_OUT);
        sid_attr = SID_COLOR4;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, color, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }


    test_set_stride(obj->comp_id, attrib_offset);

    shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_IDI, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ);

    print("%s- Create object: rectangle -%s\n"
          "   id:%u\n   comp_id:%u\n   num_faces:%u\n   verts_per_face:%u\n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

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

    const sididx_t sid_idx = gfx_create_sid(sid);

    struct object* obj = object_create(sid_idx);
    obj->num_faces = FACES_PER_CUBE;
    obj->verts_per_face = VERTICES_PER_QUAD;

    /* Create the necessary attribute descriptor and store the attribute data
    * Do this for all attributes */
    uint32 attrib_offset = 0;
    sid_t sid_attr = 0x0;

    uint32 comp_group_id = test_component_group_find_match(sid_idx);
    obj->comp_id = comp_group_id;
    test_set_obj_vertex_count(obj->comp_id, obj->num_faces * obj->verts_per_face);
    const uint32 obj_comp_idx = test_create_new_obj(obj->comp_id, obj->num_faces * obj->verts_per_face, MESH_CUBE);

    const build_t build_idx = shader_create_input(sid_idx, "VPOS3", VEC3F, SID_VPOSI3_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS);
    test_set_build_idx(comp_group_id, build_idx);

    {
        shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS);
        sid_attr = SID_WPOSI3;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, wpos, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE);
        sid_attr = SID_SCALE2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, scale, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT);
        sid_attr = SID_ROTAT2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, rot, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_IDI, IN_VSHADER | IS_ATTR | IS_OUT);
        sid_attr = SID_COLOR4;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, color, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }


    test_set_stride(obj->comp_id, attrib_offset);

    shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_IDI, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ);

    print("%s- Create object: cube -%s\n"
          "   id:%u\n   comp_id:%u\n   num_faces:%u\n   verts_per_face:%u\n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

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

    const sididx_t sid_idx = gfx_create_sid(sid);

    struct object* obj = object_create(sid_idx);
    obj->num_faces = FACES_PER_OCTAHEDRON;
    obj->verts_per_face = VERTICES_PER_TRI;

    /* Create the necessary attribute descriptor and store the attribute data
    * Do this for all attributes */
    uint32 attrib_offset = 0;
    sid_t sid_attr = 0x0;

    uint32 comp_group_id = test_component_group_find_match(sid_idx);
    obj->comp_id = comp_group_id;
    test_set_obj_vertex_count(obj->comp_id, obj->num_faces * obj->verts_per_face);
    const uint32 obj_comp_idx = test_create_new_obj(obj->comp_id, obj->num_faces * obj->verts_per_face, MESH_OCTAHEDRON);

    const build_t build_idx = shader_create_input(sid_idx, "VPOS3", VEC3F, SID_VPOSI3_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS);
    test_set_build_idx(comp_group_id, build_idx);

    {
        shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS);
        sid_attr = SID_WPOSI3;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, wpos, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE);
        sid_attr = SID_SCALE2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, scale, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT);
        sid_attr = SID_ROTAT2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, rot, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_IDI, IN_VSHADER | IS_ATTR | IS_OUT);
        sid_attr = SID_COLOR4;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, color, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }


    test_set_stride(obj->comp_id, attrib_offset);

    shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_IDI, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ);

    print("%s- Create object: octahedron -%s\n"
          "   id:%u\n   comp_id:%u\n   num_faces:%u\n   verts_per_face:%u\n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

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

    const sididx_t sid_idx = gfx_create_sid(sid);

    struct object* obj = object_create(sid_idx);
    obj->num_faces = FACES_PER_DODECAHEDRON;
    obj->verts_per_face = VERTICES_PER_PENTAGON;


    /* Create the necessary attribute descriptor and store the attribute data
    * Do this for all attributes */
    uint32 attrib_offset = 0;
    sid_t sid_attr = 0x0;

    uint32 comp_group_id = test_component_group_find_match(sid_idx);
    obj->comp_id = comp_group_id;
    test_set_obj_vertex_count(obj->comp_id, obj->num_faces * obj->verts_per_face);
    const uint32 obj_comp_idx = test_create_new_obj(obj->comp_id, obj->num_faces * obj->verts_per_face, MESH_DODECAHEDRON);

    const build_t build_idx = shader_create_input(sid_idx, "VPOS3", VEC3F, SID_VPOSI3_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS);;
    test_set_build_idx(comp_group_id, build_idx);

    {
        shader_create_input(sid_idx, "WPOS3", VEC3F, SID_WPOSI3_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS);
        sid_attr = SID_WPOSI3;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, wpos, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "SCALE2", VEC2F, SID_SCALE2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE);
        sid_attr = SID_SCALE2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, scale, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "ROT2", VEC2F, SID_ROTAT2_IDI, IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT);
        sid_attr = SID_ROTAT2;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, rot, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }
    {
        shader_create_input(sid_idx, "COL4", VEC4F, SID_COLOR4_IDI, IN_VSHADER | IS_ATTR | IS_OUT);
        sid_attr = SID_COLOR4;
        uint32 attrib_size = shaderinfo_flag_to_numelems(sid_attr);
        test_add_comp(obj->comp_id, obj_comp_idx, color, attrib_size, attrib_offset, sid_attr);
        attrib_offset += attrib_size;
        obj->attrib_count++;
    }


    test_set_stride(obj->comp_id, attrib_offset);

    shader_create_input(sid_idx, "PROJMAT4X4", MAT4X4F, SID_PROJMAT4X4_IDI, IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ);

    print("%s- Create object: dodecahedron -%s\n"
          "   id:%u\n   comp_id:%u\n   num_faces:%u\n   verts_per_face:%u\n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->comp_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

    return obj;
}
