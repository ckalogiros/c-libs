/*
 * Created by ckalo on 4/23/26.
 */

#include "shader_descriptor.h"

#include "logger.h"
#include "slab_allocator_internals.h"
#include "sparse_arr_internals.h"

// slab_arr_id slab_inst_id = INT_NULL;

sparse_arr_t* shader_attributes_desc_table = NULL;
// uint32 shader_attributes_desc_table_count = 0;
// sparse_arr_t* buffer = NULL;

struct vertex_layout* shader_attributes_desc_get_byidx(const uint32 idx)
{
    struct vertex_layout* data = shader_attributes_desc_table->data;
    return &data[idx];
}

uint32 shader_attributes_desc_get_count() { return shader_attributes_desc_table->count; }

void shader_attribute_descriptor_init()
{
    shader_attributes_desc_table = sparse_arr_constructor_gentype(8, sizeof(struct vertex_layout));
}

uint32 shader_attributes_desc_set(struct vertex_layout attribs_desc[], const uint32 count)
{
    /// INIT table
    if(shader_attributes_desc_table == NULL)
        shader_attribute_descriptor_init();


    struct vertex_layout* attribs = shader_attributes_desc_table->data;
    /// TODO: typedef the sparse array's element index
    const uint16 idx = sparse_arr_set_for_push(shader_attributes_desc_table);

    for(uint32 i = 0; i < count; i++) {
        if(attribs_desc[i].size > 0) {
            attribs[i].idx     = attribs_desc[i].idx;
            attribs[i].loc     = attribs_desc[i].loc;
            attribs[i].type    = attribs_desc[i].type;
            attribs[i].offset  = attribs_desc[i].offset;
            attribs[i].size    = attribs_desc[i].size;
            attribs[i].type    = attribs_desc[i].type;
        }

    }
    for(uint32 i = 0; i < count; i++) {
        if(attribs_desc[i].size > 0) {
            print("%2u", attribs_desc[i].idx);
            print(" %2u", attribs[i].loc);
            print(" %2u", attribs[i].type);
            print(" %2u", attribs[i].offset);
            print(" %2u", attribs[i].size);
            print(" %2u\n", attribs[i].type);
        }
    }

    return idx;

}

void shader_attributes_enable(const uint32 idx)
{
    // struct generic_location* attribs = shader_attributes_desc_table[idx];
    // 'loc', location,
    // 'size', size,
    // 'offset', offset,
    // 'stride', stride,
    // 'divisor', attribs_divisor,
    // 'vbidx', vbidx,

    // for (let i = start_loc; i < stop_loc; i++)
    // const location = vb_attribs[i].locs;
    // const size = vb_attribs[i].sizes;
    // const offset = vb_attribs[i].offsets;
    //
    // gfxCtx.gl.vertexAttribPointer(location, size, gl_type, false, stride * FLOAT32_BYTE_SIZE, offset * FLOAT32_BYTE_SIZE);
    // gfxCtx.gl.vertexAttribDivisor(location, attribs_divisor);
    // gfxCtx.gl.enableVertexAttribArray(location);
}