/*
 * Created by ckalo on 6/11/26.
 */
#ifndef GRAPHICS_TEST_OBJECT_H
#define GRAPHICS_TEST_OBJECT_H
#include "gfx_interface.h"
#include "my_types.h"
#include "shader_id.h"

struct object_gfx {
    uint32 start;
};
struct object_attrib {
    sid_t sid_attr;         // SID_WPOSI3, SID_SCALE2, etc.
    uint32 offset;          // offset into object's data array
    uint32 count;           // number of elements
};
struct object {
    uint32 id;
    uint16 num_faces;
    uint16 comp_id;
    uint32 attrib_count;
    struct gfx_sid sid;
    struct object_attrib* attribs_arr_list;  // array of present attributes only
    // Optional: quick access pointers for CPU-side updates
    void* attrib_data;         // mapped copy of GPU buffer range
    uint8 verts_per_face;

    int gfx; /// TODO: struct gfx = {start, count, vbid, ...} OR index to array buffer
};


struct object* object_create(struct gfx_sid sid);

struct object*
object_create_triangle(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

struct object*
object_create_rectangle(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

struct object*
object_create_pentagon(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

struct object*
object_create_octagon(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

struct object*
object_create_cube(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

struct object*
object_create_octahedron(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

struct object*
object_create_dodecahedron(struct gfx_sid sid, float wpos[3], float scale[2], float rot[2], float color[4]);

#endif //GRAPHICS_TEST_OBJECT_H