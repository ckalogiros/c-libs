/*
 * Created by ckalo on 4/23/26.
 */

#include "shader_descriptor.h"

#include <assert.h>

#include "logger.h"
#include "opengl.h"
#include "opengl_debug.h"
#include "shader_builder.h"
#include "slab_allocator_internals.h"
#include "sparse_arr_internals.h"



sparse_arr_t* shader_descriptor_table = NULL;


#define s_desc_get(desc_idx) (&((struct shader_descriptor*) shader_descriptor_table->data)[desc_idx])

void
shader_descriptor_init(void)
{
    shader_descriptor_table = sparse_arr_constructor_gentype(80, sizeof(struct shader_descriptor));
}

build_t
shader_descriptor_create(const sididx_t sid_idx)
{
    const desc_t desc_idx = sparse_arr_set_for_push(shader_descriptor_table);
    struct shader_descriptor* desc = s_desc_get(desc_idx);

    desc->sid_idx = sid_idx;
    desc->added_entry_flags_renameme = 0x0;
    desc->vshader_code = NULL;
    desc->vshader_attribs_idx = INT_NULL;
    desc->vshader_code_size = 0;
    desc->vshader_attribs_count = 0;
    desc->vshader_attribs_stride = 0;
    desc->vshader_attrib_locations_count = 0;
    desc->last_loc_num_elems = 0;
    desc->entry_count = 0;
    desc->vlayout_count = 0;
    desc->is_set = FALSE;

    /// init vertex_layout
    for(uint32 i = 0; i < SHADER_INPUT_ENTRIES_MAX_COUNT; i++) {
        struct shader_in_variables_struct* entry = &desc->entries[i];
        entry->flagname = NULL;
        entry->codename = NULL;
        entry->flag = 0x0;
        entry->type = 0; // TODO: should create "enum TYPES_INDEXING NULL_TYPE "
        entry->which_renameme = 0x0;
        entry->typesize = 0;
        entry->elems = 0;
        entry->var_table_idx = 0; // TODO: should have NIL/NULL value
        entry->shader_var_id = 0; // TODO: should have NIL/NULL value

        struct vertex_layout* vl = &desc->v_layout[i];
        vl->loc = 0;
        vl->size = 0;
        vl->type = 0;
        vl->loc_offset = 0;
        vl->idx = 0;
        vl->attr_idx = 0;
    }

    /// init vertex_attribute_ids buffer
    struct vertex_attribute_ids* ids = &desc->vertex_attribute_ids;
    ids->count = 0;
    for(uint32 j = 0; j < 8; j++) {
        ids->ids[j] = 0;
    }

    return desc_idx;
}

void
shader_descriptor_create_vertex_layout(const desc_t desc_idx, struct vertex_layout attribs_desc)
{
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    const uint32 idx = desc->vlayout_count;
    desc->vlayout_count++;
    assert(desc->vlayout_count < SHADER_INPUT_ENTRIES_MAX_COUNT);

    struct vertex_layout* vl = &desc->v_layout[idx];
    vl->loc         = attribs_desc.idx;
    vl->size        = attribs_desc.size;
    vl->type        = attribs_desc.type;
    vl->loc_offset  = attribs_desc.loc_offset;
    vl->offset      = attribs_desc.offset;
    vl->idx         = attribs_desc.idx;
    vl->attr_idx    = attribs_desc.attr_idx;
}

uint32
shader_descriptor_get_count(void) { return shader_descriptor_table->count; }


struct shader_descriptor*
shader_descriptor_get(const desc_t idx)
{
    assert(shader_descriptor_table != NULL);
    assert(shader_descriptor_table->data != NULL);
    struct shader_descriptor* desc = &((struct shader_descriptor*) shader_descriptor_table->data)[idx];
    assert(desc != NULL);
    return desc;
}

struct vertex_layout*
shader_descriptor_vertex_layout_get(const desc_t desc_idx, const vlayout_t vlayout_idx)
{
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    struct vertex_layout* vl = &desc->v_layout[vlayout_idx];
    return vl;
}

uint8
shader_attrib_id_get(const desc_t desc_idx, const enum SID_ID idx)
{
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    uint8 attrib_id = desc->vertex_attribute_ids.ids[idx];
    return attrib_id;
}

