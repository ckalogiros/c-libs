/*
 * Created by ckalo on 5/29/26.
 */

#include "attributes.h"
#include <assert.h>
#include "fdebug_test.h"
#include "gfx_buffers.h"
#include "logger.h"
#include "arena_allocator.h"
#include "my_macros.h"
#include "object.h"
#include "buffer_arr_internals.h"
#include "buffer_arr_struct.h"

/* This group mirrors the vertex buffers,
 * for each one vertex buffer there is one group.
 * The "data" stores the object's attributes (in elements type).
 * Each attribute is defined by "num_elems", that is the size of the attribute in elements type
 * and an offset that is the offset of the first float of a attributes in the data buffer.
 * attribute wpos3  :3 elems, offset:0
 * attribute scale2 :2 elems, offset:3
 * attribute col4   :4 elems, offset:5
 */



#define buffer_arr_attribs_get(attribs_id) (&((struct attributes_group*) attribsGroupDescr_groups_table->data)[attribs_id])
#define buffer_arr_obj_bufids_get(id) (&((objid_t*) attribs_group->obj_bufids->data)[id])
// #define buffer_arr_obj_bufids_get(id) (&((objid_t*) attribs_group->obj_bufids->data)[id]) /// TODO: to compine the 2 buffers we use the first buffer offset( sizeof(struct attribs_per_attrib_descriptor) * num_attributes)
// #define buffer_arr_obj_bufids_get(id) (&((objid_t*) attribs_group->obj_bufids->data)[id])


buffer_t* attribsGroupDescr_groups_table = NULL;


void
attribsGroupDescr_table_init(void)
{
    attribsGroupDescr_groups_table = buffer_arr_constructor_gentype(2, sizeof(struct attributes_group));
}

attrid_t attribsGroupDescr_create_new_components_group(const sididx_t sid_idx)
{
    /// init struct attribs_per_group_descriptor
    const attrid_t group_id = buffer_arr_set_for_push(attribsGroupDescr_groups_table);
    struct attributes_group* attribs_group = buffer_arr_attribs_get(group_id);
    attribs_group->attribs_data_id = arena_allocator_create_new_instance(6);

    /// init the mem group buffer that will store all objects id for a compomonent group
    attribs_group->obj_bufids = buffer_arr_create_gentype(16, sizeof(objid_t));

    attribs_group->sid_idx               = sid_idx;
    attribs_group->added_components_flag = 0x0;
    attribs_group->commands_flag         = 0x0;
    attribs_group->attribs_desc_counter  = 0;
    attribs_group->vertex_count          = 0;
    attribs_group->attribs_count         = 0;
    attribs_group->stride                = 0;
    attribs_group->build_idx = INT_NULL;
    attribs_group->ibidx     = INT_NULL;
    attribs_group->vaidx     = INT_NULL;
    attribs_group->vbidx     = INT_NULL;
    attribs_group->ibidx     = INT_NULL;
    attribs_group->tbidx     = INT_NULL;
    attribs_group->rqidx     = INT_NULL;

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "-- CREATE NEW COMPONENTS BUFFER, idx: %u\n", group_id);

    return group_id;
}

attrid_t attribsGroupDescr_group_find_match(const sididx_t sid_idx)
{
    for(attrid_t i = 0; i < attribsGroupDescr_groups_table->tail+1; i++) {
        const struct attributes_group* attribs_group = buffer_arr_attribs_get(i);
        if(sid_check_match(attribs_group->sid_idx, sid_idx)) { return i; }
    }

    return INT_NULL;
}

objid_t attribsGroupDescr_create_new_obj(const attrid_t group_id, const objid_t obj_id, const uint16 obj_vertex_count)
{
    struct attributes_group* attribs_group = buffer_arr_attribs_get(group_id);

    /// initialization: parameters for each object attributes
    attribs_group->vertex_count += obj_vertex_count;

    attribs_group->attribs_descr[attribs_group->attribs_desc_counter].attr_offset = 0;
    attribs_group->attribs_descr[attribs_group->attribs_desc_counter].attr_count++;
    assert(attribs_group->attribs_desc_counter < COMP_MAX_ATTRIBUTES);
    attribs_group->attribs_desc_counter++;

    ///
    objid_t idx = buffer_arr_set_for_push(attribs_group->obj_bufids); /// get the
    objid_t* buffer_ids = buffer_arr_obj_bufids_get(idx);
    *buffer_ids = obj_id;
    const objid_t obj_count = attribs_group->obj_bufids->count;
    return obj_count;
}

struct attributes_group* attribsGroupDescr_get_component(const attrid_t group_id)
{
    // struct attributes_group* group = &object_components_buffer[group_id];
    struct attributes_group* attribs_group = buffer_arr_attribs_get(group_id);
    return attribs_group;
}

