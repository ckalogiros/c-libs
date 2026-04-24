
#include "gfx_buffers.h"
#include "mem_man_interface.h"
#include "opengl.h"
#include "opengl_debug.h"
#include "run.h"
#include "scene.h"
#include "shader_builder.h"
#include "sparse_arr_internals.h"


static const char* fragment_shader_text =
"#version 330\n"
// "in vec4 out_color4;\n"
"in vec3 out_color3;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
// "    fragment = out_color4;\n"
"    fragment = vec4(out_color3, 1.0);\n"
"}\n";



progidx_t gfx_program_exists(const struct gfx_sid sid){return INT_NULL;}
uint32 gfx_program_create(const struct gfx_sid sid)
{
    uint32 progidx = opengl_program_create(sid, fragment_shader_text);
    return progidx;
}


void gfx_generate_context(const struct gfx_sid sid)
{
    progidx_t progidx = INT_NULL;
    vaidx_t vaoidx = INT_NULL;
    vaidx_t vao    = INT_NULL;
    vbidx_t vbidx  = INT_NULL;
    ibidx_t ibidx  = INT_NULL;
    // uint32 vb_group = NULL;
    // uint32 vbidxs = NULL;

    /**********************************************************************************************/
    /* program */
    // Check if program already exists
    progidx = gfx_program_exists(sid);
    if (progidx == INT_NULL) { // If shader program does not exist, create new program.


        progidx = gfx_program_create(sid);
        // Scene_set_projection_matrix(sceneidx);
        // Scene_set_screen_res_uniform();


        gl_gen_vertex_array(1, &vao);
        gl_bind_vertex_array(vao);
    }



    /**********************************************************************************************/
    /* vertex array object */
    /// set up vertex attrib pointers
    // gl_enable_attrib_array();
    // gl_enable_attrib_pointer();


    /**********************************************************************************************/
    /* vertex buffer object*/
    /* Check for any vbidx match, from already created vbs, in the mesh tree structure */
    /* If no vbidx found from the mesh tree, search the vb pool if any apropriate vb exists.
     * If none found, create a new one */
    vbidx = gfx_vertex_buffer_find_match(sid);
    if(vbidx == INT_NULL) {
        vbidx = gfx_vertex_buffer_create(sid, 1024);
    }




    /**********************************************************************************************/
    /* index array object */

    progidx_t program = opengl_program_get_byidx(progidx);
    vbidx_t vbo = vertex_buffer_get_vbo(vbidx);
    renderer_renderqueue_add(program, progidx, vao, 0, vbo, vbidx);

    OpenGlDebugContext();


}

int
main(void)
{


    memman_init();
    sparse_arr_init_table(32);
    shader_builder_init();


    struct scene* scene = scene_create("scene 1");
    struct layer layer1 = {
        .name="layer1",
    };
    layer_create_layer(&layer1);
    scene_push_layer(scene, layer1);

    const struct gfx_sid sid = {
        .attr = SID_VPOSI2 | SID_SCALE2 | SID_COLOR3,
        .unif = SID_PROJMAT4X4,
        .out = SID_COLOR3,
        .pass = SID_COLOR3,
        .oper = SID_NULL,
    };

    gfx_generate_context(sid);


    float vertices[] = {
         -1.f, -1.f, 1.0f, 1.0f, 1.f, 0.f, 0.f,
          1.f, -1.f, 1.0f, 1.0f, 0.f, 1.f, 0.f ,
          .0f,  1.f, 1.0f, 1.0f, 0.f, 0.f, 1.f
    };
    gfx_vertex_buffer_set_data(0, (void*)&vertices);
    // struct entity e = {
    //     .name = "entity1",
    //     .vertices = vertices,
    // };
    // scene_add_entity(scene, layer1, e);

    run();

    sparse_arr_destroy_table();
    shader_builder_destroy();
    memman_terminate();

   return 0;
}
