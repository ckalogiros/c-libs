/*
 * Created by ckalo on 7/29/26.
 */

#include "test_custom_rendering.h"

#include "mesh.h"
#include "shader_id.h"
#include "shader_descriptor.h"
#include <math.h>
#include <string.h>
#include "arena_allocator.h"
#include "fdebug_test.h"
#include "logger.h"
#include "object.h"
#include "shader_builder.h"
#include "camera.h"
#include "command.h"
#include "my_macros.h"
#include "opengl.h"
#include "opengl_debug.h"
#include "run.h"
#include "string_format.h"
#include "uniforms.h"


struct cmd_gfx_params {
    const char* unif_name;
    const float* unif_data;
    sididx_t sid_idx;
    attr_t   attribs_group_desc_idx;
    build_t  build_idx;
    desc_t   desc_idx;
    uint8    vpos_num_elems;
};


static const char* test_fragment_shader_text = "\
        #version 330\n\
        in vec2 out_vpos;\n\
        in vec4 out_color4;\n\
        out vec4 fragment;\n\
\n\
        void main()\n\
        {\n\
            vec2 screen_size = vec2(1000.0, 1000.0);\n\
            vec2 uv = (gl_FragCoord.xy / screen_size) * .5 + .5;\n\
            vec2 p = (abs(out_vpos)/screen_size) * .5 + .5;\n\
            float d = length( (p - uv) ) * 2.;\n\
\n\
            //vec3 final_color = vec3(out_color4.rgb*d);\n\
            vec3 final_color = vec3(out_color4.rgb);\n\
            fragment = vec4(final_color, 1.0);\n\
\n\
            //float depth = gl_FragCoord.z/ 2.;\n\
            //fragment = vec4(depth, depth, depth, 1.0);\n\
            //fragment *= vec4(final_color * depth, 1.0);\n\
        }\n";


// void TEMP_new_shader_program_needed(const sididx_t sid_idx, const attrid_t attribs_group_id, const desc_t desc_idx);

void temp_fn(objid_t obj_id)
{
    struct object* obj = object_get(obj_id);

    /// create shader build
    build_t build_idx = shader_build_exists(obj->sid_idx);
    if(build_idx == INT_NULL) build_idx = shader_builder_create_new_build(obj->sid_idx);
    struct shader_descriptor* shader_desc = shader_descriptor_get(build_idx);
    struct opengl_program* prog = shader_program_get(build_idx);


    attribsGroupDescr_set_build_idx(obj->attribs_group_id, build_idx);



    /**********************************************************************************************
     * ATTRIBUTES */
    struct attributes_group* attribs_group = attribsGroupDescr_get_attribs_group(obj->attribs_group_id);
    struct attributes_descriptor* attribs_descr = attribs_group->attribs_descr;
    struct uniforms_group* unifs_group = unifGroupDescr_get_unif_group(obj->unifs_group_id);
    struct uniform_descriptor * unif_descr = unifs_group->unif_descr;

    /// create shader attributes inputs,
    for(uint32 i = 0; i < attribs_group->attribs_desc_count; i++) {

        const attr_t shader_input_id = shaderinfo_flag_to_id(attribs_descr[i].attrib_flag);
        shader_create_input(build_idx, shader_input_id, attribs_descr[i].shader_input_flags, attribs_descr[i].divisor);
    }


    /***********************************************************************************************
     * UNIFORMS */
    /// create shader uniform inputs
    for(uint32 i = 0; i < unifs_group->unif_count; i++) {

        const attr_t shader_input_id = shaderinfo_flag_to_id(unif_descr[i].unif_flag);
        shader_create_input(build_idx, shader_input_id, unif_descr[i].shader_input_flags, 0);
        // sid.unif |= unif_descr[i].unif_flag;
        unif_descr[i].loc = i;

        // unifGroupDescr_add_uniforms(i, const float* const src, elems_count, const sid_t unif_flag);

    }


    /***********************************************************************************************
     * TODO: SSBO's, UBO's */


    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
    // const attr_t vao_attribs_idx = shader_desc->vaoidx;

    /*  */
    struct cmd_gfx_params gfx_params = {
        .sid_idx = obj->sid_idx,
        .attribs_group_desc_idx = obj->attribs_group_id,
        .build_idx = attribs_group->build_idx,
        .desc_idx = INT_NULL,
        .vpos_num_elems = mesh_get_num_elems_per_vertex(obj->mesh_type),
        .unif_name = NULL,
        .unif_data = NULL
    };

    if((attribs_group->commands_flag & CMD_CREATE_SHADER_PROGRAM)==0) {

        build_t build_idx = gfx_program_exists(gfx_params.sid_idx);
        if(build_idx == INT_NULL) { // If shader program does not exist, create new program.
            opengl_program_create(gfx_params.sid_idx, gfx_params.build_idx, test_fragment_shader_text); GFX_CHECK_ERROR();
        }

        attribs_group->commands_flag |= CMD_CREATE_SHADER_PROGRAM;
        gfx_params.desc_idx =  attribs_group->build_idx;
    }


    if((attribs_group->commands_flag & CMD_CREATE_VAO)==0) {

        struct attributes_descriptor* vao_descr = attribsGroupDescr_get_attribs_descr(gfx_params.attribs_group_desc_idx);
        const uint32 stride = shader_desc->vshader_attribs_stride;
        gl_use_program(prog->program);

        /* VERTEX ARRAY */
        const bool attrib_devisor = 0; /// TODO: attrib devisor should exist in object OR object component OR shader descriptor???
        bool is_new_vao = FALSE;
        vaidx_t vaoidx = gfx_vertex_array_find_match(shader_desc->sid_idx, attrib_devisor);
        if(vaoidx == INT_NULL) {
            vaoidx = gfx_vertex_array_create(shader_desc->build_idx);
            is_new_vao = TRUE;
        }
        shader_desc->vaoidx = vaoidx;


        struct vertex_array_object* va = gfx_vertex_array_get_byidx(vaoidx);
        gl_bind_vertex_array(va->vao);                                                              GFX_CHECK_ERROR();


        /* VERTEX BUFFER */
        vbidx_t vbidx = INT_NULL;
        struct vertex_buffer_object* vb = NULL;
        if(is_new_vao){

            struct vertex_array_object* vao = gfx_vertex_array_get_byidx(vaoidx);

            char buf[64] = {0}; char* ptr = &buf[0];
            uint32 size = string_format(&ptr, 64, "--- VAO:%2u ---\n", vao->idx);
            engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf, size);

            /* Enable attributes in openGl shader */
            uint32 cnt = 0, vao_attr_desc_cnt = 0;
            attr_t divisor = INT_NULL;
            attr_t vb_stride = 0;
            for(uint32 i = 0; i < shader_desc->entry_count; i++){

                if(shader_desc->entries[i].input_flag & IS_ATTR) {

                    // create vertex buffer.
                    // 1. for each different divisor for instanced rendering
                    // 2. for per attribute buffer rendering

                    if(divisor != shader_desc->entries[i].divisor) {
                        vbidx = gfx_vertex_buffer_create(attribs_group->obj_bufids->count*attribs_group->vertex_count*attribs_group->stride *SIZE_F32, stride);
                        vao->vbos[cnt++] = vbidx;                                                       /// push new vertex buffer's index to VAO


                        vb = vertex_buffer_get_vbo(vbidx);
                        gl_bind_buffer(OPENGL_ARRAY_BUFFER, vb->vbo);                               GFX_CHECK_ERROR();

                        divisor = shader_desc->entries[i].divisor;

                        char buf2[64] = {0}; ptr = &buf[0];
                        uint32 size2 = string_format(&ptr, 64, " VBO:%2u\n", vbidx);
                        engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf2, size2);

                        va->vbcount++;
                        vb_stride = 0;
                    }
// HERE: the .attr_idx is -1
                    vb->v_layout[vb->attrib_count].loc        = shader_desc->v_layout[i].loc;
                    vb->v_layout[vb->attrib_count].num_elems  = shader_desc->v_layout[i].num_elems;
                    vb->v_layout[vb->attrib_count].type       = shader_desc->v_layout[i].type;
                    vb->v_layout[vb->attrib_count].attr_idx   = shader_desc->v_layout[i].attr_idx;             /// UNUSED for vertex buffers???
                    vb->v_layout[vb->attrib_count].loc_offset = shader_desc->v_layout[i].loc_offset;           /// UNUSED for vertex buffers???
                    vb->v_layout[vb->attrib_count].offset     = vao_descr[vao_attr_desc_cnt].attr_offset;
                    vb->v_layout[vb->attrib_count].idx        = shader_desc->v_layout[i].idx;                  /// UNUSED for vertex buffers???
                    vb_stride += shader_desc->v_layout[i].num_elems;
                    vb->stride = vb_stride;

                    // @params: ( loc, num_elems, stride, offset, size, divisor )
                    shader_attribute_enable(
                        vb->v_layout[vb->attrib_count].loc,
                        shader_desc->v_layout[i].num_elems,
                        stride,
                        vb->v_layout[vb->attrib_count].offset,
                        SIZE_F32,
                        shader_desc->entries[i].divisor
                        );

                    vb->attrib_count++;

                    char buf2[128] = {0}; ptr = &buf[0];
                    uint32 size2 = string_format(&ptr, 128,
                        "[%2u]: "
                        "loc: %2u, "
                        "num_elems: %2u, "
                        "type: %s, "
                        "loc_offset: %2u, "
                        "offset: %2u, "
                        "\n",
                        shader_desc->v_layout[i].idx,
                        shader_desc->v_layout[i].loc,
                        shader_desc->v_layout[i].num_elems,
                        shaderinfo_type_to_glsltype_string(shader_desc->v_layout[i].type),
                        shader_desc->v_layout[i].loc_offset,
                        vao_descr[vao_attr_desc_cnt].attr_offset
                    );
                    engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf2, size2);

                    vao_attr_desc_cnt++;
                }
            }

            // shader_attributes_enable(desc);
            opengl_debug_attributes_in(shader_desc->vshader_attrib_locations_count);                GFX_CHECK_ERROR();
            attribs_group->rqidx = renderer_renderqueue_add(gfx_params.sid_idx, prog->program, shader_desc->build_idx,
                                                            va->vao, va->idx, vb->vbo, vb->idx);
        } else {
            vbidx = gfx_vertex_buffer_find_match(attribs_group->sid_idx);
            vb = vertex_buffer_get_vbo(vbidx);
        }
        assert(vbidx != INT_NULL);



        /* VERTEX INDEX BUFFER */
        ibidx_t ibidx = INT_NULL;
        struct vertex_index_buffer_object* ib = NULL;
        if(is_new_vao){
            /// TODO: calculate the size of the vertex buffer OR use dynamic array vertex buffer
            ibidx = gfx_vertex_index_buffer_create(200*SIZE_I32);

            ib = vertex_index_buffer_get_ib(ibidx);
            gl_bind_buffer(OPENGL_ELEMENT_ARRAY_BUFFER, ib->ibo);                                   GFX_CHECK_ERROR();

        } else {
            assert(0>0);
            // vbidx = gfx_vertex_buffer_find_match(attribs_desc->sid_idx);
            // vb = vertex_buffer_get_vbo(vbidx);
        }
        assert(ibidx != INT_NULL);

        /* SET COMMANDS */
        void* p = NULL;
        /// SET COMMAND: IB_SET_DATA
        struct ib_cmd_params ind_args = { .ibidx = ib->idx, .vpos_num_elems = gfx_params.vpos_num_elems, .attribs_idx = gfx_params.attribs_group_desc_idx};
        p = &ind_args;
        cmd_add_command((void*)gfx_vertex_index_buffer_set_data, p, sizeof(struct ib_cmd_params));
        // SET COMMAND: IB_UPDATE_DATA
        cmd_add_command((void*) gfx_vertex_index_buffer_update_data, p, sizeof(struct ib_cmd_params));


        /// SET COMMAND: VB_SET_DATA
        struct vb_cmd_params vert_args = { .vaidx = vaoidx, .vbidx = vb->idx, .vpos_num_elems = gfx_params.vpos_num_elems, .shader_attribs_idx = gfx_params.attribs_group_desc_idx};
        p = &vert_args;
        // cmd_add_command((void*)gfx_vertex_buffer_set_data, p, sizeof(struct vb_cmd_params));
        cmd_add_command((void*)gfx_vertex_buffer_set_data2, p, sizeof(struct vb_cmd_params));
        // SET COMMAND: VB_UPDATE_DATA
        cmd_add_command((void*) gfx_vertex_buffer_update_data, p, sizeof(struct vb_cmd_params));

        gl_use_program(0x0);
        gl_bind_vertex_array(0x0);
        gl_bind_buffer(OPENGL_ARRAY_BUFFER, 0x0);


        /// close file for debug
        engine_fdebug_append_to_file_from_arena(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT);
        attribs_group->commands_flag |= CMD_CREATE_VAO;
    }

    /* Set and update shader uniform inputs */
    gl_use_program(prog->program);
    //     const float* data = &uniforms_data[unif_desc[i].unif_offset];

    float* uniforms_data = unifGroupDescr_get_unif_data(unifs_group->unif_data_id);
    for(uint32 i = 0; i < unifs_group->unif_count; i++) {

        const float* data = &uniforms_data[unif_descr[i].unif_offset];
        union uniform_data params = {
            .buffer = { .count = 1, .data = data, .transpose = FALSE },
        };
        unif_descr->update_fn.fnptr(&params, unif_descr->loc);                                        GFX_CHECK_ERROR();
    }

    gl_use_program(0x0);


}

