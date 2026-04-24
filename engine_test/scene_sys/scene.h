/*
 * Created by ckalo on 4/2/26.
 */
#ifndef ENGINE_TEST_SCENE_H
#define ENGINE_TEST_SCENE_H

#include "layer.h"

struct scene {
    struct layer_stack layer_stack;
    struct entity* entities;
    uint32 entities_count;
    const char* name;
};


void scene_init();
struct scene* scene_create(const char* name);
void scene_destroy(); /// todo: implement
void scene_update(struct scene* scene); /// todo: implement
void scene_render(struct scene* scene); /// todo: implement
void scene_run(struct scene* scene);

void scene_push_layer(struct scene* scene, struct layer layer);
void scene_add_entity(struct scene* scene, struct layer layer, struct entity entity);

#endif //ENGINE_TEST_SCENE_H