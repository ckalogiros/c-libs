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
#include "opengl.h"
#include "opengl_debug.h"
#include "run.h"
#include "string_format.h"


struct cmd_gfx_params {
    struct camera camera;
    const char* unif_name;
    const float* unif_data;
    sididx_t sid_idx;
    attr_t   shader_attribs_idx;
    attrid_t vao_attribs_idx;
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



void render_tri(struct camera* camera)
{
    /// create attrib buffer:
    struct attribs_per_attrib_descriptor desc[] = {
        {.attr_offset = 0 , .attr_num_elems = 2, .divisor = 0, .attrib_flag = SID_VPOSI2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_VPOS},
        {.attr_offset = 2 , .attr_num_elems = 4, .divisor = 0, .attrib_flag = SID_COLOR4, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_OUT},
        {.attr_offset = 6 , .attr_num_elems = 3, .divisor = 0, .attrib_flag = SID_WPOSI3, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_WPOS},
        {.attr_offset = 9,  .attr_num_elems = 2, .divisor = 0, .attrib_flag = SID_SCALE2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_SCALE},
        {.attr_offset = 11, .attr_num_elems = 2, .divisor = 0, .attrib_flag = SID_ROTAT2, .shader_input_flags = IN_VSHADER | IS_ATTR | IS_TRANSFORM_ROT}
    };
    float* triVert = mesh_get_mesh_vertices_from_type(MESH_TRIANGLE);
    float attribs[] = {
        /// vpos2              color4             wpos3          scale2     rot2
        triVert[0],triVert[1], 0.4,0.4,0.4,1.0,   0.3,0.3,-10,   1.0,1.0,   cos(0.0),sin(0.0), // v0
        triVert[2],triVert[3], 0.4,0.4,0.4,1.0,   0.3,0.3,-10,   1.0,1.0,   cos(0.0),sin(0.0), // v1
        triVert[4],triVert[5], 0.4,0.4,0.4,1.0,   0.3,0.3,-10,   1.0,1.0,   cos(0.0),sin(0.0), // v2
    };

    uint32 num_objects = 1;
    uint32 num_vertices = 3;
    uint32 stride = 13;
    uint32 num_attributes = 5;

    struct gfx_sid sid = {0,0,0,0,0};
    // sid.attr |= SID_VPOSI2 | SID_COLOR4 | SID_WPOSI3 | SID_SCALE2 | SID_ROTAT2;
    // sid.unif |= SID_PROJMAT4X4;
    sid.out  |= SID_COLOR4;
    sid.pass |= SID_COLOR4;
    sid.oper |= SID_NULL;

    enum MESH_TYPE mesh_type = MESH_TRIANGLE;

    sididx_t sid_idx = gfx_sid_find_match(sid);
    attrid_t attribs_group_id = INT_NULL;

    if(sid_idx != INT_NULL) { /// 1. attribute descriptor group exists
        attribs_group_id = attribsGroupDescr_group_find_match(sid_idx);
    } else {                  /// 2. create new attribute descriptor group
        sid_idx = gfx_create_sid(sid);
        attribs_group_id = attribsGroupDescr_create_new_attribs_group(sid_idx);
    }

    struct object* obj  = object_create(sid_idx, mesh_type);
    obj->num_faces      = 1;
    obj->verts_per_face = mesh_get_mesh_vertex_count_from_type(mesh_type);
    obj->index_count    = mesh_get_mesh_indices_count_from_type(mesh_type);
    obj->attribs_group_id = attribs_group_id;

    object_set_vertex_count(obj->id, obj->num_faces * obj->verts_per_face);
    attribsGroupDescr_create_new_obj(obj->attribs_group_id, obj->id, obj->num_faces * obj->verts_per_face);

    /// 1. find or create shader build
    build_t build_idx = shader_build_exists(sid_idx);
    if(build_idx == INT_NULL) build_idx = shader_builder_create_new_build(sid_idx);
    attribsGroupDescr_set_build_idx(attribs_group_id, build_idx);


    enum SHADER_INPUT_FLAGS attrib_flags = 0x0;
    enum SID_ID shader_input_id = 0;

    /**********************************************************************************************
     * ATTRIBUTES */
    struct attributes_group* attribs_group = attribsGroupDescr_get_attribs_group(attribs_group_id);

    uint32 attrib_size = num_vertices*stride*SIZE_F32;
    float* buffer_ptr = arena_allocator_alloc(attribs_group->attribs_data_id, attrib_size, SIZE_F32);
    memcpy(buffer_ptr, attribs, attrib_size);

    for(uint32 i = 0; i < num_attributes; i++) {

        if((attribs_group->added_components_flag & desc[i].attrib_flag) == 0x0) {
            const attr_t attrib_idx = attribs_group->attribs_count;
            assert(attrib_idx < COMP_MAX_ATTRIBUTES);
            attribs_group->added_components_flag                       |= desc[i].attrib_flag;
            attribs_group->attribs_descr[attrib_idx].attrib_flag        = desc[i].attrib_flag;
            attribs_group->attribs_descr[attrib_idx].attr_offset        = desc[i].attr_offset;
            attribs_group->attribs_descr[attrib_idx].attr_num_elems     = desc[i].attr_num_elems;
            attribs_group->attribs_descr[attrib_idx].divisor            = desc[i].divisor;
            attribs_group->attribs_descr[attrib_idx].shader_input_flags = desc[i].shader_input_flags;
            attribs_group->attribs_count++;
        }

        const attr_t shader_input_id = shaderinfo_flag_to_id(desc[i].attrib_flag);
        shader_create_input(build_idx, shader_input_id, desc[i].shader_input_flags, desc[i].divisor);
        sid.attr |= desc[i].attrib_flag;
    }



    /***********************************************************************************************
     * UNIFORMS */
    /// add projection matrix uniform
    attrib_flags = IN_VSHADER | IS_UNIF | IS_TRANSFORM_PROJ;
    shader_input_id = SID_PROJMAT4X4_ID;
    shader_create_input(build_idx, shader_input_id, attrib_flags, 0);
    sid.unif |= SID_PROJMAT4X4;

    attribsGroupDescr_set_stride(attribs_group_id, stride);

    print("%s- Create object: triangle -%s\n"
          "   id:%u\n   attribs_id:%u\n   num_faces:%2u\n   verts_per_face:%3u \n   attrib_count:%u\n",
          BLUE, CLEAR, obj->id, obj->attribs_group_id, obj->num_faces, obj->verts_per_face, obj->attrib_count);

    engine_fdebug_append_string_format(ENGINE_FDEBUG_FILE_GENERIC, "\n");
    const attr_t vao_attribs_idx = 0;

    struct object* tri = obj;


    /***********************************************************************************************
     */
    struct cmd_gfx_params gfx_params = {
        .sid_idx = attribs_group->sid_idx,
        .shader_attribs_idx = tri->attribs_group_id,
        .shader_attribs_idx = vao_attribs_idx,
        .build_idx = attribs_group->build_idx,
        .desc_idx = INT_NULL,
        .vpos_num_elems = mesh_get_num_elems_per_vertex(mesh_type),
        .camera = *camera,
        .unif_name = NULL,
        .unif_data = NULL
    };

    if((attribs_group->commands_flag & CMD_CREATE_SHADER_PROGRAM)==0) {

        build_t build_idx = gfx_program_exists(gfx_params.sid_idx);
        if(build_idx == INT_NULL) { // If shader program does not exist, create new program.
            opengl_program_create(gfx_params.sid_idx, gfx_params.build_idx, test_fragment_shader_text);
            GL_CHECK();
        }

        attribs_group->commands_flag |= CMD_CREATE_SHADER_PROGRAM;
        gfx_params.desc_idx =  attribsGroupDescr_get_attribs_group(tri->attribs_group_id)->build_idx;
    }


    if((attribs_group->commands_flag & CMD_CREATE_VAO)==0) {
        // cmd_add_command((void*)command_create_vao, &gfx_params, sizeof(struct cmd_gfx_params));
        struct attributes_group* attribs_group = attribsGroupDescr_get_attribs_group(gfx_params.shader_attribs_idx);
        struct shader_descriptor* desc = shader_descriptor_get(gfx_params.desc_idx);
        struct attribs_per_attrib_descriptor* vao_descr = attribsGroupDescr_get_attribs_descr(gfx_params.vao_attribs_idx);
        const uint32 stride = desc->vshader_attribs_stride;
        struct opengl_program* prog = shader_program_get(gfx_params.desc_idx);
        gl_use_program(prog->program);

        /* VERTEX ARRAY */
        const bool attrib_devisor = 0; /// TODO: attrib devisor should exist in object OR object component OR shader descriptor???
        bool is_new_vao = FALSE;
        vaidx_t vaoidx = gfx_vertex_array_find_match(desc->sid_idx, attrib_devisor);
        if(vaoidx == INT_NULL) {
            vaoidx = gfx_vertex_array_create(desc->build_idx);
            is_new_vao = TRUE;
        }
        desc->vaoidx = vaoidx;


        struct vertex_array_object* va = gfx_vertex_array_get_byidx(vaoidx);
        gl_bind_vertex_array(va->vao);                                                      GL_CHECK();


        /* VERTEX BUFFER */
        vbidx_t vbidx = INT_NULL;
        struct vertex_buffer_object* vb = NULL;
        if(is_new_vao){

            struct vertex_array_object* vao = gfx_vertex_array_get_byidx(vaoidx);

            char buf[64] = {0}; char* ptr = &buf[0];
            uint32 size = string_format(&ptr, 64, "--- VAO:%2u ---\n", vao->idx);
            engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf, size);

            uint32 cnt = 0, vao_attr_desc_cnt = 0;
            attr_t divisor = INT_NULL;
            attr_t vb_stride = 0;
            for(uint32 i = 0; i < desc->entry_count; i++){

                if(desc->entries[i].input_flag & IS_ATTR) {

                    if(divisor != desc->entries[i].divisor) {
                        vbidx = gfx_vertex_buffer_create(num_objects*num_vertices*stride*SIZE_F32, stride);
                        vao->vbos[cnt++] = vbidx;                                                       /// push new vertex buffer's index to VAO


                        vb = vertex_buffer_get_vbo(vbidx);
                        gl_bind_buffer(OPENGL_ARRAY_BUFFER, vb->vbo);                       GL_CHECK();

                        divisor = desc->entries[i].divisor;

                        char buf2[64] = {0}; ptr = &buf[0];
                        uint32 size2 = string_format(&ptr, 64, " VBO:%2u\n", vbidx);
                        engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf2, size2);

                        va->vbcount++;
                        vb_stride = 0;
                    }

                    vb->v_layout[vb->attrib_count].loc        = desc->v_layout[i].loc;
                    vb->v_layout[vb->attrib_count].num_elems  = desc->v_layout[i].num_elems;
                    vb->v_layout[vb->attrib_count].type       = desc->v_layout[i].type;
                    vb->v_layout[vb->attrib_count].attr_idx   = desc->v_layout[i].attr_idx;             /// UNUSED for vertex buffers???
                    vb->v_layout[vb->attrib_count].loc_offset = desc->v_layout[i].loc_offset;           /// UNUSED for vertex buffers???
                    vb->v_layout[vb->attrib_count].offset     = vao_descr[vao_attr_desc_cnt].attr_offset;
                    vb->v_layout[vb->attrib_count].idx        = desc->v_layout[i].idx;                  /// UNUSED for vertex buffers???
                    vb->attrib_count++;
                    vb_stride += desc->v_layout[i].num_elems;
                    vb->stride = vb_stride;


                    shader_attribute_enable(desc->v_layout[i].loc, desc->v_layout[i].num_elems,
                        stride, vao_descr[vao_attr_desc_cnt].attr_offset, SIZE_F32, divisor);

                    char buf2[128] = {0}; ptr = &buf[0];
                    uint32 size2 = string_format(&ptr, 128,
                        "[%2u]: "
                        "loc: %2u, "
                        "num_elems: %2u, "
                        "type: %s, "
                        "loc_offset: %2u, "
                        "offset: %2u, "
                        "\n",
                        desc->v_layout[i].idx,
                        desc->v_layout[i].loc,
                        desc->v_layout[i].num_elems,
                        shaderinfo_type_to_glsltype_string(desc->v_layout[i].type),
                        desc->v_layout[i].loc_offset,
                        vao_descr[vao_attr_desc_cnt].attr_offset
                    );
                    engine_fdebug_append_string(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT, buf2, size2);

                    vao_attr_desc_cnt++;
                }



            }

            shader_attributes_enable(desc);
            opengl_debug_attributes_in(desc->vshader_attrib_locations_count);         GL_CHECK();
            attribs_group->rqidx = renderer_renderqueue_add(gfx_params.sid_idx, prog->program, desc->build_idx,
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
            gl_bind_buffer(OPENGL_ELEMENT_ARRAY_BUFFER, ib->ibo);
            GL_CHECK();

        } else {
            assert(0>0);
            // vbidx = gfx_vertex_buffer_find_match(attribs_desc->sid_idx);
            // vb = vertex_buffer_get_vbo(vbidx);
        }
        assert(ibidx != INT_NULL);

        /* SET COMMANDS */
        void* p = NULL;
        /// SET COMMAND: IB_SET_DATA
        struct ib_cmd_params ind_args = { .ibidx = ib->idx, .vpos_num_elems = gfx_params.vpos_num_elems, .attribs_idx = gfx_params.shader_attribs_idx};
        p = &ind_args;
        cmd_add_command((void*)gfx_vertex_index_buffer_set_data, p, sizeof(struct ib_cmd_params));
        // SET COMMAND: IB_UPDATE_DATA
        cmd_add_command((void*) gfx_vertex_index_buffer_update_data, p, sizeof(struct ib_cmd_params));


        /// SET COMMAND: VB_SET_DATA
        struct vb_cmd_params vert_args = { .vaidx = vaoidx, .vbidx = vb->idx, .vpos_num_elems = gfx_params.vpos_num_elems, .shader_attribs_idx = gfx_params.shader_attribs_idx};
        p = &vert_args;
        cmd_add_command((void*)gfx_vertex_buffer_set_data, p, sizeof(struct vb_cmd_params));
        // SET COMMAND: VB_UPDATE_DATA
        cmd_add_command((void*) gfx_vertex_buffer_update_data, p, sizeof(struct vb_cmd_params));

        gl_use_program(0x0);
        gl_bind_vertex_array(0x0);
        gl_bind_buffer(OPENGL_ARRAY_BUFFER, 0x0);


        /// close file for debug
        engine_fdebug_append_to_file_from_arena(ENGINE_FDEBUG_FILE_VERTEX_LAYOUT);
        attribs_group->commands_flag |= CMD_CREATE_VAO;
    }


    if((attribs_group->commands_flag & CMD_SET_CAMERA_PROJ)==0) {
        // cmd_add_command((void*)command_set_camera, &gfx_params, sizeof(struct cmd_gfx_params));
        struct opengl_program* prog = shader_program_get(gfx_params.build_idx);
        gl_use_program(prog->program);
        GLint mvp_location = gl_get_uniform_location(prog->program, "projMat");
        struct camera camera = gfx_params.camera;
        glUniformMatrix4fv(mvp_location, 1, camera.is_column_major, (float*)camera.projection_matrix.projection_matrix);
        GL_CHECK();
        gl_use_program(0x0);
        attribs_group->commands_flag |= CMD_SET_CAMERA_PROJ;
    }

    struct command_render_queue_params rq_params = {
        .attribs_idx  = tri->attribs_group_id,
        .index_count  = num_objects * object_get_vertex_index_count(obj->id),
        .vertex_count = num_objects * object_get_vertex_count(obj->id)
    };
    cmd_add_command((void*) render_queue_add_draw_count, &rq_params, sizeof(struct command_render_queue_params));

}
