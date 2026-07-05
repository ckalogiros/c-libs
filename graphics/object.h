/*
 * Created by ckalo on 6/11/26.
 */
#ifndef GRAPHICS_TEST_OBJECT_H
#define GRAPHICS_TEST_OBJECT_H

#include "my_types.h"
#include "engine.h"
#include "shader_id.h"
#include "mesh.h"


struct object {
    enum MESH_TYPE mesh_type; /// used by "gfx_vertex_buffer_set_data()" to get the object's correct vpos mesh
    objid_t id;
    uint32 attrib_count;
    uint16 vb_start;          /// the start index of an object, in the vertex buffer
    uint16 ib_start;          /// the start index of an object, in the vertex buffer
    uint16 num_faces;
    uint16 vertex_count;      /// number of object's vertices
    uint16 index_count;       /// number of object's vertex indices
    attrid_t attribs_id;
    sididx_t sid_idx;
    uint8  verts_per_face;

};

void object_table_init(void);

struct object* object_create(sididx_t sid_idx, enum MESH_TYPE mesh_type);

void object_destroy(objid_t obj_id);

struct object* object_get(objid_t obj_id);

uint16 object_get_vertex_count(objid_t obj_id);

vind_t object_get_vertex_index_count(objid_t obj_id);

void object_set_vertex_count(const objid_t obj_id, const uint16 vertex_count);

enum MESH_TYPE object_get_vertex_mesh_type(objid_t obj_id);

struct object*
object_create_triangle(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

struct object*
object_create_rectangle(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

struct object*
object_create_pentagon(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

struct object*
object_create_hexagon(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

struct object*
object_create_octagon(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

struct object*
object_create_cube(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

struct object*
object_create_octahedron(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

struct object*
object_create_dodecahedron(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

#endif //GRAPHICS_TEST_OBJECT_H