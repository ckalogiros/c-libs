/*
 * Created by ckalo on 8/10/26.
 */

#include "uniforms.h"

#include "arena_allocator.h"
#include "buffer_arr_internals.h"
#include "fdebug_test.h"
#include "gfx_context.h"
#include "logger.h"
#include "shader_id.h"
#include "glad/gl.h"


#define unif_group_buffer_get(unif_id) (&((struct uniforms_group*) unifGroupDescr_groups_table->data)[unif_id])


buffer_t* unifGroupDescr_groups_table = NULL;


void
unifGroupDescr_table_init(void)
{
    unifGroupDescr_groups_table = buffer_arr_constructor_gentype(2, sizeof(struct uniforms_group));
}

attrid_t unifGroupDescr_create_new_unif_group(const sididx_t sid_idx)
{
    /// create new uniforms group
    const attrid_t unif_idx = buffer_arr_set_for_push(unifGroupDescr_groups_table);
    struct uniforms_group* unif_group = unif_group_buffer_get(unif_idx);

    /// init memory storage for uniforms data. This is a contiguous buffer of type float(for now), storing uniforms for each object interleaved.
    unif_group->unif_data_id = arena_allocator_create_new_instance(6);

    unif_group->sid_idx             = sid_idx;
    unif_group->added_uniforms_flag = 0x0;
    unif_group->commands_flag       = 0x0;
    unif_group->unif_desc_count = 0;
    unif_group->unif_count      = 0;

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "-- CREATE NEW UNIFORMS GROUP, idx: %u\n", unif_idx);

    return unif_idx;
}

attrid_t unifGroupDescr_group_find_match(const sididx_t sid_idx)
{
    for(attrid_t i = 0; i < unifGroupDescr_groups_table->tail+1; i++) {
        const struct uniforms_group* unif_group = unif_group_buffer_get(i);
        if(sid_check_match(unif_group->sid_idx, sid_idx)) { return i; }
    }

    return INT_NULL;
}


void unifGroupDescr_add_uniforms(const attrid_t unif_idx, const float* const src, 
                                 const uint32 elems_count, const sid_t unif_flag)
{
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, " { %s }:[", shaderinfo_flag_to_flagname(unif_flag));
    struct uniforms_group* unif_group = unifGroupDescr_get_unif_group(unif_idx);

    float* buffer_ptr = arena_allocator_alloc(unif_group->unif_data_id, elems_count*sizeof(float), sizeof(float));
    for(uint32 i = 0; i < elems_count; i++) {
        buffer_ptr[i] = src[i];
        print("  %2.2f, ", buffer_ptr[i]);
        engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "%2.2f,", buffer_ptr[i]);
    }
    print("\n");

    /// the "per_attr_desc" stores each unique uniforms description (ONCE per uniform per group).
    if((unif_group->added_uniforms_flag & unif_flag) == 0x0) {
        const attr_t attrib_idx = unif_group->unif_count;
        assert(attrib_idx < MAX_UNIFORMS);
        unif_group->added_uniforms_flag |= unif_flag;
        unif_group->unif_descr[attrib_idx].unif_flag = unif_flag;
        unif_group->unif_descr[attrib_idx].unif_num_elems  = elems_count;
        unif_group->unif_count++;
    }

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "]");
}

struct uniforms_group* unifGroupDescr_get_unif_group(const attrid_t unif_idx)
{
    // struct uniforms_group* group = &object_components_buffer[unif_idx];
    struct uniforms_group* unif_group = unif_group_buffer_get(unif_idx);
    return unif_group;
}


/* GETTERS-SETTERS */
struct uniform_descriptor* unifGroupDescr_get_unif_descr(const attrid_t unif_idx)
{
    struct uniforms_group* unif_group = unif_group_buffer_get(unif_idx);
    return unif_group->unif_descr;
}
/// get the count of how many uniforms this group/descriptor have enabled
attr_t unifGroupDescr_get_unif_count(const attrid_t unif_idx)
{
    const struct uniforms_group* unif_group = unif_group_buffer_get(unif_idx);
    return unif_group->unif_count;
}
float* unifGroupDescr_get_unif_data(const attrid_t unif_idx)
{
    const struct uniforms_group* unif_group = unif_group_buffer_get(unif_idx);
    float* buffer_ptr = debug_arena_allocator_get_instance_data(unif_group->unif_data_id);
    return &buffer_ptr[0];
}