uint8
shader_attrib_get_offset(const desc_t desc_idx, const enum SID_ID idx)
{
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    uint8 attrib_id = desc->vertex_attribute_ids.ids[idx];
    uint8 voffset = desc->v_layout[attrib_id].offset;
    return voffset;
}

uint8
shader_attrib_get_stride(const desc_t desc_idx)
{
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    uint8 stride = desc->vshader_attribs_stride;
    return stride;
}

sididx_t
shader_dscriptor_get_sid(const desc_t desc_idx)
{
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    sididx_t sid_idx = desc->sid_idx;
    return sid_idx;
}

uint32
shader_descriptor_vertex_layout_set(const desc_t desc_idx, struct vertex_layout attribs_desc[], const uint32 count)
{
    assert(shader_descriptor_table != NULL);

    struct shader_descriptor* desc = s_desc_get(desc_idx);
    struct vertex_layout* attribs = desc->v_layout;
    /// TODO: typedef the sparse array's element index
    const desc_t idx = desc_idx;

    for(uint32 i = 0; i < count; i++) {
        if(attribs_desc[i].size > 0) {
            attribs[i].idx = attribs_desc[i].idx;
            attribs[i].loc = attribs_desc[i].loc;
            attribs[i].type = attribs_desc[i].type;
            attribs[i].loc_offset = attribs_desc[i].loc_offset;
            attribs[i].offset = attribs_desc[i].offset;
            attribs[i].size = attribs_desc[i].size;
            attribs[i].type = attribs_desc[i].type;
            attribs[i].attr_idx = 0;
        }
    }

    print("%s--- Shader descriptor. set vertex layout ---%s\n", ry, CLEAR);
    for(uint32 i = 0; i < count; i++) {
        if(attribs_desc[i].size > 0) {
            print("   attr:%2u", attribs_desc[i].idx);
            print(" type: %s", shaderinfo_type_to_glsltype_string(attribs[i].type));
            print(" loc: %2u", attribs[i].loc);
            print(" offset %2u", attribs[i].offset);
            print(" loc_offset %2u", attribs[i].loc_offset);
            print(" size %2u", attribs[i].size);
        }
        print("\n");
    }
    print("\n");

    return idx;
}

void
shader_descriptor_set_attribute_ids(const desc_t desc_idx, const uint32 attrib_id, const uint32 at_index)
{
    if(shader_descriptor_table == NULL) assert(1);
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    struct vertex_attribute_ids* buffer = &desc->vertex_attribute_ids;
    buffer->ids[attrib_id] = at_index;
    buffer->count++;
    print("   attribute_id:%u at index:%u\n", attrib_id, at_index);
}

void
shader_attributes_enable(const uint32 locations_count, const uint32 stride, const uint8 last_loc_num_elems)
{
    assert(last_loc_num_elems>0);
    assert(last_loc_num_elems <= SHADER_MAX_LOCATION_SIZE);
    uint32 k = 0;
    uint32 loc = 0;

    for(; loc < locations_count - 1; loc++) {
        gl_enable_vertex_attrib_array(loc);
        GL_CHECK();
        gl_enable_attrib_pointer(loc, SHADER_MAX_LOCATION_SIZE, stride * SIZE_F32, (k * SHADER_MAX_LOCATION_SIZE));
        GL_CHECK();
        gl_vertex_attrib_divisor(loc, SHADER_DEVISOR_PER_VERTEX);
        GL_CHECK();

        k += SHADER_MAX_LOCATION_SIZE;
    }

    gl_enable_vertex_attrib_array(loc);
    gl_enable_attrib_pointer(loc, last_loc_num_elems, stride * SIZE_F32, (k * SHADER_MAX_LOCATION_SIZE));
    gl_vertex_attrib_divisor(loc, SHADER_DEVISOR_PER_VERTEX);
}


/* debug-print */

void
shader_descriptor_print_attribute_ids(const desc_t desc_idx, const uint32 vid)
{
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    uint8 id = desc->vertex_attribute_ids.ids[vid];
    print("   vertex_attribute_id[%u]: %u\n", vid, id);
}
