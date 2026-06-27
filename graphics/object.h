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
    uint32 count;
};
struct object {
    // struct object_gfx gfx;
    sididx_t sid_idx;
    uint32 id;
    uint32 attrib_count;
    uint16 num_faces;
    uint16 comp_id;
    uint8  verts_per_face;
};


struct object* object_create(const sididx_t sid_idx);

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