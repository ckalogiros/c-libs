/*
 * Created by ckalo on 4/8/26.
 */

#include "mem_man_interface.h"
#include "shader_builder.h"
#include "stringc.h"


int main(void)
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
     *  buffer of id/idx$$$$
     *
     *
     *
     */

    const uint32 attrib_color_idx = create_in_attribute("color4f", VEC4F, TRUE, 0x0);
    create_in_attribute("vpos2f" , VEC2F, FALSE, TRANSFORM_WORLD_SPACE);
    create_in_attribute("scale2f", VEC2F, FALSE, TRANSFORM_LOCAL_SPACE);
    create_in_attribute("rotate2f", VEC2F, FALSE, TRANSFORM_LOCAL_SPACE);
    create_in_attribute("wpos3f" , VEC3F, FALSE, TRANSFORM_WORLD_SPACE);

    const uint32 out_color_idx  = create_output_variable("out_color" , VEC4F);

    const uint32 unif_mvp_idx  = create_in_uniform("MVP" , MAT4X4F, SHADER_VERTEX|SHADER_FRAGMENT, 0x0);
    create_in_uniform("unif_scale2f" , VEC2F, SHADER_VERTEX|SHADER_FRAGMENT, TRANSFORM_LOCAL_SPACE);
    create_in_uniform("unif_wpos2f"  , VEC2F, SHADER_VERTEX|SHADER_FRAGMENT, TRANSFORM_WORLD_SPACE);

    shader_attribs_info_table_print();
    shader_uniforms_info_table_print();

    shader_builder_build();

    shader_builder_destroy();
    memman_terminate();


    return 0;
}




