/*
 * Created by ckalo on 5/29/26.
 */

#include "object_components.h"
#include <assert.h>
#include "fdebug_test.h"
#include "gfx_buffers.h"
#include "logger.h"
#include "arena_allocator.h"
#include "my_macros.h"
#include "object.h"
#include "sparse_arr_internals.h"
#include "sparse_arr_struct.h"

/* This group mirrors the vertex buffers,
 * for each one vertex buffer there is one group.
 * The "data" stores the object's components or attributes (in elements type).
 * Each component is defined by "num_elems", that is the size of the component in elements type
 * and an offset that is the offset of the first float of a component in the data buffer.
 * component wpos3  :3 elems, offset:0
 * component scale2 :2 elems, offset:3
 * component col4   :4 elems, offset:5
 */



#define sparse_arr_comp_get(comp_id) (&((struct components_group*) obj_component_groups_table->data)[comp_id])
#define sparse_arr_obj_id_get(id) (&((objid_t*) comp_group->obj_group->data)[id])
// #define sparse_arr_obj_id_get(id) (&((objid_t*) comp_group->obj_group->data)[id]) /// TODO: to compine the 2 buffers we use the first buffer offset( sizeof(struct comp_per_attrib_descriptor) * num_attributes)
// #define sparse_arr_obj_id_get(id) (&((objid_t*) comp_group->obj_group->data)[id])


sparse_arr_t* obj_component_groups_table = NULL;


void
obj_component_table_init(void)
{
    obj_component_groups_table = sparse_arr_constructor_gentype(2, sizeof(struct components_group));
}

compid_t obj_component_group_find_match(const sididx_t sid_idx)
{
    for(compid_t i = 0; i < obj_component_groups_table->tail+1; i++) {
        struct components_group* comp_group = sparse_arr_comp_get(i);
        struct comp_per_group_descriptor* comp_desc = &comp_group->comp_desc;
        if(sid_check_match(comp_desc->sid_idx, sid_idx)) { return i; }
    }

    return INT_NULL;
}

compid_t obj_component_create_new_components_group(const sididx_t sid_idx)
{
    /// init struct comp_per_group_descriptor
    const compid_t comp_group_id = sparse_arr_set_for_push(obj_component_groups_table);
    struct components_group* comp_group = sparse_arr_comp_get(comp_group_id);
    comp_group->attribs_data_inst_id = arena_allocator_create_new_instance(6);

    /// init the mem group buffer that will store all objects id for a compomonent group
    comp_group->obj_group = sparse_arr_create_gentype(16, sizeof(objid_t));

    struct comp_per_group_descriptor* comp_desc = &comp_group->comp_desc;
    comp_desc->sid_idx               = sid_idx;
    comp_desc->added_components_flag = 0x0;
    comp_desc->commands_flag         = 0x0;
    comp_desc->per_attr_desc_counter = 0;
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

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "-- CREATE NEW COMPONENTS BUFFER, idx: %u\n", comp_group_id);

    return comp_group_id;
}

compid_t obj_component_create_new_obj(const vbidx_t group_id, const objid_t obj_id, const uint16 obj_vertex_count, enum MESH_TYPE mesh_type)
{
    // struct components_group* group = &object_components_buffer[group_id];
    struct components_group* comp_group = sparse_arr_comp_get(group_id);
    const uint32 obj_comp_idx = comp_group->comp_desc.obj_count;
    comp_group->comp_desc.obj_count++;

    /// initialization: parameters for each object components
    struct comp_per_group_descriptor* desc = &comp_group->comp_desc; // cache
    desc->total_vertex_count += obj_vertex_count;

    desc->per_attr_desc[desc->per_attr_desc_counter].attr_offset = 0;
    desc->per_attr_desc[desc->per_attr_desc_counter].attr_count++;
    assert(desc->per_attr_desc_counter < COMP_MAX_ATTRIBUTES);
    desc->per_attr_desc_counter++;

    comp_group->obj_group_id = sparse_arr_set_for_push(comp_group->obj_group);
    objid_t* buffer_ids = sparse_arr_obj_id_get(comp_group->obj_group_id);
    *buffer_ids = obj_id;

    // const struct object* obj = object_get(obj_comp_idx);



    return obj_comp_idx;
}

struct components_group* obj_component_get_component(const vbidx_t group_id)
{
    // struct components_group* group = &object_components_buffer[group_id];
    struct components_group* comp_group = sparse_arr_comp_get(group_id);
    return comp_group;
}

