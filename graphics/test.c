/*
 * Created by ckalo on 5/29/26.
 */

#include "test.h"

#include <assert.h>

#include "command.h"
#include "gfx_buffers.h"
#include "logger.h"
#include "mesh.h"

/* This group mirrors the vertex buffers,
 * for each one vertex buffer there is one group.
 * The "data" stores the object's components or attributes (in elements type).
 * Each component is defined by "num_elems", that is the size of the component in elements type
 * and an offset that is the offset of the first float of a component in the data buffer.
 * component wpos3  :3 elems, offset:0
 * component scale2 :2 elems, offset:3
 * component col4   :4 elems, offset:5
 */



struct object_components_group object_components_buffer[32];
uint32 index_count = 0;


uint32 test_component_group_find_match(const sididx_t sid_idx)
{
    const uint32 count = index_count;
    for(build_t i = 0; i < count; i++) {
        struct object_components_descriptor comp_desc = object_components_buffer[i].comp_desc;
        if(sid_check_match(comp_desc.sid_idx, sid_idx)) { return i; }
    }

    uint32 group_id = test_create_new_components_buffer(sid_idx);

    return group_id;
}

uint32 test_create_new_components_buffer(const sididx_t sid_idx)
{
    uint32 idx = index_count;
    index_count++;

    object_components_buffer[idx].comp_desc.build_idx = idx;
    // const sididx_t sid_idx = gfx_create_sid(sid);
    object_components_buffer[idx].comp_desc.sid_idx = sid_idx;

    /// ADD COMMAND: create vertex buffer
    // struct vb_cmd_params params = { .vbidx = INT_NULL, .vpos = NULL, .vpos_flag = 0x0, };
    // void* p = &params;
    // cmd_add_command((void*)gfx_vertex_buffer_create, p, sizeof(struct vb_cmd_params));

    return idx;
}
uint32 test_create_new_obj(const vbidx_t group_id)
{
    struct object_components_group* group = &object_components_buffer[group_id];
    const uint32 obj_comp_idx = group->comp_desc.obj_count;
    group->comp_desc.obj_count++;
    group->comp_desc.total_vertex_count = group->comp_desc.obj_count * group->comp_desc.obj_vertex_count; // TODO: should total vertices be calculated objects count * object vertices OR add each object vertices to total???

    return obj_comp_idx;
}


struct object_components_group* test_get_component(const vbidx_t group_id)
{
    struct object_components_group* group = &object_components_buffer[group_id];
    return group;
}

// HERE: The object for the first time a group is created must provide the desriptor for that group ONCE
void test_add_comp(const vbidx_t group_id, const uint32 obj_idx, float* src, const uint32 elems_count, const uint32 offset, const sid_t comp_flag)
{
    print("%s- ADD COMPONENT: %s -%s\n",  YELLOW, shaderinfo_flag_to_flagname(comp_flag), CLEAR);
    struct object_components_group* group = test_get_component(group_id);
    const uint32 start = obj_idx * group->comp_desc.stride;
    float* buffer = &group->data[start];
    uint32 bidx = offset;
    for(uint32 i = 0; i < elems_count; i++) {
        buffer[bidx++] = src[i];
        print("  %2.2f, ", buffer[bidx-1]);
    }
    print("\n");

    /// add attribute's description (ONCE per attribute per group)
    if((group->comp_desc.added_components_flag & comp_flag) == 0x0) {
        const uint32 comp_idx = group->comp_desc.num_components;
        assert(comp_idx < MAX_COMPONENTS);
        group->comp_desc.added_components_flag |= comp_flag;
        group->comp_desc.attr_dsc[comp_idx].elems_count = elems_count;
        group->comp_desc.attr_dsc[comp_idx].offset = offset;
        group->comp_desc.num_components++;
    }


}


/* GETTERS-SETTERS */
// void test_set_build_idx(const vbidx_t group_id, build_t build_idx)
// {
//     struct object_components_group* group = &object_components_buffer[group_id];
//     group->comp_desc.build_idx = build_idx;
// }
void test_set_stride(const vbidx_t group_id, uint32 stride)
{
    struct object_components_group* group = &object_components_buffer[group_id];
    group->comp_desc.stride = stride;
}
void test_set_obj_vertex_count(const vbidx_t group_id, uint32 obj_vertex_count)
{
    struct object_components_group* group = &object_components_buffer[group_id];
    group->comp_desc.obj_vertex_count = obj_vertex_count;
}

struct object_components_descriptor* test_get_comp_desc(const vbidx_t group_id)
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
float* test_get_obj_attribs_data(const vbidx_t buffer_id)
{
    return &object_components_buffer[buffer_id].data[0];
}
uint32 test_get_obj_vertex_count(const vbidx_t buffer_id)
{
    return object_components_buffer[buffer_id].comp_desc.obj_vertex_count;
}
uint32 test_get_total_vertex_count(const vbidx_t buffer_id)
{
    return object_components_buffer[buffer_id].comp_desc.total_vertex_count;
}


/* DEBUG-PRINT */
void obj_comp_group_print()
{
    print("\n%s- OBJECT COMPONENTS GROUP -%s",  YELLOW, CLEAR);
    for(uint32 group_id = 0; group_id < index_count; group_id++) {

        struct object_components_group* group = test_get_component(group_id);


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
        for(uint32 comp_idx = 0; comp_idx < group->comp_desc.num_components; comp_idx++) {

            print("\n  comp:[%u] - offset:%u, elems:%u\n", comp_idx,
                group->comp_desc.attr_dsc[comp_idx].offset,
                group->comp_desc.attr_dsc[comp_idx].elems_count
                );

            uint32 elems_count = group->comp_desc.attr_dsc[comp_idx].elems_count;
            for(uint32 idx = offset; idx < offset+elems_count; idx++) {
                print("%s  %.2f, %s", ry, group->data[idx], CLEAR);
            }

            offset += elems_count;
        }
        print("\n");
    }
    print("\n\n");
}
