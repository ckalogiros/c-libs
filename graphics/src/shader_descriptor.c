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
#include "buffer_arr_internals.h"
#include "fdebug_test.h"
#include "string_format.h"


buffer_t* shader_descriptor_table = NULL;


#define s_desc_get(desc_idx) (&((struct shader_descriptor*) shader_descriptor_table->data)[desc_idx])

void
shader_descriptor_init(void)
{
    shader_descriptor_table = buffer_arr_constructor_gentype(80, sizeof(struct shader_descriptor));
}

build_t
shader_descriptor_create(const sididx_t sid_idx)
{
    const desc_t desc_idx = buffer_arr_set_for_push(shader_descriptor_table);
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
        struct shader_entry* entry = &desc->entries[i];
        entry->flagname = NULL;
        entry->codename = NULL;
        entry->flag = 0x0;
        entry->type = 0; // TODO: should create "enum TYPES_INDEXING NULL_TYPE "
        entry->input_flag = 0x0;
        entry->typesize = 0;
        entry->elems = 0;
        entry->input_idx = 0; // TODO: should have NIL/NULL value
        entry->shader_input_id = 0; // TODO: should have NIL/NULL value

        struct shader_attribute_layout* vl = &desc->v_layout[i];
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
    for(uint32 j = 0; j < SHADER_INPUT_ENTRIES_MAX_COUNT; j++) {
        ids->ids[j] = INT_NULL;
    }

    return desc_idx;
}

void
shader_descriptor_create_vertex_layout(const desc_t desc_idx, struct shader_attribute_layout vlayout)
{
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    const uint32 idx = desc->vlayout_count;
    desc->vlayout_count++;
    assert(desc->vlayout_count < SHADER_INPUT_ENTRIES_MAX_COUNT);

    struct shader_attribute_layout* vl = &desc->v_layout[idx];
    vl->loc         = vlayout.loc;
    vl->size        = vlayout.size;
    vl->type        = vlayout.type;
    vl->loc_offset  = vlayout.loc_offset;
    vl->offset      = vlayout.offset;
    vl->idx         = vlayout.idx;

    print("# CREATE VLAYOUT[%u]: loc:%u, loc_offset:%d, offset:%d, size:%u\n", idx, vlayout.loc, vlayout.loc_offset, vlayout.offset, vlayout.size);
    char buf[64] = {0};
    char* ptr = &buf[0];
    uint32 size = string_format(&ptr, 64, "[%u]: loc:%u, loc_offset:%d, offset:%d, size:%u\n",
                                idx, vlayout.loc, vlayout.loc_offset, vlayout.offset, vlayout.size);
    engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf, size);

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

struct shader_attribute_layout*
shader_descriptor_vertex_layout_get(const desc_t desc_idx, const vlayout_t vlayout_idx)
{
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    struct shader_attribute_layout* vl = &desc->v_layout[vlayout_idx];
    return vl;
}

attr_t
shader_attrib_id_get(const desc_t desc_idx, const enum SID_ID idx)
{
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    attr_t attrib_id = desc->vertex_attribute_ids.ids[idx];
    return attrib_id;
}

attr_t
shader_attrib_get_offset(const desc_t desc_idx, const enum SID_ID idx)
{
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    attr_t attrib_id = desc->vertex_attribute_ids.ids[idx];
    attr_t voffset = desc->v_layout[attrib_id].offset;
    return voffset;
}

attr_t
shader_attrib_get_stride(const desc_t desc_idx)
{
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    attr_t stride = desc->vshader_attribs_stride;
    return stride;
}

sididx_t
shader_dscriptor_get_sid(const desc_t desc_idx)
{
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    sididx_t sid_idx = desc->sid_idx;
    return sid_idx;
}

void
shader_descriptor_set_attribute_ids(const desc_t desc_idx, const attr_t attrib_id, const uint32 at_index)
{
    if(shader_descriptor_table == NULL) assert(1);
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    struct vertex_attribute_ids* buffer = &desc->vertex_attribute_ids;
    buffer->ids[attrib_id] = at_index;
    buffer->count++;
    // print("   attribute_id:%u at index:%u\n", attrib_id, at_index);
}

void
// shader_attributes_enable(const uint32 locations_count, const uint32 stride, const attr_t last_loc_num_elems)
shader_attributes_enable(const struct shader_descriptor* desc)
{
    assert(desc->last_loc_num_elems>0);
    assert(desc->last_loc_num_elems <= SHADER_MAX_LOCATION_SIZE);

    for(uint32 i = 0; i < desc->vshader_attrib_locations_count; i++) {

        const struct shader_attribute_layout* vlayout = &desc->v_layout[i];

        gl_enable_vertex_attrib_array(vlayout->loc);
        GL_CHECK();
        gl_enable_attrib_pointer(vlayout->loc, vlayout->size, desc->vshader_attribs_stride * SIZE_F32, vlayout->offset * SIZE_F32);
        GL_CHECK();
        gl_vertex_attrib_divisor(vlayout->loc, SHADER_DEVISOR_PER_VERTEX);
        GL_CHECK();

    }

    // gl_enable_vertex_attrib_array(vlayout->loc);
    // gl_enable_attrib_pointer(vlayout->loc, desc->last_loc_num_elems, desc->vshader_attribs_stride * SIZE_F32, vlayout->offset);
    // gl_vertex_attrib_divisor(vlayout->loc, SHADER_DEVISOR_PER_VERTEX);
}

void
shader_attributes_instanced_enable(const uint32 locations_count, const uint32 stride, const attr_t last_loc_num_elems)
{

    /// LOCATIONS REMAIN PER SHADER PROGRAM AND IN SEQUENCE
    /// STRIDE IS PER VERTEX BUFFER
    /// OFFSET IS PER BUFFER
    ///
    // glEnableVertexAttribArray(2);
    // glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE, (void*)0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute

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

    if(last_loc_num_elems) {
        gl_enable_vertex_attrib_array(loc);
        gl_enable_attrib_pointer(loc, last_loc_num_elems, stride * SIZE_F32, (k * SHADER_MAX_LOCATION_SIZE));
        gl_vertex_attrib_divisor(loc, SHADER_DEVISOR_PER_VERTEX);
    }

}


/* debug-print */

void
shader_descriptor_print_attribute_ids(const desc_t desc_idx, const uint32 vid)
{
    struct shader_descriptor* desc = s_desc_get(desc_idx);
    int8 id = desc->vertex_attribute_ids.ids[vid];
    print("   vertex_attribute_id[%u]: %u\n", vid, id);
}

void
shader_descriptor_print_vertex_layout(const desc_t desc_idx, const uint32 count)
{
    assert(shader_descriptor_table != NULL);

    struct shader_descriptor* desc = s_desc_get(desc_idx);
    struct shader_attribute_layout* v_layout = desc->v_layout;

    print("%s--- Shader descriptor vertex layout ---%s\n", ry, CLEAR);
    for(uint32 i = 0; i < count; i++) {
        if(v_layout[i].size > 0) {
            print("| attr:%2u", v_layout[i].idx);
            print("| type:%s", shaderinfo_type_to_glsltype_string(v_layout[i].type));
            print("| loc:%2u", v_layout[i].loc);
            print("| offset:%2u", v_layout[i].offset);
            print("| loc_offset:%2u", v_layout[i].loc_offset);
            print("| size:%2u", v_layout[i].size);
        }
        print("\n");
    }
    print("\n");
}