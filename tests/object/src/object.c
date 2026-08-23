/*
 * Created by ckalo on 8/20/26.
 */

#include "object.h"


static uint32 object_id = 0;

struct object* object_table[1];

struct object*
// object_create(const sididx_t sid_idx, const enum MESH_TYPE mesh_type)
object_create(const sididx_t sid_idx, const enum MESH_TYPE mesh_type)
{
    const struct object obj = {
        .id = object_id++,
        // .sid_idx = sid_idx,
        // .num_faces = 0,
        // .attrib_count = 0,
        // .attribs_group_id = 0,
        // .verts_per_face = 0,
        // .mesh_type = mesh_type,
        // .vb_start = 0,
        // .vertex_count = 0,
        // .index_count = 0,
    };

    const uint16 obj_idx = buffer_arr_set_for_push(object_table);
    struct object* object = obj_get(obj_idx);
    *object = obj;

    return object;
}