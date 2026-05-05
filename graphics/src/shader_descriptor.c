/*
 * Created by ckalo on 4/23/26.
 */

#include "shader_descriptor.h"

#include "logger.h"
#include "opengl.h"
#include "opengl_debug.h"
#include "shader_builder.h"
#include "slab_allocator_internals.h"
#include "sparse_arr_internals.h"


sparse_arr_t* shader_attributes_table = NULL;

struct vertex_layout*
shader_attributes_desc_get_byidx(const uint32 idx)
{
    struct vertex_layout* data = shader_attributes_table->data;
    return &data[idx];
}

uint32
shader_attributes_desc_get_count(void) { return shader_attributes_table->count; }

void
shader_attribute_descriptor_init()
{
    shader_attributes_table = sparse_arr_constructor_gentype(16, sizeof(struct vertex_layout));
}

uint32
shader_attributes_desc_set(struct vertex_layout attribs_desc[], const uint32 count)
{
    /// INIT table
    if(shader_attributes_table == NULL)
        shader_attribute_descriptor_init();


    struct vertex_layout* attribs = shader_attributes_table->data;
    /// TODO: typedef the sparse array's element index
    const uint16 idx = sparse_arr_set_for_push(shader_attributes_table);

    for(uint32 i = 0; i < count; i++) {
        if(attribs_desc[i].size > 0) {
            attribs[i].idx = attribs_desc[i].idx;
            attribs[i].loc = attribs_desc[i].loc;
            attribs[i].type = attribs_desc[i].type;
            attribs[i].offset = attribs_desc[i].offset;
            attribs[i].size = attribs_desc[i].size;
            attribs[i].type = attribs_desc[i].type;
        }
    }



    print("--- %svertex_layout%s ---\n", ry, CLEAR);
    for(uint32 i = 0; i < count; i++) {
        if(attribs_desc[i].size > 0) {
            print("attr:%2u", attribs_desc[i].idx);
            print(" loc: %2u", attribs[i].loc);
            print(" offset %2u", attribs[i].offset);
            print(" size %2u", attribs[i].size);
            print(" type: %s", shaderinfo_type_to_glsltype_string(attribs[i].type));
        }
        print("\n");
    }
    print("\n");

    return idx;
}

void
shader_attributes_enable(const uint32 locations_count, const uint32 stride, const uint8 last_loc_num_elems)
{
    assert(last_loc_num_elems>0);
    assert(last_loc_num_elems<=4);
    uint32 k = 0;
    uint32 loc = 0;

    for(; loc < locations_count - 1; loc++) {
        gl_enable_vertex_attrib_array(loc);
        GL_CHECK();
        gl_enable_attrib_pointer(loc, 4, stride * SIZE_F32, (k * SIZE_F32));
        // gl_enable_attrib_pointer(loc, 4, stride * SIZE_F32, (void*) (k * SIZE_F32));
        GL_CHECK();
        gl_vertex_attrib_divisor(loc, 0);
        GL_CHECK();

        k += 4;
    }

    gl_enable_vertex_attrib_array(loc);
    gl_enable_attrib_pointer(loc, last_loc_num_elems, stride * SIZE_F32, (k * SIZE_F32));
    // gl_enable_attrib_pointer(loc, last_loc_num_elems, stride * SIZE_F32, (void*) (k * SIZE_F32));
    gl_vertex_attrib_divisor(loc, 0);
}
