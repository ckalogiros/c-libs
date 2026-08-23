/*
 * Created by ckalo on 5/29/26.
 */

#include "attributes.h"
#include <assert.h>
#include <string.h>

#include "fdebug_test.h"
#include "gfx_buffers.h"
#include "logger.h"
#include "arena_allocator.h"
#include "my_macros.h"
#include "object.h"
#include "buffer_arr_internals.h"
#include "buffer_arr_struct.h"

/*
 * The "data" stores the object's attributes (in elements of one type).
 * Each attribute is defined by "num_elems", that is the length of the attribute in elements(not bytes)
 * and an offset that is the offset of the first float of each attribute in the data buffer.
 * attribute wpos3  :3 elems, offset:0
 * attribute scale2 :2 elems, offset:3
 * attribute col4   :4 elems, offset:5
 */



#define attribs_group_buffer_get(attribs_id) (&((struct attributes_group*) attribsGroupDescr_groups_table->data)[attribs_id])
#define obj_bufids_get(id) (&((objid_t*) attribs_group->obj_bufids->data)[id])


buffer_t* attribsGroupDescr_groups_table = NULL;


void
attribsGroupDescr_table_init(void)
{
    attribsGroupDescr_groups_table = buffer_arr_constructor_gentype(2, sizeof(struct attributes_group));
}

attrid_t attribsGroupDescr_create_new_attribs_group(const sididx_t sid_idx)
{
    /// create new attributes group
    const attrid_t attribs_idx = buffer_arr_set_for_push(attribsGroupDescr_groups_table);
    struct attributes_group* attribs_group = attribs_group_buffer_get(attribs_idx);

    /// init memory storage for attributes data. This is a contiguous buffer of type float(for now), storing attributes for each object interleaved.
    attribs_group->attribs_data_id = arena_allocator_create_new_instance(6);

    /// init memory storage for object ids. Store the objects that "belong" to this group
    attribs_group->obj_bufids = buffer_arr_create_gentype(16, sizeof(objid_t));

    attribs_group->sid_idx               = sid_idx;
    attribs_group->added_attributes_flag = 0x0;
    attribs_group->commands_flag         = 0x0;
    attribs_group->attribs_desc_count  = 0;
    attribs_group->vertex_count          = 0;
    attribs_group->attribs_count         = 0;
    attribs_group->stride                = 0;
    attribs_group->build_idx = INT_NULL;
    attribs_group->rqidx     = INT_NULL;

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "-- CREATE NEW COMPONENTS BUFFER, idx: %u\n", attribs_idx);

    return attribs_idx;
}

attrid_t attribsGroupDescr_group_find_match(const sididx_t sid_idx)
{
    for(attrid_t i = 0; i < attribsGroupDescr_groups_table->tail+1; i++) {
        const struct attributes_group* attribs_group = attribs_group_buffer_get(i);
        if(sid_check_match(attribs_group->sid_idx, sid_idx)) { return i; }
    }

    return INT_NULL;
}

/* adds the object's id to the objects id buffer of the attribute group, along with some descriptor parameters */
objid_t attribsGroupDescr_create_new_obj(const attrid_t attribs_idx, const objid_t obj_id, const uint16 obj_vertex_count)
{
    struct attributes_group* attribs_group = attribs_group_buffer_get(attribs_idx);

    /*
     * initialization: parameters for the group's attributes
     * The "attribs_descr" is a per attribute buffer and not a per object.
     * That means, all objects must have the following attribute characteristics:
     *      stride,
     *
     */
    attribs_group->vertex_count += obj_vertex_count;
    const uint32 cnt = attribs_group->attribs_desc_count;
    assert(cnt < COMP_MAX_ATTRIBUTES);
    attribs_group->attribs_descr[cnt].attr_offset    = 0;
    attribs_group->attribs_descr[cnt].attr_num_elems = 0;
    attribs_group->attribs_desc_count++;

    /// an attribute group stores all objects by their id, in a dynamicaly grown buffer
    const objid_t idx = buffer_arr_set_for_push(attribs_group->obj_bufids); /// get the
    objid_t* buffer_ids = obj_bufids_get(idx);
    *buffer_ids = obj_id;
    const objid_t obj_count = attribs_group->obj_bufids->count;

    return obj_count;
}

void attribsGroupDescr_add_attributes(const attrid_t attribs_idx, const float* const src, const uint32 elems_count,
                                      const uint32 offset, const sid_t attrib_flag, const int16 inst_divisor, const attr_t shader_attrib_idx)
{
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, " { %s }:[", shaderinfo_flag_to_flagname(attrib_flag));
    struct attributes_group* attribs_group = attribsGroupDescr_get_attribs_group(attribs_idx);

    float* buffer_ptr = arena_allocator_alloc(attribs_group->attribs_data_id, elems_count*sizeof(float), sizeof(float));
    for(uint32 i = 0; i < elems_count; i++) {
        buffer_ptr[i] = src[i];
        print("  %2.2f, ", buffer_ptr[i]);
        engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "%2.2f,", buffer_ptr[i]);
    }
    print("\n");

    /// the "per_attr_desc" stores each unique attributes description (ONCE per attributes per group).
    /// this function is called for every object, but we need not to keep duplicates of attributes descriptions
    if((attribs_group->added_attributes_flag & attrib_flag) == 0x0) {
        const attr_t attrib_idx = attribs_group->attribs_count;
        assert(attrib_idx < COMP_MAX_ATTRIBUTES);
        attribs_group->added_attributes_flag |= attrib_flag;
        attribs_group->attribs_descr[attrib_idx].attrib_flag = attrib_flag;
        attribs_group->attribs_descr[attrib_idx].attr_offset = offset;
        attribs_group->attribs_descr[attrib_idx].divisor  = inst_divisor;
        attribs_group->attribs_descr[attrib_idx].attr_num_elems  = elems_count;
        // attribs_group->attribs_descr[attrib_idx].shader_input_flags  = shader_input_flags;
        attribs_group->attribs_count++;
    }

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "]");
}