void render_tri(struct camera* camera)
{
    /*
     * USER MUST MATCH THE ATTRIBUTES DESCRIPTOR WITH THE ATTRIBUTES BUFFER
     */
    /// create attrib buffer:
    uint32 attr_offset = 0;
    struct attributes_descriptor attrib_desc[] = {
        {.attr_offset = 0, .attr_num_elems = 2, .divisor = 0, .attrib_flag = SID_VPOSI2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS},
        {.attr_offset = 0, .attr_num_elems = 4, .divisor = 0, .attrib_flag = SID_COLOR4, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_OUT},
        {.attr_offset = 0, .attr_num_elems = 3, .divisor = 0, .attrib_flag = SID_WPOSI3, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS},
        {.attr_offset = 0, .attr_num_elems = 2, .divisor = 0, .attrib_flag = SID_SCALE2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE},
        {.attr_offset = 0, .attr_num_elems = 2, .divisor = 0, .attrib_flag = SID_ROTAT2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT},
    };

    const float z = 10., inten = 1., defi = 0.0;
    float attribs_data[] = {
        // color4              wpos3         scale2     rot2
        inten,defi,defi,1.0,   0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v0
        defi,inten,defi,1.0,   0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v1
        defi,defi,inten,1.0,   0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v2
    };

    uint32 num_objects = 1;
    uint32 num_vertices = 3;
    uint32 attribs_stride = 2+4+3+2+2;
    uint32 num_attributes = ARRAY_LENGTH(attrib_desc);


    float color[] = {0.9, 0.5, 0.1, 1.0};
    float scale[] = {1.0, 1.0};
    float rot[]   = {cos(0.0), sin(0.0)};

    struct uniform_descriptor unif_desc[] = {
        {.loc = INT_NULL, .unif_offset = 0, .unif_num_elems = 4*4, .unif_flag = SID_PROJMAT4X4, .update_fn = &gl_uniform_matrix4fv, .shader_input_flags = IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ},
    };
    uint32 unif_stride = (4*4);
    uint32 num_uniforms = ARRAY_LENGTH(unif_desc);
    /// copy each uniform data to the contiguous uniforms buffer data
    float* temp[num_uniforms];
    temp[0] = &camera->projection_matrix.projection_matrix[0];
    float uniforms_data[unif_stride*SIZE_F32] = {};
    for(uint32 i = 0; i < num_uniforms; i++) {
        memcpy(uniforms_data, &temp[i][0], unif_stride*SIZE_F32);
    }

    /// create object
    struct gfx_sid sid = {0,0,0,0,0};
    sid.out  |= SID_COLOR4;
    sid.pass |= SID_COLOR4;
    sid.oper |= SID_NULL;

    enum MESH_TYPE mesh_type = MESH_TRIANGLE;

    /// create shader id if does not exist
    sididx_t sid_idx = gfx_sid_find_match(sid);
    if(sid_idx == INT_NULL) {
        sid_idx = gfx_create_sid(sid);
    }

    /// create attributes group if does not exist
    attrid_t attribs_group_id = attribsGroupDescr_group_find_match(sid_idx);
    if(attribs_group_id == INT_NULL) {
        attribs_group_id = attribsGroupDescr_create_new_attribs_group(sid_idx);
    }

    /// create uniforms group if does not exist
    attrid_t uniforms_group_id = unifGroupDescr_group_find_match(sid_idx);
    if(uniforms_group_id == INT_NULL) {
        uniforms_group_id = unifGroupDescr_create_new_unif_group(sid_idx);
    }

    /// create object
    struct object* obj  = object_create(sid_idx, mesh_type);
    obj->num_faces      = 1;
    obj->attrib_count   = num_attributes;
    obj->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
    obj->index_count    = mesh_get_mesh_indices_count_from_type(mesh_type);
    obj->attribs_group_id = attribs_group_id;
    obj->unifs_group_id   = uniforms_group_id;
    object_set_vertex_count(obj->id, obj->num_faces * obj->verts_per_face);
    // create new attributes entry for this object
    attribsGroupDescr_create_new_obj(obj->attribs_group_id, obj->id, obj->num_faces * obj->verts_per_face);


    // /// create shader build
    // build_t build_idx = shader_build_exists(sid_idx);
    // if(build_idx == INT_NULL) build_idx = shader_builder_create_new_build(sid_idx);
    // attribsGroupDescr_set_build_idx(attribs_group_id, build_idx);

// HERE:
//         1. store the attributes to the object attributes group buffer. DO NOT CREATE THE DESCRIPTOR HERE
//         2. store the uniforms in the uniforms group buffer.            DO NOT CREATE THE DESCRIPTOR HERE

    /**********************************************************************************************
     * ATTRIBUTES */
    struct attributes_group* attribs_group = attribsGroupDescr_get_attribs_group(attribs_group_id);

    uint32 attrib_size = num_vertices*attribs_stride*SIZE_F32;
    float* attribs_buffer_ptr = arena_allocator_alloc(attribs_group->attribs_data_id, attrib_size, SIZE_F32);
    memcpy(attribs_buffer_ptr, attribs_data, attrib_size);

// HERE: should run for user created descriptor to copy valules to the attributes group descreptor

    /// create shader attributes inputs,
    for(uint32 i = 0; i < obj->attrib_count; i++) {

        if((attribs_group->added_attributes_flag & attrib_desc[i].attrib_flag) == 0x0) {
            const attr_t attrib_idx = attribs_group->attribs_count;
            assert(attrib_idx < COMP_MAX_ATTRIBUTES);
            attribs_group->added_attributes_flag                       |= attrib_desc[i].attrib_flag;
            attribs_group->attribs_descr[attrib_idx].attrib_flag        = attrib_desc[i].attrib_flag;
            // attribs_group->attribs_descr[attrib_idx].attr_offset        = attrib_desc[i].attr_offset;
            attribs_group->attribs_descr[attrib_idx].attr_offset        = attr_offset;
            attribs_group->attribs_descr[attrib_idx].attr_num_elems     = attrib_desc[i].attr_num_elems;
            attribs_group->attribs_descr[attrib_idx].divisor            = attrib_desc[i].divisor;
            attribs_group->attribs_descr[attrib_idx].shader_input_flags = attrib_desc[i].shader_input_flags;
            attribs_group->attribs_count++;
            attr_offset += attrib_desc[i].attr_num_elems;
        }

        sid.attr |= attrib_desc[i].attrib_flag;
    }
    attribsGroupDescr_set_stride(attribs_group_id, attribs_stride);

    // TODO: add
    //     attribsGroupDescr_add_attributes(attribs_group_id, src, entry->elems, attribs_descr->attr_offset, entry->flag, divisor, shader_input_id);
    // and remove temp buffer memcpy

    /***********************************************************************************************
     * UNIFORMS */
    struct uniforms_group* unif_group = unifGroupDescr_get_unif_group(uniforms_group_id);
    uint32 unif_size = unif_stride * SIZE_F32;
    float* unif_buffer_ptr = arena_allocator_alloc(unif_group->unif_data_id, unif_size, SIZE_F32);
    memcpy(unif_buffer_ptr, uniforms_data, unif_size);
//
// TODO:
//     unifGroupDescr_add_uniforms(const attrid_t unif_idx, const float* const src,const uint32 elems_count, const sid_t unif_flag);
//     and remove temp buffer memcpy


    /***********************************************************************************************
     * TODO: SSBO's, UBO's */


    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");

    struct object* tri = obj;
    print("%s- Create object: triangle -%s\n"
          "   id:%u\n   attribs_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->attribs_group_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

    /* calculate and set command for renderqueue */
    struct command_render_queue_params rq_params = {
        .attribs_idx  = tri->attribs_group_id,
        .index_count  = num_objects * object_get_vertex_index_count(obj->id),
        .vertex_count = num_objects * object_get_vertex_count(obj->id)
    };
    cmd_add_command((void*) render_queue_add_draw_count, &rq_params, sizeof(struct command_render_queue_params));

    temp_fn(obj->id);

    /*  */
    // struct cmd_gfx_params gfx_params = {
    //     .sid_idx = attribs_group->sid_idx,
    //     .shader_attribs_idx = tri->attribs_group_id,
    //     .shader_attribs_idx = vao_attribs_idx,
    //     .build_idx = attribs_group->build_idx,
    //     .desc_idx = INT_NULL,
    //     .vpos_num_elems = mesh_get_num_elems_per_vertex(mesh_type),
    //     .unif_name = NULL,
    //     .unif_data = NULL
    // };
    //
    // if((attribs_group->commands_flag & CMD_CREATE_SHADER_PROGRAM)==0) {
    //
    //     build_t build_idx = gfx_program_exists(gfx_params.sid_idx);
    //     if(build_idx == INT_NULL) { // If shader program does not exist, create new program.
    //         opengl_program_create(gfx_params.sid_idx, gfx_params.build_idx, test_fragment_shader_text); GFX_CHECK_ERROR();
    //     }
    //
    //     attribs_group->commands_flag |= CMD_CREATE_SHADER_PROGRAM;
    //     gfx_params.desc_idx =  attribs_group->build_idx;
    // }
    //
    // struct shader_descriptor* shader_desc = shader_descriptor_get(gfx_params.desc_idx);
    // struct opengl_program* prog = shader_program_get(gfx_params.desc_idx);
    //
    // if((attribs_group->commands_flag & CMD_CREATE_VAO)==0) {
    //
    //     struct attributes_descriptor* vao_descr = attribsGroupDescr_get_attribs_descr(gfx_params.vao_attribs_idx);
    //     const uint32 stride = shader_desc->vshader_attribs_stride;
    //     gl_use_program(prog->program);
    //
    //     /* VERTEX ARRAY */
    //     const bool attrib_devisor = 0; /// TODO: attrib devisor should exist in object OR object component OR shader descriptor???
    //     bool is_new_vao = FALSE;
    //     vaidx_t vaoidx = gfx_vertex_array_find_match(shader_desc->sid_idx, attrib_devisor);
    //     if(vaoidx == INT_NULL) {
    //         vaoidx = gfx_vertex_array_create(shader_desc->build_idx);
    //         is_new_vao = TRUE;
    //     }
    //     shader_desc->vaoidx = vaoidx;
    //
    //
    //     struct vertex_array_object* va = gfx_vertex_array_get_byidx(vaoidx);
    //     gl_bind_vertex_array(va->vao);                                                              GFX_CHECK_ERROR();
    //
    //
    //     /* VERTEX BUFFER */
    //     vbidx_t vbidx = INT_NULL;
    //     struct vertex_buffer_object* vb = NULL;
    //     if(is_new_vao){
    //
    //         struct vertex_array_object* vao = gfx_vertex_array_get_byidx(vaoidx);
    //
    //         char buf[64] = {0}; char* ptr = &buf[0];
    //         uint32 size = string_format(&ptr, 64, "--- VAO:%2u ---\n", vao->idx);
    //         engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf, size);
    //
    //         /* Enable attributes in openGl shader */
    //         uint32 cnt = 0, vao_attr_desc_cnt = 0;
    //         attr_t divisor = INT_NULL;
    //         attr_t vb_stride = 0;
    //         for(uint32 i = 0; i < shader_desc->entry_count; i++){
    //
    //             if(shader_desc->entries[i].input_flag & IS_ATTR) {
    //
    //                 // create vertex buffer.
    //                 // 1. for each different divisor for instanced rendering
    //                 // 2. for per attribute buffer rendering
    //                 if(divisor != shader_desc->entries[i].divisor) {
    //                     vbidx = gfx_vertex_buffer_create(num_objects*num_vertices*stride*SIZE_F32, stride);
    //                     vao->vbos[cnt++] = vbidx;                                                       /// push new vertex buffer's index to VAO
    //
    //
    //                     vb = vertex_buffer_get_vbo(vbidx);
    //                     gl_bind_buffer(OPENGL_ARRAY_BUFFER, vb->vbo);                               GFX_CHECK_ERROR();
    //
    //                     divisor = shader_desc->entries[i].divisor;
    //
    //                     char buf2[64] = {0}; ptr = &buf[0];
    //                     uint32 size2 = string_format(&ptr, 64, " VBO:%2u\n", vbidx);
    //                     engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf2, size2);
    //
    //                     va->vbcount++;
    //                     vb_stride = 0;
    //                 }
    //
    //                 vb->v_layout[vb->attrib_count].loc        = shader_desc->v_layout[i].loc;
    //                 vb->v_layout[vb->attrib_count].num_elems  = shader_desc->v_layout[i].num_elems;
    //                 vb->v_layout[vb->attrib_count].type       = shader_desc->v_layout[i].type;
    //                 vb->v_layout[vb->attrib_count].attr_idx   = shader_desc->v_layout[i].attr_idx;             /// UNUSED for vertex buffers???
    //                 vb->v_layout[vb->attrib_count].loc_offset = shader_desc->v_layout[i].loc_offset;           /// UNUSED for vertex buffers???
    //                 vb->v_layout[vb->attrib_count].offset     = vao_descr[vao_attr_desc_cnt].attr_offset;
    //                 vb->v_layout[vb->attrib_count].idx        = shader_desc->v_layout[i].idx;                  /// UNUSED for vertex buffers???
    //                 vb_stride += shader_desc->v_layout[i].num_elems;
    //                 vb->stride = vb_stride;
    //
    //                 // @params: ( loc, num_elems, stride, offset, size, divisor )
    //                 shader_attribute_enable(
    //                     vb->v_layout[vb->attrib_count].loc,
    //                     shader_desc->v_layout[i].num_elems,
    //                     stride,
    //                     vb->v_layout[vb->attrib_count].offset,
    //                     SIZE_F32,
    //                     shader_desc->entries[i].divisor
    //                     );
    //
    //                 vb->attrib_count++;
    //
    //                 char buf2[128] = {0}; ptr = &buf[0];
    //                 uint32 size2 = string_format(&ptr, 128,
    //                     "[%2u]: "
    //                     "loc: %2u, "
    //                     "num_elems: %2u, "
    //                     "type: %s, "
    //                     "loc_offset: %2u, "
    //                     "offset: %2u, "
    //                     "\n",
    //                     shader_desc->v_layout[i].idx,
    //                     shader_desc->v_layout[i].loc,
    //                     shader_desc->v_layout[i].num_elems,
    //                     shaderinfo_type_to_glsltype_string(shader_desc->v_layout[i].type),
    //                     shader_desc->v_layout[i].loc_offset,
    //                     vao_descr[vao_attr_desc_cnt].attr_offset
    //                 );
    //                 engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf2, size2);
    //
    //                 vao_attr_desc_cnt++;
    //             }
    //         }
    //
    //         // shader_attributes_enable(desc);
    //         opengl_debug_attributes_in(shader_desc->vshader_attrib_locations_count);                GFX_CHECK_ERROR();
    //         attribs_group->rqidx = renderer_renderqueue_add(gfx_params.sid_idx, prog->program, shader_desc->build_idx,
    //                                                         va->vao, va->idx, vb->vbo, vb->idx);
    //     } else {
    //         vbidx = gfx_vertex_buffer_find_match(attribs_group->sid_idx);
    //         vb = vertex_buffer_get_vbo(vbidx);
    //     }
    //     assert(vbidx != INT_NULL);
    //
    //
    //
    //     /* VERTEX INDEX BUFFER */
    //     ibidx_t ibidx = INT_NULL;
    //     struct vertex_index_buffer_object* ib = NULL;
    //     if(is_new_vao){
    //         /// TODO: calculate the size of the vertex buffer OR use dynamic array vertex buffer
    //         ibidx = gfx_vertex_index_buffer_create(200*SIZE_I32);
    //
    //         ib = vertex_index_buffer_get_ib(ibidx);
    //         gl_bind_buffer(OPENGL_ELEMENT_ARRAY_BUFFER, ib->ibo);                                   GFX_CHECK_ERROR();
    //
    //     } else {
    //         assert(0>0);
    //         // vbidx = gfx_vertex_buffer_find_match(attribs_desc->sid_idx);
    //         // vb = vertex_buffer_get_vbo(vbidx);
    //     }
    //     assert(ibidx != INT_NULL);
    //
    //     /* SET COMMANDS */
    //     void* p = NULL;
    //     /// SET COMMAND: IB_SET_DATA
    //     struct ib_cmd_params ind_args = { .ibidx = ib->idx, .vpos_num_elems = gfx_params.vpos_num_elems, .attribs_idx = gfx_params.shader_attribs_idx};
    //     p = &ind_args;
    //     cmd_add_command((void*)gfx_vertex_index_buffer_set_data, p, sizeof(struct ib_cmd_params));
    //     // SET COMMAND: IB_UPDATE_DATA
    //     cmd_add_command((void*) gfx_vertex_index_buffer_update_data, p, sizeof(struct ib_cmd_params));
    //
    //
    //     /// SET COMMAND: VB_SET_DATA
    //     struct vb_cmd_params vert_args = { .vaidx = vaoidx, .vbidx = vb->idx, .vpos_num_elems = gfx_params.vpos_num_elems, .shader_attribs_idx = gfx_params.shader_attribs_idx};
    //     p = &vert_args;
    //     // cmd_add_command((void*)gfx_vertex_buffer_set_data, p, sizeof(struct vb_cmd_params));
    //     cmd_add_command((void*)gfx_vertex_buffer_set_data2, p, sizeof(struct vb_cmd_params));
    //     // SET COMMAND: VB_UPDATE_DATA
    //     cmd_add_command((void*) gfx_vertex_buffer_update_data, p, sizeof(struct vb_cmd_params));
    //
    //     gl_use_program(0x0);
    //     gl_bind_vertex_array(0x0);
    //     gl_bind_buffer(OPENGL_ARRAY_BUFFER, 0x0);
    //
    //
    //     /// close file for debug
    //     engine_fdebug_append_to_file_from_arena(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT);
    //     attribs_group->commands_flag |= CMD_CREATE_VAO;
    // }
    //
    // /* Set and update shader uniform inputs */
    // gl_use_program(prog->program);
    //
    // for(uint32 i = 0; i < num_uniforms; i++) {
    //
    //     const float* data = &uniforms_data[unif_desc[i].unif_offset];
    //     union uniform_data params = {
    //         .buffer = { .count = 1, .data = data, .transpose = FALSE },
    //     };
    //     unif_desc->update_fn.fnptr(&params, unif_desc->loc);                                        GFX_CHECK_ERROR();
    // }
    //
    // gl_use_program(0x0);


}
/*SAVE*/
// void render_tri(struct camera* camera)
// {
//     /*
//      * USER MUST MATCH THE ATTRIBUTES DESCRIPTOR WITH THE ATTRIBUTES BUFFER
//      */
//     /// create attrib buffer:
//     uint32 attr_offset = 0;
//     struct attributes_descriptor desc[] = {
//         {.attr_offset = 0, .attr_num_elems = 2, .divisor = 0, .attrib_flag = SID_VPOSI2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS},
//         {.attr_offset = 0, .attr_num_elems = 4, .divisor = 0, .attrib_flag = SID_COLOR4, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_OUT},
//         {.attr_offset = 0, .attr_num_elems = 3, .divisor = 0, .attrib_flag = SID_WPOSI3, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS},
//         {.attr_offset = 0, .attr_num_elems = 2, .divisor = 0, .attrib_flag = SID_SCALE2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE},
//         {.attr_offset = 0, .attr_num_elems = 2, .divisor = 0, .attrib_flag = SID_ROTAT2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT},
//     };
//
//     const float z = 10., inten = 1., defi = 0.0;
//     float attribs_data[] = {
//         // color4              wpos3         scale2     rot2
//         inten,defi,defi,1.0,   0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v0
//         defi,inten,defi,1.0,   0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v1
//         defi,defi,inten,1.0,   0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v2
//     };
//     // float attribs[] = {
//     //     // wpos3      scale2     rot2                  color4
//     //     0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0),    inten,defi,defi,1.0,    // v0
//     //     0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0),    defi,inten,defi,1.0,    // v1
//     //     0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0),    defi,defi,inten,1.0,    // v2
//     // };
//     // float attribs[] = {
//     //     // color4              wpos3         scale2     rot2
//     //     0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v0
//     //     0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v1
//     //     0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v2
//     // };
//     // float attribs[] = {
//     //     // color4              wpos3         scale2     rot2
//     //     0.0,0.0,-z, inten,defi,defi,1.0, // v0
//     //     0.0,0.0,-z, defi,inten,defi,1.0, // v1
//     //     0.0,0.0,-z, defi,defi,inten,1.0, // v2
//     // };
//     uint32 num_objects = 1;
//     uint32 num_vertices = 3;
//     uint32 attribs_stride = 2+4+3+2+2;
//     uint32 num_attributes = ARRAY_LENGTH(desc);
//
//
//     float color[] = {0.9, 0.5, 0.1, 1.0};
//     float scale[] = {1.0, 1.0};
//     float rot[]   = {cos(0.0), sin(0.0)};
//
//     struct uniform_descriptor unif_desc[] = {
//         // {.unif_num_elems = 2, .unif_flag = SID_SCALE2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE},
//         // {.unif_num_elems = 2, .unif_flag = SID_ROTAT2, .shader_input_flags = IN_VSHADER | IS_UNIF | IS_TRANSFORM_ROT},
//         // {.unif_num_elems = 4, .unif_flag = SID_COLOR4, .shader_input_flags = IN_VSHADER | IS_UNIF | IS_OUT},
//         {.loc = INT_NULL, .unif_offset = 0, .unif_num_elems = 4*4, .unif_flag = SID_PROJMAT4X4, .update_fn = &gl_uniform_matrix4fv, .shader_input_flags = IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ},
//     };
//     uint32 unif_stride = (4*4);
//     uint32 num_uniforms = ARRAY_LENGTH(unif_desc);
//     /// copy each uniform data to the contiguous uniforms buffer data
//     float* temp[num_uniforms];
//     temp[0] = &camera->projection_matrix.projection_matrix[0];
//     float uniforms_data[unif_stride*SIZE_F32] = {};
//     for(uint32 i = 0; i < num_uniforms; i++) {
//         memcpy(uniforms_data, &temp[i][0], unif_stride*SIZE_F32);
//     }
//
//     /// create object
//     struct gfx_sid sid = {0,0,0,0,0};
//     sid.out  |= SID_COLOR4;
//     sid.pass |= SID_COLOR4;
//     sid.oper |= SID_NULL;
//
//     enum MESH_TYPE mesh_type = MESH_TRIANGLE;
//
//     /// create shader id if does not exist
//     sididx_t sid_idx = gfx_sid_find_match(sid);
//     if(sid_idx != INT_NULL) {
//         sid_idx = gfx_create_sid(sid);
//     }
//
//     /// create attributes group if does not exist
//     attrid_t attribs_group_id = attribsGroupDescr_group_find_match(sid_idx);
//     if(attribs_group_id == INT_NULL) {
//         attribs_group_id = attribsGroupDescr_create_new_attribs_group(sid_idx);
//     }
//
//     /// create uniforms group if does not exist
//     attrid_t uniforms_group_id = unifGroupDescr_group_find_match(sid_idx);
//     if(uniforms_group_id == INT_NULL) {
//         uniforms_group_id = unifGroupDescr_create_new_unif_group(sid_idx);
//     }
//
//     /// create object
//     struct object* obj  = object_create(sid_idx, mesh_type);
//     obj->num_faces      = 1;
//     obj->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
//     obj->index_count    = mesh_get_mesh_indices_count_from_type(mesh_type);
//     obj->attribs_group_id = attribs_group_id;
//     obj->unifs_group_id   = uniforms_group_id;
//     object_set_vertex_count(obj->id, obj->num_faces * obj->verts_per_face);
//     // create new attributes entry for this object
//     attribsGroupDescr_create_new_obj(obj->attribs_group_id, obj->id, obj->num_faces * obj->verts_per_face);
//
//
//     /// create shader build
//     build_t build_idx = shader_build_exists(sid_idx);
//     if(build_idx == INT_NULL) build_idx = shader_builder_create_new_build(sid_idx);
//     attribsGroupDescr_set_build_idx(attribs_group_id, build_idx);
//
// // HERE:
// //         1. store the attributes to the object attributes group buffer. DO NOT CREATE THE DESCRIPTOR HERE
// //         2. store the uniforms in the uniforms group buffer.            DO NOT CREATE THE DESCRIPTOR HERE
//
//     /**********************************************************************************************
//      * ATTRIBUTES */
//     struct attributes_group* attribs_group = attribsGroupDescr_get_attribs_group(attribs_group_id);
//
//     uint32 attrib_size = num_vertices*attribs_stride*SIZE_F32;
//     float* attribs_buffer_ptr = arena_allocator_alloc(attribs_group->attribs_data_id, attrib_size, SIZE_F32);
//     memcpy(attribs_buffer_ptr, attribs_data, attrib_size);
//
//     /// build attributes group descriptor
//     for(uint32 i = 0; i < num_attributes; i++) {
//
//         if((attribs_group->added_attributes_flag & desc[i].attrib_flag) == 0x0) {
//             const attr_t attrib_idx = attribs_group->attribs_count;
//             assert(attrib_idx < COMP_MAX_ATTRIBUTES);
//             attribs_group->added_attributes_flag                       |= desc[i].attrib_flag;
//             attribs_group->attribs_descr[attrib_idx].attrib_flag        = desc[i].attrib_flag;
//             // attribs_group->attribs_descr[attrib_idx].attr_offset        = desc[i].attr_offset;
//             attribs_group->attribs_descr[attrib_idx].attr_offset        = attr_offset;
//             attribs_group->attribs_descr[attrib_idx].attr_num_elems     = desc[i].attr_num_elems;
//             attribs_group->attribs_descr[attrib_idx].divisor            = desc[i].divisor;
//             attribs_group->attribs_descr[attrib_idx].shader_input_flags = desc[i].shader_input_flags;
//             attribs_group->attribs_count++;
//             attr_offset += desc[i].attr_num_elems;
//         }
//
//         const attr_t shader_input_id = shaderinfo_flag_to_id(desc[i].attrib_flag);
//         shader_create_input(build_idx, shader_input_id, desc[i].shader_input_flags, desc[i].divisor);
//         sid.attr |= desc[i].attrib_flag;
//     }
//     attribsGroupDescr_set_stride(attribs_group_id, attribs_stride);
//
//
//     /***********************************************************************************************
//      * UNIFORMS */
//     struct uniforms_group* unif_group = unifGroupDescr_get_unif_group(uniforms_group_id);
//     uint32 unif_size = unif_stride * SIZE_F32;
//     float* unif_buffer_ptr = arena_allocator_alloc(unif_group->unif_data_id, unif_size, SIZE_F32);
//     memcpy(unif_buffer_ptr, uniforms_data, unif_size);
//
//     for(uint32 i = 0; i < num_uniforms; i++) {
//
//         const attr_t shader_input_id = shaderinfo_flag_to_id(unif_desc[i].unif_flag);
//         shader_create_input(build_idx, shader_input_id, unif_desc[i].shader_input_flags, 0);
//         sid.unif |= unif_desc[i].unif_flag;
//         unif_desc[i].loc = i;
//     }
//
//
//     /***********************************************************************************************
//      * TODO: SSBO's, UBO's */
//
//
//     engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
//     const attr_t vao_attribs_idx = 0;
//
//     struct object* tri = obj;
//     print("%s- Create object: triangle -%s\n"
//           "   id:%u\n   attribs_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
//           BLUE, CLEAR, obj->id, obj->attribs_group_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
//
//     /* calculate and set command for renderqueue */
//     struct command_render_queue_params rq_params = {
//         .attribs_idx  = tri->attribs_group_id,
//         .index_count  = num_objects * object_get_vertex_index_count(obj->id),
//         .vertex_count = num_objects * object_get_vertex_count(obj->id)
//     };
//     cmd_add_command((void*) render_queue_add_draw_count, &rq_params, sizeof(struct command_render_queue_params));
//
//     /*  */
//     struct cmd_gfx_params gfx_params = {
//         .sid_idx = attribs_group->sid_idx,
//         .shader_attribs_idx = tri->attribs_group_id,
//         .shader_attribs_idx = vao_attribs_idx,
//         .build_idx = attribs_group->build_idx,
//         .desc_idx = INT_NULL,
//         .vpos_num_elems = mesh_get_num_elems_per_vertex(mesh_type),
//         .unif_name = NULL,
//         .unif_data = NULL
//     };
//
//     if((attribs_group->commands_flag & CMD_CREATE_SHADER_PROGRAM)==0) {
//
//         build_t build_idx = gfx_program_exists(gfx_params.sid_idx);
//         if(build_idx == INT_NULL) { // If shader program does not exist, create new program.
//             opengl_program_create(gfx_params.sid_idx, gfx_params.build_idx, test_fragment_shader_text); GFX_CHECK_ERROR();
//         }
//
//         attribs_group->commands_flag |= CMD_CREATE_SHADER_PROGRAM;
//         gfx_params.desc_idx =  attribs_group->build_idx;
//     }
//
//     struct shader_descriptor* shader_desc = shader_descriptor_get(gfx_params.desc_idx);
//     struct opengl_program* prog = shader_program_get(gfx_params.desc_idx);
//
//     if((attribs_group->commands_flag & CMD_CREATE_VAO)==0) {
//
//         struct attributes_descriptor* vao_descr = attribsGroupDescr_get_attribs_descr(gfx_params.vao_attribs_idx);
//         const uint32 stride = shader_desc->vshader_attribs_stride;
//         gl_use_program(prog->program);
//
//         /* VERTEX ARRAY */
//         const bool attrib_devisor = 0; /// TODO: attrib devisor should exist in object OR object component OR shader descriptor???
//         bool is_new_vao = FALSE;
//         vaidx_t vaoidx = gfx_vertex_array_find_match(shader_desc->sid_idx, attrib_devisor);
//         if(vaoidx == INT_NULL) {
//             vaoidx = gfx_vertex_array_create(shader_desc->build_idx);
//             is_new_vao = TRUE;
//         }
//         shader_desc->vaoidx = vaoidx;
//
//
//         struct vertex_array_object* va = gfx_vertex_array_get_byidx(vaoidx);
//         gl_bind_vertex_array(va->vao);                                                              GFX_CHECK_ERROR();
//
//
//         /* VERTEX BUFFER */
//         vbidx_t vbidx = INT_NULL;
//         struct vertex_buffer_object* vb = NULL;
//         if(is_new_vao){
//
//             struct vertex_array_object* vao = gfx_vertex_array_get_byidx(vaoidx);
//
//             char buf[64] = {0}; char* ptr = &buf[0];
//             uint32 size = string_format(&ptr, 64, "--- VAO:%2u ---\n", vao->idx);
//             engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf, size);
//
//             /* Enable attributes in openGl shader */
//             uint32 cnt = 0, vao_attr_desc_cnt = 0;
//             attr_t divisor = INT_NULL;
//             attr_t vb_stride = 0;
//             for(uint32 i = 0; i < shader_desc->entry_count; i++){
//
//                 if(shader_desc->entries[i].input_flag & IS_ATTR) {
//
//                     // create vertex buffer.
//                     // 1. for each different divisor for instanced rendering
//                     // 2. for per attribute buffer rendering
//                     if(divisor != shader_desc->entries[i].divisor) {
//                         vbidx = gfx_vertex_buffer_create(num_objects*num_vertices*stride*SIZE_F32, stride);
//                         vao->vbos[cnt++] = vbidx;                                                       /// push new vertex buffer's index to VAO
//
//
//                         vb = vertex_buffer_get_vbo(vbidx);
//                         gl_bind_buffer(OPENGL_ARRAY_BUFFER, vb->vbo);                               GFX_CHECK_ERROR();
//
//                         divisor = shader_desc->entries[i].divisor;
//
//                         char buf2[64] = {0}; ptr = &buf[0];
//                         uint32 size2 = string_format(&ptr, 64, " VBO:%2u\n", vbidx);
//                         engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf2, size2);
//
//                         va->vbcount++;
//                         vb_stride = 0;
//                     }
//
//                     vb->v_layout[vb->attrib_count].loc        = shader_desc->v_layout[i].loc;
//                     vb->v_layout[vb->attrib_count].num_elems  = shader_desc->v_layout[i].num_elems;
//                     vb->v_layout[vb->attrib_count].type       = shader_desc->v_layout[i].type;
//                     vb->v_layout[vb->attrib_count].attr_idx   = shader_desc->v_layout[i].attr_idx;             /// UNUSED for vertex buffers???
//                     vb->v_layout[vb->attrib_count].loc_offset = shader_desc->v_layout[i].loc_offset;           /// UNUSED for vertex buffers???
//                     vb->v_layout[vb->attrib_count].offset     = vao_descr[vao_attr_desc_cnt].attr_offset;
//                     vb->v_layout[vb->attrib_count].idx        = shader_desc->v_layout[i].idx;                  /// UNUSED for vertex buffers???
//                     vb_stride += shader_desc->v_layout[i].num_elems;
//                     vb->stride = vb_stride;
//
//                     // @params: ( loc, num_elems, stride, offset, size, divisor )
//                     shader_attribute_enable(
//                         vb->v_layout[vb->attrib_count].loc,
//                         shader_desc->v_layout[i].num_elems,
//                         stride,
//                         vb->v_layout[vb->attrib_count].offset,
//                         SIZE_F32,
//                         shader_desc->entries[i].divisor
//                         );
//
//                     vb->attrib_count++;
//
//                     char buf2[128] = {0}; ptr = &buf[0];
//                     uint32 size2 = string_format(&ptr, 128,
//                         "[%2u]: "
//                         "loc: %2u, "
//                         "num_elems: %2u, "
//                         "type: %s, "
//                         "loc_offset: %2u, "
//                         "offset: %2u, "
//                         "\n",
//                         shader_desc->v_layout[i].idx,
//                         shader_desc->v_layout[i].loc,
//                         shader_desc->v_layout[i].num_elems,
//                         shaderinfo_type_to_glsltype_string(shader_desc->v_layout[i].type),
//                         shader_desc->v_layout[i].loc_offset,
//                         vao_descr[vao_attr_desc_cnt].attr_offset
//                     );
//                     engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf2, size2);
//
//                     vao_attr_desc_cnt++;
//                 }
//             }
//
//             // shader_attributes_enable(desc);
//             opengl_debug_attributes_in(shader_desc->vshader_attrib_locations_count);                GFX_CHECK_ERROR();
//             attribs_group->rqidx = renderer_renderqueue_add(gfx_params.sid_idx, prog->program, shader_desc->build_idx,
//                                                             va->vao, va->idx, vb->vbo, vb->idx);
//         } else {
//             vbidx = gfx_vertex_buffer_find_match(attribs_group->sid_idx);
//             vb = vertex_buffer_get_vbo(vbidx);
//         }
//         assert(vbidx != INT_NULL);
//
//
//
//         /* VERTEX INDEX BUFFER */
//         ibidx_t ibidx = INT_NULL;
//         struct vertex_index_buffer_object* ib = NULL;
//         if(is_new_vao){
//             /// TODO: calculate the size of the vertex buffer OR use dynamic array vertex buffer
//             ibidx = gfx_vertex_index_buffer_create(200*SIZE_I32);
//
//             ib = vertex_index_buffer_get_ib(ibidx);
//             gl_bind_buffer(OPENGL_ELEMENT_ARRAY_BUFFER, ib->ibo);                                   GFX_CHECK_ERROR();
//
//         } else {
//             assert(0>0);
//             // vbidx = gfx_vertex_buffer_find_match(attribs_desc->sid_idx);
//             // vb = vertex_buffer_get_vbo(vbidx);
//         }
//         assert(ibidx != INT_NULL);
//
//         /* SET COMMANDS */
//         void* p = NULL;
//         /// SET COMMAND: IB_SET_DATA
//         struct ib_cmd_params ind_args = { .ibidx = ib->idx, .vpos_num_elems = gfx_params.vpos_num_elems, .attribs_idx = gfx_params.shader_attribs_idx};
//         p = &ind_args;
//         cmd_add_command((void*)gfx_vertex_index_buffer_set_data, p, sizeof(struct ib_cmd_params));
//         // SET COMMAND: IB_UPDATE_DATA
//         cmd_add_command((void*) gfx_vertex_index_buffer_update_data, p, sizeof(struct ib_cmd_params));
//
//
//         /// SET COMMAND: VB_SET_DATA
//         struct vb_cmd_params vert_args = { .vaidx = vaoidx, .vbidx = vb->idx, .vpos_num_elems = gfx_params.vpos_num_elems, .shader_attribs_idx = gfx_params.shader_attribs_idx};
//         p = &vert_args;
//         // cmd_add_command((void*)gfx_vertex_buffer_set_data, p, sizeof(struct vb_cmd_params));
//         cmd_add_command((void*)gfx_vertex_buffer_set_data2, p, sizeof(struct vb_cmd_params));
//         // SET COMMAND: VB_UPDATE_DATA
//         cmd_add_command((void*) gfx_vertex_buffer_update_data, p, sizeof(struct vb_cmd_params));
//
//         gl_use_program(0x0);
//         gl_bind_vertex_array(0x0);
//         gl_bind_buffer(OPENGL_ARRAY_BUFFER, 0x0);
//
//
//         /// close file for debug
//         engine_fdebug_append_to_file_from_arena(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT);
//         attribs_group->commands_flag |= CMD_CREATE_VAO;
//     }
//
//     /* Set and update shader uniform inputs */
//     gl_use_program(prog->program);
//
//     for(uint32 i = 0; i < num_uniforms; i++) {
//
//         const float* data = &uniforms_data[unif_desc[i].unif_offset];
//         union uniform_data params = {
//             .buffer = { .count = 1, .data = data, .transpose = FALSE },
//         };
//         unif_desc->update_fn.fnptr(&params, unif_desc->loc);                                        GFX_CHECK_ERROR();
//     }
//
//     gl_use_program(0x0);
//
//
// }

void render_tri_many(struct camera* camera)
{
    /*
     * USER MUST MATCH THE ATTRIBUTES DESCRIPTOR WITH THE ATTRIBUTES BUFFER
     */
    /// create attrib buffer:
    uint32 attr_offset = 0;
    struct attributes_descriptor desc[] = {
        {.attr_offset = 0, .attr_num_elems = 2, .divisor = 0, .attrib_flag = SID_VPOSI2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS},
        {.attr_offset = 0, .attr_num_elems = 4, .divisor = 0, .attrib_flag = SID_COLOR4, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_OUT},
        {.attr_offset = 0, .attr_num_elems = 3, .divisor = 0, .attrib_flag = SID_WPOSI3, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS},
        {.attr_offset = 0, .attr_num_elems = 2, .divisor = 0, .attrib_flag = SID_SCALE2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE},
        {.attr_offset = 0, .attr_num_elems = 2, .divisor = 0, .attrib_flag = SID_ROTAT2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT},
    };

    const float z = 10., inten = 1., defi = 0.0;
    float attribs_data[] = {
        // color4              wpos3         scale2     rot2
        inten,defi,defi,1.0,   0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v0
        defi,inten,defi,1.0,   0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v1
        defi,defi,inten,1.0,   0.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v2
        // color4              wpos3         scale2     rot2
        inten,defi,defi,1.0,   2.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v0
        defi,inten,defi,1.0,   2.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v1
        defi,defi,inten,1.0,   2.0,0.0,-z,   1.0,1.0,   cos(0.0),sin(0.0), // v2
    };

    uint32 num_objects = 2;
    uint32 num_vertices = 3;
    uint32 attribs_stride = 2+4+3+2+2;
    uint32 num_attributes = ARRAY_LENGTH(desc);

    struct uniform_descriptor unif_desc[] = {
        // {.loc = INT_NULL, .unif_offset = 0, .unif_num_elems = 4,   .unif_flag = SID_COLOR4,     .update_fn = &gl_uniform4fv,        .shader_input_flags = IN_VSHADER | IS_UNIF | IS_OUT},
        {.loc = INT_NULL, .unif_offset = 0, .unif_num_elems = 4*4, .unif_flag = SID_PROJMAT4X4, .update_fn = &gl_uniform_matrix4fv, .shader_input_flags = IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ},
    };
    uint32 unif_stride = (4*4);
    uint32 num_uniforms = ARRAY_LENGTH(unif_desc);
    /// copy each uniform data to the contiguous uniforms buffer data
    float* temp[num_uniforms];
    temp[0] = &camera->projection_matrix.projection_matrix[0];
    float uniforms_data[unif_stride*SIZE_F32] = {};
    for(uint32 i = 0; i < num_uniforms; i++) {
        memcpy(uniforms_data, &temp[i][0], unif_stride*SIZE_F32);
    }

    /// create object
    struct gfx_sid sid = {0,0,0,0,0};
    sid.out  |= SID_COLOR4;
    sid.pass |= SID_COLOR4;
    sid.oper |= SID_NULL;

    enum MESH_TYPE mesh_type = MESH_TRIANGLE;

    /// create shader id if does not exist
    sididx_t sid_idx = gfx_sid_find_match(sid);
    if(sid_idx != INT_NULL) {
        sid_idx = gfx_create_sid(sid);
    }

    /// create attributes group if does not exist
    attrid_t attribs_group_id = attribsGroupDescr_group_find_match(sid_idx);
    if(attribs_group_id == INT_NULL) {
        attribs_group_id = attribsGroupDescr_create_new_attribs_group(sid_idx);
    }

    /// create uniforms group if does not exist
    attrid_t uniforms_group_id = unifGroupDescr_group_find_match(sid_idx);
    if(uniforms_group_id == INT_NULL) {
        uniforms_group_id = unifGroupDescr_create_new_unif_group(sid_idx);
    }

    /// create object
    struct object* obj  = object_create(sid_idx, mesh_type);
    obj->num_faces      = 1;
    obj->attrib_count   = num_attributes;
    obj->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
    obj->index_count    = mesh_get_mesh_indices_count_from_type(mesh_type);
    obj->attribs_group_id = attribs_group_id;
    object_set_vertex_count(obj->id, obj->num_faces * obj->verts_per_face);
    // create new object attributes entry to attributes buffer
    attribsGroupDescr_create_new_obj(obj->attribs_group_id, obj->id, obj->num_faces * obj->verts_per_face);

    struct object* obj2  = object_create(sid_idx, mesh_type);
    obj2->num_faces      = 1;
    obj2->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
    obj2->index_count    = mesh_get_mesh_indices_count_from_type(mesh_type);
    obj2->attribs_group_id = attribs_group_id;
    object_set_vertex_count(obj2->id, obj2->num_faces * obj2->verts_per_face);
    // create new object attributes entry to attributes buffer
    attribsGroupDescr_create_new_obj(obj2->attribs_group_id, obj2->id, obj2->num_faces * obj2->verts_per_face);

    /// create shader build
    build_t build_idx = shader_build_exists(sid_idx);
    if(build_idx == INT_NULL) build_idx = shader_builder_create_new_build(sid_idx);
    attribsGroupDescr_set_build_idx(attribs_group_id, build_idx);


    /**********************************************************************************************
     * ATTRIBUTES */
    struct attributes_group* attribs_group = attribsGroupDescr_get_attribs_group(attribs_group_id);

    uint32 attrib_size = num_objects*num_vertices*attribs_stride*SIZE_F32;
    float* attribs_buffer_ptr = arena_allocator_alloc(attribs_group->attribs_data_id, attrib_size, SIZE_F32);
    memcpy(attribs_buffer_ptr, attribs_data, attrib_size);


    // build attributes group descriptor
    for(uint32 i = 0; i < num_attributes; i++) {

        if(((attribs_group->added_attributes_flag & desc[i].attrib_flag) == 0x0) && (desc[i].shader_input_flags & IS_ATTR)) {

            const attr_t shader_input_id = shaderinfo_flag_to_id(desc[i].attrib_flag);
            shader_create_input(build_idx, shader_input_id, desc[i].shader_input_flags, desc[i].divisor);

            const attr_t attrib_idx = attribs_group->attribs_count;
            assert(attrib_idx < COMP_MAX_ATTRIBUTES);
            attribs_group->added_attributes_flag                       |= desc[i].attrib_flag;
            attribs_group->attribs_descr[attrib_idx].attrib_flag        = desc[i].attrib_flag;
            attribs_group->attribs_descr[attrib_idx].attr_offset        = attr_offset;
            attribs_group->attribs_descr[attrib_idx].attr_num_elems     = desc[i].attr_num_elems;
            attribs_group->attribs_descr[attrib_idx].divisor            = desc[i].divisor;
            attribs_group->attribs_descr[attrib_idx].shader_input_flags = desc[i].shader_input_flags;
            attribs_group->attribs_count++;
            attr_offset += desc[i].attr_num_elems;
// HERE: we must deal with vpos and the offsets that will start from 0 in the attribs_group buffer

            sid.attr |= desc[i].attrib_flag;
            //
            // if(i != 0) {
            //     int attrib_start = attribs_group->attribs_descr[attrib_idx].attr_offset;
            //     float* data = &attribs_data[attrib_start];
            //     attribsGroupDescr_add_attributes(attribs_group_id, data, desc[i].attr_num_elems,
            //             attrib_start, desc[i].attrib_flag, desc[i].divisor, shader_input_id);
            // }

        }


    }



    /***********************************************************************************************
     * UNIFORMS */
    struct uniforms_group* unif_group = unifGroupDescr_get_unif_group(uniforms_group_id);
    uint32 unif_size = unif_stride * SIZE_F32;
    float* unif_buffer_ptr = arena_allocator_alloc(unif_group->unif_data_id, unif_size, SIZE_F32);
    memcpy(unif_buffer_ptr, uniforms_data, unif_size);

    for(uint32 i = 0; i < num_uniforms; i++) {

        const attr_t shader_input_id = shaderinfo_flag_to_id(unif_desc[i].unif_flag);
        shader_create_input(build_idx, shader_input_id, unif_desc[i].shader_input_flags, 0);
        sid.unif |= unif_desc[i].unif_flag;

        int unif_start = unif_desc[i].unif_offset;
        float* data = &uniforms_data[unif_start];
        unifGroupDescr_add_uniforms(i, data, unif_desc[i].unif_num_elems, unif_desc[i].unif_flag);

        unif_desc[i].loc = i;

    }


    /***********************************************************************************************
     * TODO: SSBO's, UBO's */


    /*  */
    attribsGroupDescr_set_stride(attribs_group_id, attribs_stride);

    print("%s- Create object: triangle -%s\n"
          "   id:%u\n   attribs_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->attribs_group_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);
    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");

    struct object* tri = obj;

    /*  */
    struct cmd_gfx_params gfx_params = {
        .sid_idx = attribs_group->sid_idx,
        .attribs_group_desc_idx = tri->attribs_group_id,
        .build_idx = attribs_group->build_idx,
        .desc_idx = INT_NULL,
        .vpos_num_elems = mesh_get_num_elems_per_vertex(mesh_type),
        .unif_name = NULL,
        .unif_data = NULL
    };

    if((attribs_group->commands_flag & CMD_CREATE_SHADER_PROGRAM)==0) {

        build_t build_idx = gfx_program_exists(gfx_params.sid_idx);
        if(build_idx == INT_NULL) { // If shader program does not exist, create new program.
            opengl_program_create(gfx_params.sid_idx, gfx_params.build_idx, test_fragment_shader_text); GFX_CHECK_ERROR();
        }

        attribs_group->commands_flag |= CMD_CREATE_SHADER_PROGRAM;
        gfx_params.desc_idx =  attribs_group->build_idx;
    }

    struct shader_descriptor* shader_desc = shader_descriptor_get(gfx_params.desc_idx);
    struct opengl_program* prog = shader_program_get(gfx_params.desc_idx);

    if((attribs_group->commands_flag & CMD_CREATE_VAO)==0) {

        struct attributes_descriptor* vao_descr = attribsGroupDescr_get_attribs_descr(gfx_params.attribs_group_desc_idx);
        const uint32 stride = shader_desc->vshader_attribs_stride;
        gl_use_program(prog->program);

        /* VERTEX ARRAY */
        const bool attrib_devisor = 0; /// TODO: attrib devisor should exist in object OR object component OR shader descriptor???
        bool is_new_vao = FALSE;
        vaidx_t vaoidx = gfx_vertex_array_find_match(shader_desc->sid_idx, attrib_devisor);
        if(vaoidx == INT_NULL) {
            vaoidx = gfx_vertex_array_create(shader_desc->build_idx);
            is_new_vao = TRUE;
        }
        shader_desc->vaoidx = vaoidx;


        struct vertex_array_object* va = gfx_vertex_array_get_byidx(vaoidx);
        gl_bind_vertex_array(va->vao);                                                              GFX_CHECK_ERROR();


        /* VERTEX BUFFER */
        vbidx_t vbidx = INT_NULL;
        struct vertex_buffer_object* vb = NULL;
        if(is_new_vao){

            struct vertex_array_object* vao = gfx_vertex_array_get_byidx(vaoidx);

            char buf[64] = {0}; char* ptr = &buf[0];
            uint32 size = string_format(&ptr, 64, "--- VAO:%2u ---\n", vao->idx);
            engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf, size);

            /* Enable attributes in openGl shader */
            uint32 cnt = 0, vao_attr_desc_cnt = 0;
            attr_t divisor = INT_NULL;
            attr_t vb_stride = 0;
            for(uint32 i = 0; i < shader_desc->entry_count; i++){

                if(shader_desc->entries[i].input_flag & IS_ATTR) {

                    // create vertex buffer.
                    // 1. for each different divisor for instanced rendering
                    // 2. for per attribute buffer rendering
                    if(divisor != shader_desc->entries[i].divisor) {
                        vbidx = gfx_vertex_buffer_create(num_objects*num_vertices*stride*SIZE_F32, stride);
                        vao->vbos[cnt++] = vbidx;                                                       /// push new vertex buffer's index to VAO


                        vb = vertex_buffer_get_vbo(vbidx);
                        gl_bind_buffer(OPENGL_ARRAY_BUFFER, vb->vbo);                               GFX_CHECK_ERROR();

                        divisor = shader_desc->entries[i].divisor;

                        char buf2[64] = {0}; ptr = &buf[0];
                        uint32 size2 = string_format(&ptr, 64, " VBO:%2u\n", vbidx);
                        engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf2, size2);

                        va->vbcount++;
                        vb_stride = 0;
                    }

                    vb->v_layout[vb->attrib_count].loc        = shader_desc->v_layout[i].loc;
                    vb->v_layout[vb->attrib_count].num_elems  = shader_desc->v_layout[i].num_elems;
                    vb->v_layout[vb->attrib_count].type       = shader_desc->v_layout[i].type;
                    vb->v_layout[vb->attrib_count].attr_idx   = shader_desc->v_layout[i].attr_idx;             /// UNUSED for vertex buffers???
                    vb->v_layout[vb->attrib_count].loc_offset = shader_desc->v_layout[i].loc_offset;           /// UNUSED for vertex buffers???
                    vb->v_layout[vb->attrib_count].offset     = vao_descr[vao_attr_desc_cnt].attr_offset;
                    vb->v_layout[vb->attrib_count].idx        = shader_desc->v_layout[i].idx;                  /// UNUSED for vertex buffers???
                    vb_stride += shader_desc->v_layout[i].num_elems;
                    vb->stride = vb_stride;

                    // @params: ( loc, num_elems, stride, offset, size, divisor )
                    shader_attribute_enable(
                        vb->v_layout[vb->attrib_count].loc,
                        shader_desc->v_layout[i].num_elems,
                        stride,
                        vb->v_layout[vb->attrib_count].offset,
                        SIZE_F32,
                        shader_desc->entries[i].divisor
                        );

                    vb->attrib_count++;

                    char buf2[128] = {0}; ptr = &buf[0];
                    uint32 size2 = string_format(&ptr, 128,
                        "[%2u]: "
                        "loc: %2u, "
                        "num_elems: %2u, "
                        "type: %s, "
                        "loc_offset: %2u, "
                        "offset: %2u, "
                        "\n",
                        shader_desc->v_layout[i].idx,
                        shader_desc->v_layout[i].loc,
                        shader_desc->v_layout[i].num_elems,
                        shaderinfo_type_to_glsltype_string(shader_desc->v_layout[i].type),
                        shader_desc->v_layout[i].loc_offset,
                        vao_descr[vao_attr_desc_cnt].attr_offset
                    );
                    engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf2, size2);

                    vao_attr_desc_cnt++;
                }
            }

            opengl_debug_attributes_in(shader_desc->vshader_attrib_locations_count);                GFX_CHECK_ERROR();
            attribs_group->rqidx = renderer_renderqueue_add(gfx_params.sid_idx, prog->program, shader_desc->build_idx,
                                                            va->vao, va->idx, vb->vbo, vb->idx);
        } else {
            vbidx = gfx_vertex_buffer_find_match(attribs_group->sid_idx);
            vb = vertex_buffer_get_vbo(vbidx);
        }
        assert(vbidx != INT_NULL);



        /* VERTEX INDEX BUFFER */
        ibidx_t ibidx = INT_NULL;
        struct vertex_index_buffer_object* ib = NULL;
        if(is_new_vao){
            /// TODO: calculate the size of the vertex buffer OR use dynamic array vertex buffer
            ibidx = gfx_vertex_index_buffer_create(200*SIZE_I32);

            ib = vertex_index_buffer_get_ib(ibidx);
            gl_bind_buffer(OPENGL_ELEMENT_ARRAY_BUFFER, ib->ibo);                                   GFX_CHECK_ERROR();

        } else {
            assert(0>0);
            // vbidx = gfx_vertex_buffer_find_match(attribs_desc->sid_idx);
            // vb = vertex_buffer_get_vbo(vbidx);
        }
        assert(ibidx != INT_NULL);

        /* SET COMMANDS */
        void* p = NULL;
        /// SET COMMAND: IB_SET_DATA
        struct ib_cmd_params ind_args = { .ibidx = ib->idx, .vpos_num_elems = gfx_params.vpos_num_elems, .attribs_idx = gfx_params.attribs_group_desc_idx};
        p = &ind_args;
        cmd_add_command((void*)gfx_vertex_index_buffer_set_data, p, sizeof(struct ib_cmd_params));
        // SET COMMAND: IB_UPDATE_DATA
        cmd_add_command((void*) gfx_vertex_index_buffer_update_data, p, sizeof(struct ib_cmd_params));


        /// SET COMMAND: VB_SET_DATA
        struct vb_cmd_params vert_args = { .vaidx = vaoidx, .vbidx = vb->idx, .vpos_num_elems = gfx_params.vpos_num_elems, .shader_attribs_idx = gfx_params.attribs_group_desc_idx};
        p = &vert_args;
        // cmd_add_command((void*)gfx_vertex_buffer_set_data, p, sizeof(struct vb_cmd_params));
        cmd_add_command((void*)gfx_vertex_buffer_set_data2, p, sizeof(struct vb_cmd_params));
        // SET COMMAND: VB_UPDATE_DATA
        cmd_add_command((void*) gfx_vertex_buffer_update_data, p, sizeof(struct vb_cmd_params));

        gl_use_program(0x0);
        gl_bind_vertex_array(0x0);
        gl_bind_buffer(OPENGL_ARRAY_BUFFER, 0x0);


        /// close file for debug
        engine_fdebug_append_to_file_from_arena(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT);
        attribs_group->commands_flag |= CMD_CREATE_VAO;
    }

    /* Set and update shader uniform inputs */
    gl_use_program(prog->program);
    const float* data_src = unifGroupDescr_get_unif_data(uniforms_group_id);
    for(uint32 i = 0; i < num_uniforms; i++) {

        // const float* data = &uniforms_data[unif_desc[i].unif_offset];
        int unif_start = unif_desc[i].unif_offset;
        const float* data = &data_src[unif_start];
        union uniform_data params = {
            .buffer = { .count = 1, .data = data, .transpose = FALSE },
        };
        unif_desc->update_fn.fnptr(&params, unif_desc->loc);                                        GFX_CHECK_ERROR();
    }

    gl_use_program(0x0);

    /* calculate and set command for renderqueue */
    struct command_render_queue_params rq_params = {
        .attribs_idx  = tri->attribs_group_id,
        .index_count  = num_objects * object_get_vertex_index_count(obj->id),
        .vertex_count = num_objects * object_get_vertex_count(obj->id)
    };
    cmd_add_command((void*) render_queue_add_draw_count, &rq_params, sizeof(struct command_render_queue_params));

}

