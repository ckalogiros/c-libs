/*
 * Created by ckalo on 3/28/26.
 */
#ifndef LAYER_STACK_LAYER_H
#define LAYER_STACK_LAYER_H

#define TEMP_LAYSER_STACK_SIZE 4
#include "my_types.h"
#include "renderer.h"

struct layer {
    const char* name;
    // Vertex vertices[3];
    uint32 program;
    uint32 vao;
    uint32 vbo;
    uint32 mvp_loc;
};
struct layer_stack {
    struct layer layer[TEMP_LAYSER_STACK_SIZE];
    uint32 count;
};

void layer_create_layer_stack();
void layer_create_layer(struct layer* layer);
void layer_on_event();
void layer_on_update(struct layer* layer);
void layer_on_render(struct layer* layer);
void layer_stack_push();

#endif //LAYER_STACK_LAYER_H