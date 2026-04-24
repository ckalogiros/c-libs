// #include "component.h"
// #include "entity.h"
#include "layer.h"
#include "mem_man_interface.h"
// #include "precision_timer.h"
// #include "linmath.h"
#include "entity_test.h"
#include "scene.h"
#include "shader_builder.h"
#include "sparse_arr_internals.h"
#include "stack_allocator.h"


// struct timer* t = timer_create_perf_timer();
// perf_timer_start(t);
// perf_timer_stop(t);
// perf_timer_print(timer_print_config((timer_print_params){
//     .t = t, .repeat = 1, .buf_capacity = 1, .flags = PERF_T_USE_REPEAT|PERF_T_ALL
// }));
// perf_timer_reset(t);

struct transform {
    vec3f color;
    vec3f wpos;
    vec2f scale;
};



// void create_triangle_mesh()
// {
//     struct transform transform = {
//         .color = {.r=1., .g=1., .b=1.},
//         // .wpos = {.x=0., .y=0., .z=0.},
//         .scale = {.x=1., .y=1.},
//     };
//
//     const uint32 num_attrbs = 3 + 2;
//     const uint32 num_verts = 3;
//     const uint32 num_elems = num_attrbs * num_verts;
//
//     float vertices[num_elems] = {};
//
//
//     for(uint32 i = 0; i < num_verts; i++) {
//         for(uint32 i = 0; i < num_verts; i++) {
//         vertices[i*3] = transform.color.x;
//     }
//
// }

int
main(void)
{
    /*
     * who creates the window?
     *
     * scene = scene_create();
     * scene_set_active_context(scene)
     *
     * create meshes();
     *
     * layer1 = layer_create();
     * layer_push(meshes);
     *
     * scene_run();
     *
     */

    memman_init();
    sparse_arr_init_table(32);
    shader_builder_init();
    // const memid_t instance_id = stack_allocator_create_new_instance(4);
    // void* address = stack_allocator_alloc(instance_id, size + 1, sizeof(some_text[0][0]));
    // timer_init_table();
    // app_init();

    /* create entities */
    // init_entity_table();
    // // init_pos_table();
    // // init_scale_table();
    // // init_col_table();
    // // init_all_attribs_table();
    // const uint32 entity = entity_create();
    // print_entities();
    // // add_pos_component(entity, (vec3f){.x = 11, .y = 22, .z = 33});
    // // add_scale_component(entity, (vec2f){.x = 1111, .y = 2222,});
    // // add_col_component(entity, (vec4f){.b = 1.2, .g = 2.3, .b = 3.4, .a = 4.5,});

    struct scene* scene = scene_create("scene 1");
    struct layer layer1 = {
        .name="layer1",
    };
    layer_create_layer(&layer1);
    scene_push_layer(scene, layer1);


    Vertex vertices[] = {
        { { -1.f, -1.f }, { 1.0f, 1.0f }, { 1.f, 0.f, 0.f } },
        { {  1.f, -1.f }, { 1.0f, 1.0f }, { 0.f, 1.f, 0.f } },
        { {  .0f,  1.f }, { 1.0f, 1.0f }, { 0.f, 0.f, 1.f } }
    };
    struct entity e = {
        .name = "entity1",
        .vertices = vertices,
    };
    scene_add_entity(scene, layer1, e);

    scene_run(scene);

    shader_builder_destroy();
    memman_terminate();

    return 0;
}