/* OLD-CODE */
/* create and upload uniforms: many structs */
// // set and upload uniforms
// union gl_uniform color_params = {
//     .buffer = {
//         .count    = 1,
//         .data     = &color,
//         .transpose = FALSE
//     },
// };
// union gl_uniform camera_params = {
//     .buffer = {
//         .count    = 1,
//         .data     = &camera->projection_matrix.projection_matrix,
//         .transpose = FALSE
//     },
// };
//
//
// union gl_uniform params[] = {
//     color_params,
//     camera_params
// };

/* old uniform set and upload */
// /* Set and update shader uniform inputs */
// struct shader_descriptor* shader_desc = shader_descriptor_get(gfx_params.desc_idx);
// struct opengl_program* prog = shader_program_get(gfx_params.build_idx);
// gl_use_program(prog->program);
// for(uint32 i = 0; i < shader_desc->entry_count; i++) {
//
//     if(shader_desc->entries[i].input_flag & IS_UNIF) {
//         GLint location = gl_get_uniform_location(prog->program, shader_desc->entries[i].codename);
//         if(shader_desc->entries[i].type == MAT4X4F) {
//             if((attribs_group->commands_flag & CMD_SET_CAMERA_PROJ)==0) {
//                 struct camera camera = gfx_params.camera;
//                 // glUniformMatrix4fv(location, 1, camera.is_column_major, (float*)camera.projection_matrix.projection_matrix);
//                 unif_fnptr_camera.fnptr(&camera_params, location);
//                 attribs_group->commands_flag |= CMD_SET_CAMERA_PROJ;
//                 GFX_CHECK_ERROR();
//             }
//
//         }
//         else if(shader_desc->entries[i].type == VEC4F) {
//             // (GLint location, GLsizei count, const GLfloat * value)
//             // glUniform4fv(location, shader_desc->entries[i].elems, (float*)color);
//             glUniform4fv(location, 1, (float*)color);
//             GFX_CHECK_ERROR();
//
//         }
//     }
// }

/* camera only uniform: set camera only once */
// if((attribs_group->commands_flag & CMD_SET_CAMERA_PROJ)==0) {
//     // cmd_add_command((void*)command_set_camera, &gfx_params, sizeof(struct cmd_gfx_params));
//     struct opengl_program* prog = shader_program_get(gfx_params.build_idx);
//     gl_use_program(prog->program);
//     GLint mvp_location = gl_get_uniform_location(prog->program, "projMat");
//     struct camera camera = gfx_params.camera;
//     glUniformMatrix4fv(mvp_location, 1, camera.is_column_major, (float*)camera.projection_matrix.projection_matrix);
//     GFX_CHECK_ERROR();
//     gl_use_program(0x0);
//     attribs_group->commands_flag |= CMD_SET_CAMERA_PROJ;
// }