/*
 * Created by ckalo on 4/2/26.
 */

#include "scene.h"
#include "my_types.h"
#include "window.h"
#include "opengl_debug.h"
#include "entity_test.h"


struct scene scene_table[1];
uint32 scene_table_count = 0;


void
scene_init()
{
    /* allocate scene table memory */
    window_create();
}

struct scene*
scene_create(const char* name)
{
    const uint32 idx = scene_table_count;
    struct scene* scene = &scene_table[idx];

    scene->name = name;
    scene->layer_stack.count = 0;

    scene_table_count++;

    scene->entities_count = 8;
    struct entity* entities = entity_create_buffer(scene->entities_count);
    scene->entities = entities;

    return scene;
}

void
scene_run(struct scene* scene)
{
    vec4f color = {.r = .1, .g = .1, .b = .1, .a = 1.0};

    while(1) {
        window_poll_events();

        if(window_should_close()) {
            window_terminate();
            break;
        }

        gl_clear();
        gl_clear_color(color);

        for(uint32 i = 0; i < scene->layer_stack.count; i++) {
            struct layer* layer = &scene->layer_stack.layer[i];
            layer_on_update(layer);
            layer_on_render(layer);
        }


        window_swap_buffers();

        // OpenGlDebugContext();
    }
}

void
scene_push_layer(struct scene* scene, struct layer layer)
{
    const uint32 idx = scene->layer_stack.count;
    scene->layer_stack.layer[idx] = layer;
    scene->layer_stack.count++;
    // layer_stack_push(layer1);
}

// TODO: Should layer be passed by index???
void
scene_add_entity(struct scene* scene, struct layer layer, struct entity entity)
{
    const uint32 idx = scene->entities_count;
    scene->entities[idx] = entity;
    scene->entities_count++;
}


void
scene_destroy() {}

void
scene_update(struct scene* scene) {}

void
scene_render(struct scene* scene) {}
