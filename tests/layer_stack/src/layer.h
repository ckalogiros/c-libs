/*
 * Created by ckalo on 3/28/26.
 */
#ifndef LAYER_STACK_LAYER_H
#define LAYER_STACK_LAYER_H


struct layer {
    const char* name;
};

void layer_create_layer();
void layer_on_event();
void layer_on_update();
void layer_on_render();
void layer_stack_push();

#endif //LAYER_STACK_LAYER_H