struct attributes_group* attribsGroupDescr_get_attribs_group(const attrid_t attribs_idx)
{
    // struct attributes_group* group = &object_components_buffer[attribs_idx];
    struct attributes_group* attribs_group = attribs_group_buffer_get(attribs_idx);
    return attribs_group;
}


/* GETTERS-SETTERS */

void attribsGroupDescr_set_stride(const attrid_t attribs_idx, const uint32 stride)
{
    struct attributes_group* attribs_group = attribs_group_buffer_get(attribs_idx);
    attribs_group->stride = stride;
}
void attribsGroupDescr_set_build_idx(const attrid_t attribs_idx, const build_t build_idx)
{
    struct attributes_group* attribs_group = attribs_group_buffer_get(attribs_idx);
    attribs_group->build_idx = build_idx;
}
buffer_t* attribsGroupDescr_get_objects_group(const attrid_t attribs_idx)
{
    struct attributes_group* attribs_group = attribs_group_buffer_get(attribs_idx);
    return attribs_group->obj_bufids;
}
struct attributes_descriptor* attribsGroupDescr_get_attribs_descr(const attrid_t attribs_idx)
{
    struct attributes_group* attribs_group = attribs_group_buffer_get(attribs_idx);
    return attribs_group->attribs_descr;
}
build_t attribsGroupDescr_get_buildidx(const attrid_t attribs_idx)
{
    const struct attributes_group* attribs_group = attribs_group_buffer_get(attribs_idx);
    return attribs_group->build_idx;
}
uint32 attribsGroupDescr_get_attribs_stride(const attrid_t attribs_idx)
{
    const struct attributes_group* attribs_group = attribs_group_buffer_get(attribs_idx);
    return attribs_group->stride;
}
/// get the count of how many attributes this group/descriptor have enabled
attr_t attribsGroupDescr_get_attribs_count(const attrid_t attribs_idx)
{
    const struct attributes_group* attribs_group = attribs_group_buffer_get(attribs_idx);
    return attribs_group->attribs_count;
}
uint32 attribsGroupDescr_get_vertex_count(const attrid_t attribs_idx)
{
    const struct attributes_group* attribs_group = attribs_group_buffer_get(attribs_idx);
    return attribs_group->vertex_count;
}
float* attribsGroupDescr_get_attribs_data(const attrid_t attribs_idx)
{
    const struct attributes_group* attribs_group = attribs_group_buffer_get(attribs_idx);
    float* buffer_ptr = debug_arena_allocator_get_instance_data(attribs_group->attribs_data_id);
    return &buffer_ptr[0];
}
uint32 attribsGroupDescr_get_objects_count(const attrid_t attribs_idx)
{
    const struct attributes_group* attribs_group = attribs_group_buffer_get(attribs_idx);
    return attribs_group->obj_bufids->count;
}
bool attribsGroupDescr_is_indexed_geometry(const attrid_t attribs_idx)
{
    const struct attributes_group* attribs_group = attribs_group_buffer_get(attribs_idx);
    const sididx_t sid_idx = attribs_group->sid_idx;
    const bool res = sid_check_attr_match_from_flag(sid_idx, SID_INDEXED);
    return res;
}


/* DEBUG-PRINT */
void attribsGroupDescr_group_print()
{
    print("\n%s- OBJECT ATTRIBUTES GROUP -%s",  YELLOW, CLEAR);
    for(attrid_t attribs_idx = 0; attribs_idx < attribsGroupDescr_groups_table->tail+1; attribs_idx++) {

        const struct attributes_group* attribs_group = attribsGroupDescr_get_attribs_group(attribs_idx);


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

            print("\n  attrib:[%u] - offset:%u, elems:%u", attribs_idx,
                  attribs_group->attribs_descr[attribs_idx].attr_offset,
                  attribs_group->attribs_descr[attribs_idx].attr_num_elems
            );
        }
        // for(uint32 i = 0; i < attribs_group->instancing->tail+1; i++) {
        //
        //     struct instancing* inst = attribs_group->instancing->data;
        //     print("\n  inst:[%u] - attribs_count:%u, divisor:%u, stride:%u, vbidx:%d", i,
        //           inst[i].attribs_count,
        //           inst[i].divisor,
        //           inst[i].stride,
        //           inst[i].vbidx
        //     );
        // }
    }
    print("\n\n");
}