void attribsGroupDescr_add_comp(const attrid_t group_id, const float* src, const uint32 elems_count, const uint32 offset, const sid_t attribs_flag)
{
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, " { %s }:[", shaderinfo_flag_to_flagname(attribs_flag));
    struct attributes_group* group = attribsGroupDescr_get_component(group_id);

    float* buffer_ptr = arena_allocator_alloc(group->attribs_data_id, elems_count*sizeof(float), sizeof(float));
    for(uint32 i = 0; i < elems_count; i++) {
        buffer_ptr[i] = src[i];
        print("  %2.2f, ", buffer_ptr[i]);
        engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "%2.2f,", buffer_ptr[i]);
    }
    print("\n");

    /// the "per_attr_desc" stores each unique attributes description (ONCE per attributes per group).
    /// this function is called for every object, but we need not to keep duplicates of attributes descriptions
    if((group->added_components_flag & attribs_flag) == 0x0) {
        const uint32 attribs_idx = group->attribs_count;
        assert(attribs_idx < COMP_MAX_GROUPS);
        group->added_components_flag |= attribs_flag;
        group->attribs_descr[attribs_idx].attr_offset = offset;
        group->attribs_descr[attribs_idx].attr_count  = elems_count;
        group->attribs_count++;
    }

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "]");
}


/* GETTERS-SETTERS */

void attribsGroupDescr_set_stride(const attrid_t group_id, const uint32 stride)
{
    struct attributes_group* attribs_group = buffer_arr_attribs_get(group_id);
    attribs_group->stride = stride;
}
void attribsGroupDescr_set_build_idx(const attrid_t group_id, const build_t build_idx)
{
    struct attributes_group* attribs_group = buffer_arr_attribs_get(group_id);
    attribs_group->build_idx = build_idx;
}
struct attributes_group* attribsGroupDescr_get_group(const attrid_t group_id)
{
    struct attributes_group* attribs_group = buffer_arr_attribs_get(group_id);
    return attribs_group;
}
buffer_t* attribsGroupDescr_get_objects_group(const attrid_t group_id)
{
    const struct attributes_group* attribs_group = buffer_arr_attribs_get(group_id);
    return attribs_group->obj_bufids;
}
build_t attribsGroupDescr_get_buildidx(const attrid_t group_id)
{
    const struct attributes_group* attribs_group = buffer_arr_attribs_get(group_id);
    return attribs_group->build_idx;
}
uint32 attribsGroupDescr_get_attribs_stride(const attrid_t group_id)
{
    const struct attributes_group* attribs_group = buffer_arr_attribs_get(group_id);
    return attribs_group->stride;
}
uint32 attribsGroupDescr_get_vertex_count(const attrid_t group_id)
{
    const struct attributes_group* attribs_group = buffer_arr_attribs_get(group_id);
    return attribs_group->vertex_count;
}
float* attribsGroupDescr_get_attribs_data(const attrid_t group_id)
{
    const struct attributes_group* attribs_group = buffer_arr_attribs_get(group_id);
    float* buffer_ptr = debug_arena_allocator_get_instance_data(attribs_group->attribs_data_id);
    return &buffer_ptr[0];
}
uint32 attribsGroupDescr_get_objects_count(const attrid_t group_id)
{
    const struct attributes_group* attribs_group = buffer_arr_attribs_get(group_id);
    return attribs_group->obj_bufids->count;
}
bool attribsGroupDescr_is_indexed_geometry(const attrid_t group_id)
{
    const struct attributes_group* attribs_group = buffer_arr_attribs_get(group_id);
    const sididx_t sid_idx = attribs_group->sid_idx;
    const bool res = sid_check_attr_match_from_flag(sid_idx, SID_INDEXED);
    return res;
}


/* DEBUG-PRINT */
void attribsGroupDescr_group_print()
{
    print("\n%s- OBJECT COMPONENTS GROUP -%s",  YELLOW, CLEAR);
    for(attrid_t group_id = 0; group_id < attribsGroupDescr_groups_table->tail; group_id++) {

        const struct attributes_group* attribs_group = attribsGroupDescr_get_component(group_id);


        print(
            "\n%s  obj_count:%u, "
            "  total_vertex_count:%u, "
            "  rqidx:%u %s", CYAN,
            attribs_group->obj_bufids->count,
            attribs_group->vertex_count,
            attribs_group->rqidx,
            CLEAR
        );

        for(uint32 attribs_idx = 0; attribs_idx < attribs_group->attribs_count; attribs_idx++) {

            print("\n  comp:[%u] - offset:%u, elems:%u", attribs_idx,
                  attribs_group->attribs_descr[attribs_idx].attr_offset,
                  attribs_group->attribs_descr[attribs_idx].attr_count
            );
        }
    }
    print("\n\n");
}
