/*
 * Created by ckalo on 5/29/26.
 */

#include "test.h"

#include <assert.h>

#include "fdebug_test.h"
#include "gfx_buffers.h"
#include "logger.h"
#include "mesh.h"
#include "my_macros.h"

/* This group mirrors the vertex buffers,
 * for each one vertex buffer there is one group.
 * The "data" stores the object's components or attributes (in elements type).
 * Each component is defined by "num_elems", that is the size of the component in elements type
 * and an offset that is the offset of the first float of a component in the data buffer.
 * component wpos3  :3 elems, offset:0
 * component scale2 :2 elems, offset:3
 * component col4   :4 elems, offset:5
 */



struct components_group object_components_buffer[COMP_MAX_GROUPS];
uint32 index_count = 0;


uint32 test_component_group_find_match(const sididx_t sid_idx)
{
    const uint32 count = index_count;
    for(build_t i = 0; i < count; i++) {
        struct comp_per_group_descriptor comp_desc = object_components_buffer[i].comp_desc;
        if(sid_check_match(comp_desc.sid_idx, sid_idx)) { return i; }
    }

    uint32 group_id = test_create_new_components_buffer(sid_idx);

    return group_id;
}

uint32 test_create_new_components_buffer(const sididx_t sid_idx)
{
    uint32 idx = index_count;
    index_count++;

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "-- CREATE NEW COMPONENTS BUFFER, idx: %u\n", idx);


    /// init struct comp_per_group_descriptor
    struct comp_per_group_descriptor* comp_desc = &object_components_buffer[idx].comp_desc;
    comp_desc->sid_idx = sid_idx;
    comp_desc->added_components_flag = 0x0;
    comp_desc->commands_flag         = 0x0;
    comp_desc->per_attr_desc_counter = 0;
    comp_desc->per_obj_desc_counter  = 0;
    comp_desc->obj_count             = 0;
    comp_desc->total_vertex_count    = 0;
    comp_desc->obj_vertex_count      = 0;
    comp_desc->components_count      = 0;
    comp_desc->stride                = 0;
    comp_desc->build_idx = INT_NULL;
    comp_desc->ibidx     = INT_NULL;
    comp_desc->vaidx     = INT_NULL;
    comp_desc->vbidx     = INT_NULL;
    comp_desc->ibidx     = INT_NULL;
    comp_desc->tbidx     = INT_NULL;
    comp_desc->rqidx     = INT_NULL;

    return idx;
}
uint32 test_create_new_obj(const vbidx_t group_id, const uint16 obj_vertex_count, enum MESH_TYPE mesh_type)
{
    struct components_group* group = &object_components_buffer[group_id];
    const uint32 obj_comp_idx = group->comp_desc.obj_count;
    group->comp_desc.obj_count++;


    /// initialization: parameters for each object components
    struct comp_per_group_descriptor* desc = &group->comp_desc; // cache
    desc->total_vertex_count += obj_vertex_count;

    desc->per_obj_desc[desc->per_obj_desc_counter].mesh_type    = mesh_type;
    desc->per_obj_desc[desc->per_obj_desc_counter].vb_start     = 0;
    desc->per_obj_desc[desc->per_obj_desc_counter].vertex_count = obj_vertex_count;
    assert(desc->per_obj_desc_counter < COMP_MAX_OBJECTS);
    desc->per_obj_desc_counter++;

    desc->per_attr_desc[desc->per_attr_desc_counter].attr_offset = 0;
    desc->per_attr_desc[desc->per_attr_desc_counter].attr_count++;
    assert(desc->per_attr_desc_counter < COMP_MAX_ATTRIBUTES);
    desc->per_attr_desc_counter++;

    return obj_comp_idx;
}

struct components_group* test_get_component(const vbidx_t group_id)
{
    struct components_group* group = &object_components_buffer[group_id];
    return group;
}

