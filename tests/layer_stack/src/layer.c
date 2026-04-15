/*
 * Created by ckalo on 3/28/26.
 */

#include "layer.h"

#include <unistd.h>

#include "logger.h"
#include "my_types.h"


#define STACK_SIZE 4


struct layer layer_stack_buffer[STACK_SIZE];
uint32 stack_layer_count = 0;

struct layer* struct_layer_get_byid(const uint32 layer_id)
{
    return &layer_stack_buffer[layer_id];
}


void layer_create_layer(){}

void layer_on_event(){}
void layer_on_update()
{
    print("Updating ...\n");
    sleep(1);
    for(uint32 i=0;i<stack_layer_count;i++) {
        struct layer* layer = struct_layer_get_byid(i);
        print("   %s\n", layer->name);
        sleep(1);
    }
    print("---------------------\n");
    sleep(2);
}
void layer_on_render()
{
    /* gl_draw_arrays();
     */
    print("Rendering ...\n");
    sleep(1);
    for(uint32 i=0;i<stack_layer_count;i++) {
        struct layer* layer = struct_layer_get_byid(i);
        print("   %s\n", layer->name);
        sleep(1);
    }
    print("---------------------\n");
    sleep(2);
}


void layer_stack_push(struct layer layer)
{
    const uint32 idx = stack_layer_count;
    layer_stack_buffer[idx] = layer;
    stack_layer_count++;
}