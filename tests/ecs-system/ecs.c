/*
 * Created by ckalo on 1/27/26.
 */

#include "ecs.h"

#include "component.h"
#include "entity.h"
#include "logger.h"








void
game_loop()
{
    while(TRUE) {
        // print fps
    }
}

void
test_ecs()
{
    // init
    buffer_init();
    init_entity_table();
    init_pos_table();
    init_scale_table();
    init_col_table();

    // // create
    // for (uint32 i = 0; i < CAPACITY; i++) {
    //   entity_create();
    // }
    // print_entities();

    const uint32 entity = entity_create();

    // add_pos_component(entity, (vec3f){ .color=GRAY, .pos={10,50}, .size={200,8} });
    add_pos_component(entity, (vec3f){.x = 11, .y = 22, .z = 33});
    print_entities();

    add_scale_component(entity, (vec2f){.x = 1111, .y = 2222,});
    print_entities();

    add_col_component(entity, (vec4f){.b = 1.2, .g = 2.3, .b = 3.4, .a = 4.5,});
    print_entities();
}