void gl_uniform1f             (union uniform_data* params, const int32 location){}
void gl_uniform1fv            (union uniform_data* params, const int32 location){}
void gl_uniform1i             (union uniform_data* params, const int32 location){}
void gl_uniform1iv            (union uniform_data* params, const int32 location){}
void gl_uniform1ui            (union uniform_data* params, const int32 location){}
void gl_uniform1uiv           (union uniform_data* params, const int32 location){}
void gl_uniform2f             (union uniform_data* params, const int32 location){}
void gl_uniform2fv            (union uniform_data* params, const int32 location){}
void gl_uniform2i             (union uniform_data* params, const int32 location){}
void gl_uniform2iv            (union uniform_data* params, const int32 location){}
void gl_uniform2ui            (union uniform_data* params, const int32 location){}
void gl_uniform2uiv           (union uniform_data* params, const int32 location){}
void gl_uniform3f             (union uniform_data* params, const int32 location){}
void gl_uniform3fv            (union uniform_data* params, const int32 location){}
void gl_uniform3i             (union uniform_data* params, const int32 location){}
void gl_uniform3iv            (union uniform_data* params, const int32 location){}
void gl_uniform3ui            (union uniform_data* params, const int32 location){}
void gl_uniform3uiv           (union uniform_data* params, const int32 location){}
void gl_uniform4f             (union uniform_data* params, const int32 location)
{
    glUniform4f(location, params->value.v0, params->value.v1, params->value.v2, params->value.v3);
}
void gl_uniform4fv            (union uniform_data* params, const int32 location)
{
    float* data = params->buffer.data;
    glUniform4fv(location, params->buffer.count, data);
}
void gl_uniform4i             (union uniform_data* params, const int32 location){}
void gl_uniform4iv            (union uniform_data* params, const int32 location){}
void gl_uniform4ui            (union uniform_data* params, const int32 location){}
void gl_uniform4uiv           (union uniform_data* params, const int32 location){}
void gl_uniform_matrix2fv     (union uniform_data* params, const int32 location){}
void gl_uniform_matrix2x3fv   (union uniform_data* params, const int32 location){}
void gl_uniform_matrix2x4fv   (union uniform_data* params, const int32 location){}
void gl_uniform_matrix3fv     (union uniform_data* params, const int32 location){}
void gl_uniform_matrix3x2fv   (union uniform_data* params, const int32 location){}
void gl_uniform_matrix3x4fv   (union uniform_data* params, const int32 location){}
void gl_uniform_matrix4fv     (union uniform_data* params, const int32 location)
{
    float* data = params->buffer.data;
    glUniformMatrix4fv(location, params->buffer.count, params->buffer.transpose, data);

}
void gl_uniform_matrix4x2fv   (union uniform_data* params, const int32 location){}
void gl_uniform_matrix4x3fv   (union uniform_data* params, const int32 location){}
void gl_uniform_block_binding (uint32 program, uint32 uniformBlockIndex, uint32 uniformBlockBinding){}


/* DEBUG-PRINT */
void unifGroupDescr_group_print()
{
    print("\n%s- OBJECT UNIFORMS GROUP -%s",  YELLOW, CLEAR);
    assert(unifGroupDescr_groups_table);
    for(attrid_t gr_idx = 0; gr_idx < unifGroupDescr_groups_table->tail+1; gr_idx++) {

        const struct uniforms_group* unif_group = unifGroupDescr_get_unif_group(gr_idx);

        // added_uniforms_flag;  commands_flag;  uniforms_data_id; uniforms_count; uniforms_desc_count;
        print("\n  uniforms group id: %u, num uniforms:[%u]", gr_idx,
              unif_group->unif_count
        );

        for(uint32 unif_idx = 0; unif_idx < unif_group->unif_count; unif_idx++) {

            const char* unif_name = shaderinfo_flag_to_flagname(unif_group->unif_descr[unif_idx].unif_flag);
            print("\n  unif:[%s], elems:%u", unif_idx,
                  unif_name,
                  unif_group->unif_descr[unif_idx].unif_num_elems
            );
        }
    }
    print("\n\n");
}