// HERE: The object for the first time a group is created must provide the desriptor for that group ONCE
void obj_component_add_comp(const vbidx_t group_id, const uint32 obj_idx, float* src, const uint32 elems_count, const uint32 offset, const sid_t comp_flag)
{
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, " { %s }:[", shaderinfo_flag_to_flagname(comp_flag));
    struct components_group* group = obj_component_get_component(group_id);

    float* buffer_ptr = arena_allocator_alloc(group->attribs_data_inst_id, elems_count*sizeof(float), sizeof(float));
    uint32 bidx = offset;
    for(uint32 i = 0; i < elems_count; i++) {
        buffer_ptr[i] = src[i];
        print("  %2.2f, ", buffer_ptr[i]);
        engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "%2.2f,", buffer_ptr[i]);
    }
    print("\n");

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

void obj_component_set_stride(const vbidx_t group_id, uint32 stride)
{
    struct components_group* comp_group = sparse_arr_comp_get(group_id);
    comp_group->comp_desc.stride = stride;
}
build_t obj_component_set_build_idx(const vbidx_t group_id, const build_t build_idx)
{
    struct components_group* comp_group = sparse_arr_comp_get(group_id);
    comp_group->comp_desc.build_idx = build_idx;
}
// void obj_component_set_obj_vertex_count(const vbidx_t group_id, const uint32 obj_vertex_count)
// {
//     // struct components_group* comp_group = &object_components_buffer[group_id];
//     struct components_group* comp_group = sparse_arr_comp_get(group_id);
//     comp_group->comp_desc.obj_vertex_count = obj_vertex_count;
// }

struct comp_per_group_descriptor* obj_component_get_comp_desc(const vbidx_t group_id)
{
    struct components_group* comp_group = sparse_arr_comp_get(group_id);
    return &comp_group->comp_desc;
}
bool obj_component_is_indexed_geometry(const vbidx_t group_id)
{
    struct components_group* comp_group = sparse_arr_comp_get(group_id);
    const sididx_t sid_idx = comp_group->comp_desc.sid_idx;
    bool res = sid_check_attr_match_from_flag(sid_idx, SID_INDEXED);
    return res;
}
sparse_arr_t* obj_component_get_obj_group(const vbidx_t group_id)
{
    struct components_group* comp_group = sparse_arr_comp_get(group_id);
    return comp_group->obj_group;
}
build_t obj_component_get_build_idx(const vbidx_t group_id)
{
    struct components_group* comp_group = sparse_arr_comp_get(group_id);
    return comp_group->comp_desc.build_idx;
}
uint32 obj_component_obj_comp_desc_get_stride(const vbidx_t group_id)
{
    struct components_group* comp_group = sparse_arr_comp_get(group_id);
    return comp_group->comp_desc.stride;
}
uint32 obj_component_obj_comp_desc_get_total_vertex_count(const vbidx_t group_id)
{
    struct components_group* comp_group = sparse_arr_comp_get(group_id);
    return comp_group->comp_desc.total_vertex_count;
}
uint32 obj_component_obj_comp_desc_get_total_object_count(const vbidx_t group_id)
{
    struct components_group* comp_group = sparse_arr_comp_get(group_id);
    return comp_group->comp_desc.obj_count;
}
float* obj_component_get_obj_attribs_data(const vbidx_t group_id)
{
    struct components_group* comp_group = sparse_arr_comp_get(group_id);
    float* buffer_ptr = debug_arena_allocator_get_instance_data(comp_group->attribs_data_inst_id);
    return &buffer_ptr[0];
}
uint32 obj_component_get_obj_vertex_count(const vbidx_t group_id)
{
    struct components_group* comp_group = sparse_arr_comp_get(group_id);
    return comp_group->comp_desc.obj_vertex_count;
}
uint32 obj_component_get_total_vertex_count(const vbidx_t group_id)
{
    struct components_group* comp_group = sparse_arr_comp_get(group_id);
    return comp_group->comp_desc.total_vertex_count;
}


/* DEBUG-PRINT */
void obj_component_group_print()
{
    print("\n%s- OBJECT COMPONENTS GROUP -%s",  YELLOW, CLEAR);
    for(uint32 group_id = 0; group_id < obj_component_groups_table->tail; group_id++) {

        struct components_group* comp_group = obj_component_get_component(group_id);


        print(
              "\n%s  obj_count:%u, "
              "  obj_vertex_count:%u, "
              "  total_vertex_count:%u, "
              "  rqidx:%u %s", CYAN,
            comp_group->comp_desc.obj_count,
            comp_group->comp_desc.obj_vertex_count,
            comp_group->comp_desc.total_vertex_count,
            comp_group->comp_desc.rqidx,
            CLEAR
            );

        uint32 offset = 0;
        for(uint32 comp_idx = 0; comp_idx < comp_group->comp_desc.components_count; comp_idx++) {

            print("\n  comp:[%u] - offset:%u, elems:%u", comp_idx,
                comp_group->comp_desc.per_attr_desc[comp_idx].attr_offset,
                comp_group->comp_desc.per_attr_desc[comp_idx].attr_count
                );
        }
    }
    print("\n\n");
}
