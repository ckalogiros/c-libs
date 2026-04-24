/*
 * Created by ckalo on 4/8/26.
 */

#include "logger.h"
#include "mem_man_interface.h"
#include "shader_builder.h"
#include "stringc.h"
#include "test.h"

struct test_struct {
    const char* text;
};

struct test_struct test()
{
    return (struct test_struct){ .text = "HELLO!" };
}

void m_main(void)
{
    memman_init();
    shader_builder_init();

    /*
     * the above concludes:
     * 1. there must be some structure to store all flags, unique per shader program
     * 2. there must be a way to set attributes/uniforms/out_variables by name, type, in_vs/in_fs
     *  some basic attributes may be pre-created (color, vpos, scale, rot, time, ...)
     *
     *  ??? case we need to pass a local created variable (in vertex shader main())
     *      how do we implement to "get" it in the fragment shader ???
     *
     *  create_attribute_in(name:"time", type:float, id:0x1, idx: 0)
     *  create_uniform_in(name:"time", type:float, id:0x1, idx: 0)
     *  create_out_variable(name:get_name_by_id(), type:(attrib/unif/)get_type_byid(), id:0x1, idx: 0)
     *  create_operation(name:"sdf_op", code:"code here...")
     *
     *  global store
     *  buffer of per shader program:
     *      struct SHADER IN/OUT:
     *          in_attributes,
     *          in_uniforms,
     *          out_variables,
     *          shader_operations,
     *  buffer of per SHADER IN/OUT
     *      struct: name,    type,   id,   idx
     *              "color"  vec4f   0x1   0
     *              "vpos"   vec3f   0x2   1
     *              ...
     *
     *
     * TODO:
     * implement generic_ attributes
     * implement attribute/uniform assignments in vertex buffer main()
     * implement shader_operations
     *
     *
     */

    // sid_t sid_attr = SID_VPOSI2 | SID_SCALE2 | SID_WPOSI3 | SID_ROTAT2 | SID_PIVOT2 | SID_PROJMAT4X4;
    // sid_t sid_attr = SID_VPOSI2 | SID_SCALE2 | SID_WPOSI3 | SID_ROTAT2 | SID_PIVOT2;
    // sid_t sid_attr = SID_VPOSI2 | SID_SCALE2 | SID_WPOSI3 ;
    // sid_t sid_attr = SID_VPOSI2 | SID_WPOSI3 | SID_COLOR3;
    sid_t sid_attr = SID_VPOSI2 | SID_COLOR4;
    const sid_t sid_attr_out = SID_COLOR3;
    // sid_t sid_unif = SID_VPOSI2 | SID_SCALE2 | SID_WPOSI2 | SID_ROTAT2 | SID_COLOR3 | SID_PROJMAT4X4;
    sid_t sid_unif = SID_PROJMAT4X4;
    const sid_t sid_other = SID_COLOR4;

    // const uint32 attrib_color_idx = create_in_attribute("color4f", VEC4F, TRUE, 0x0);
    // sid_attr |= create_in_attribute("############" , VEC2F);
    // sid_attr |= create_in_attribute("11111" , VEC4F);
    // sid_attr |= create_in_attribute("2222" , MAT2X2I8);
    // // create_in_attribute("scale2f", VEC2F, FALSE, TRANSFORM_LOCAL_SPACE);
    // // create_in_attribute("rotate2f", VEC2F, FALSE, TRANSFORM_LOCAL_SPACE);
    // // create_in_attribute("wpos3f" , VEC3F, FALSE, TRANSFORM_WORLD_SPACE);
    // //
    // // const uint32 out_color_idx  = create_output_variable("out_color" , VEC4F);
    // //
    sid_unif |= create_in_uniform("MVP" , MAT4X4F);
    // create_in_uniform("unif_scale2f" , VEC2F, SHADER_VERTEX|SHADER_FRAGMENT, TRANSFORM_LOCAL_SPACE);
    // create_in_uniform("unif_wpos2f"  , VEC2F, SHADER_VERTEX|SHADER_FRAGMENT, TRANSFORM_WORLD_SPACE);

    // shader_attribs_info_table_print();
    // shader_uniforms_info_table_print();

    shader_builder_build(sid_attr, sid_attr_out, sid_unif, sid_other);


    // opengl attributes sample
    {
        // GLuint vbo;
        // gl_gen_buffers(1, &vbo);
        // gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo);
        // gl_buffer_data(OPENGL_ARRAY_BUFFER, size, vertices, OPENGL_STATIC_DRAW);

        // progidx_t program = renderer_create_shader();
        // GLint mvp_location = gl_get_uniform_location(program, "MVP");
        // GLint vpos_location = gl_get_attribute_location(program, "vPos");
        // GLint vcol_location = gl_get_attribute_location(program, "vCol");
        //
        // GLuint vao = 0;
        // gl_gen_vertex_array(1, &vao);
        // gl_bind_vertex_array(vao);
        // gl_enable_attrib_array(vpos_location);
        // gl_enable_attrib_pointer(vpos_location, 2, sizeof(Vertex), offsetof(Vertex, pos));
        // gl_enable_attrib_array(vcol_location);
        // gl_enable_attrib_pointer(vcol_location, 3, sizeof(Vertex), offsetof(Vertex, col));
    }


    struct test_struct t = test();
    print("%s", t.text);


    shader_builder_destroy();
    memman_terminate();
}

int main(void)
{
    // test_shader_builder();
    m_main();

    return 0;
}