// HERE: The object for the first time a group is created must provide the desriptor for that group ONCE
void test_add_comp(const vbidx_t group_id, const uint32 obj_idx, float* src, const uint32 elems_count, const uint32 offset, const sid_t comp_flag)
{
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, " { %s }:[", shaderinfo_flag_to_flagname(comp_flag));
    struct components_group* group = test_get_component(group_id);
    const uint32 start = obj_idx * group->comp_desc.stride;
    float* buffer = &group->data[start];
    uint32 bidx = offset;
    for(uint32 i = 0; i < elems_count; i++) {
        buffer[bidx++] = src[i];
        print("  %2.2f, ", buffer[bidx-1]);
        engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "%2.2f,", buffer[bidx-1]);
    }
    print("\n");
    // engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");

    /// the "per_attr_desc" stores each unique component description (ONCE per component per group).
    /// this function is called for every object, but we need not to keep duplicates of component descriptions
    if((group->comp_desc.added_components_flag & comp_flag) == 0x0) {
        const uint32 comp_idx = group->comp_desc.components_count;
        assert(comp_idx < COMP_MAX_GROUPS);
        group->comp_desc.added_components_flag |= comp_flag;
        group->comp_desc.per_attr_desc[comp_idx].attr_offset = offset;
        group->comp_desc.per_attr_desc[comp_idx].attr_count  = elems_count;
        group->comp_desc.components_count++;
    }

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "]");
}


/* GETTERS-SETTERS */

void test_set_stride(const vbidx_t group_id, uint32 stride)
{
    struct components_group* group = &object_components_buffer[group_id];
    group->comp_desc.stride = stride;
}
build_t test_set_build_idx(const vbidx_t group_id, const build_t build_idx)
{
    object_components_buffer[group_id].comp_desc.build_idx = build_idx;
}
void test_set_obj_vertex_count(const vbidx_t group_id, const uint32 obj_vertex_count)
{
    struct components_group* group = &object_components_buffer[group_id];
    group->comp_desc.obj_vertex_count = obj_vertex_count;
}

struct comp_per_group_descriptor* test_get_comp_desc(const vbidx_t group_id)
{
    return &object_components_buffer[group_id].comp_desc;
}
build_t test_get_build_idx(const vbidx_t group_id)
{
    return object_components_buffer[group_id].comp_desc.build_idx;
}
uint32 test_obj_comp_desc_get_stride(const vbidx_t group_id)
{
    return object_components_buffer[group_id].comp_desc.stride;
}
uint32 test_obj_comp_desc_get_total_vertex_count(const vbidx_t group_id)
{
    return object_components_buffer[group_id].comp_desc.total_vertex_count;
}
uint32 test_obj_comp_desc_get_total_object_count(const vbidx_t group_id)
{
    return object_components_buffer[group_id].comp_desc.obj_count;
}
float* test_get_obj_attribs_data(const vbidx_t group_id)
{
    return &object_components_buffer[group_id].data[0];
}
uint32 test_get_obj_vertex_count(const vbidx_t group_id)
{
    return object_components_buffer[group_id].comp_desc.obj_vertex_count;
}
uint32 test_get_total_vertex_count(const vbidx_t group_id)
{
    return object_components_buffer[group_id].comp_desc.total_vertex_count;
}


/* DEBUG-PRINT */
void obj_comp_group_print()
{
    print("\n%s- OBJECT COMPONENTS GROUP -%s",  YELLOW, CLEAR);
    for(uint32 group_id = 0; group_id < index_count; group_id++) {

        struct components_group* group = test_get_component(group_id);


        print(
              "\n%s  obj_count:%u, "
              "  obj_vertex_count:%u, "
              "  total_vertex_count:%u, "
              "  rqidx:%u %s", CYAN,
            group->comp_desc.obj_count,
            group->comp_desc.obj_vertex_count,
            group->comp_desc.total_vertex_count,
            group->comp_desc.rqidx,
            CLEAR
            );

        uint32 offset = 0;
        for(uint32 comp_idx = 0; comp_idx < group->comp_desc.components_count; comp_idx++) {

            print("\n  comp:[%u] - offset:%u, elems:%u", comp_idx,
                group->comp_desc.per_attr_desc[comp_idx].attr_offset,
                group->comp_desc.per_attr_desc[comp_idx].attr_count
                );
        }
    }
    print("\n\n");
}
