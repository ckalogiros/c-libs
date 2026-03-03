/*
 * Created by ckalo on 2/1/26.
 */

#include "tests.h"

#include "buffer_internals.h"
#include "component.h"
#include "ecs.h"
#include "entity.h"
#include "logger.h"
#include "mem_man_interface.h"
#include "precision_timer_logger.h"


// static const uint32 REPEAT = 1000000;
static const uint32 REPEAT = 100000;

void
test_create_entities(const uint32 count)
{
    memman_init();
    buffer_init_table();
    init_entity_table();


    // create many entities
    for(uint32 i = 0; i < count; i++) {
        uint32 eidx = entity_create();
    }

    print_entities();


    memman_terminate();
}

void // adding to each component in its own for loop
test_components_seperate_add(const uint32 count)
{
    memman_init();
    buffer_init_table();
    timer_init_table();

    init_entity_table();
    init_pos_table();
    init_scale_table();
    init_col_table();


    // create many entities
    for(uint32 i = 0; i < count; i++) {
        uint32 eidx = entity_create();
    }

    const struct timer* t = timer_create_perf_timer();

    perf_timer_start(t);
    // add component pos
    const uint32 repeat = REPEAT;
    for(uint32 r = 0; r < repeat; r++) {
        for(uint32 i = 0; i < count; i++) {
            const uint32 eid = entity_get_id(i);
            add_pos_component(eid, (vec3f){
                                  .x = i, .y = i + count, .z = 0,
                              });
        }
        // // add component scale
        // for(uint32 i = 0; i < count; i++) {
        //     const uint32 eid = entity_get_id(i);
        //     add_scale_component(eid, (vec2f){
        //         .x = i, .y=i+count,
        //     });
        // }
        // // add component col
        // for(uint32 i = 0; i < count; i++) {
        //     const uint32 eid = entity_get_id(i);
        //     add_col_component(eid, (vec4f){
        //         .r = i, .g=i+count, .b = 0, .a=1.0,
        //     });
        // }
    }
    perf_timer_stop(t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat, PERF_T_ALL
    }));


    // print_components();

    memman_terminate();
}


void // adding to components in 1 for loop
test_components_together_add(const uint32 count)
{
    memman_init();
    buffer_init_table();
    timer_init_table();

    init_entity_table();
    init_pos_table();
    init_scale_table();
    init_col_table();


    // create many entities
    for(uint32 i = 0; i < count; i++) {
        uint32 eidx = entity_create();
    }

    const struct timer* t = timer_create_perf_timer();

    perf_timer_start(t);
    // add component pos
    const uint32 repeat = REPEAT;
    for(uint32 r = 0; r < repeat; r++) {
        for(uint32 i = 0; i < count; i++) {
            const uint32 eid = entity_get_id(i);
            add_pos_component(eid, (vec3f){
                                  .x = i, .y = i + count, .z = 0,
                              });
            // // add component scale
            // add_scale_component(eid, (vec2f){
            //     .x = i, .y=i+count,
            // });
            // // add component col
            // add_col_component(eid, (vec4f){
            //     .r = i, .g=i+count, .b = 0, .a=1.0,
            // });
        }
    }
    perf_timer_stop(t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat, PERF_T_ALL
    }));


    // print_components();

    memman_terminate();
}


void
test_components_all_attribs(const uint32 count)
{
    memman_init();
    buffer_init_table();
    timer_init_table();

    init_entity_table();
    init_all_attribs_table();


    // create many entities
    for(uint32 i = 0; i < count; i++) {
        uint32 eidx = entity_create();
    }

    const struct timer* t = timer_create_perf_timer();
    const uint32 repeat = REPEAT;

    perf_timer_start(t);
    // add component all_attribs
    for(uint32 r = 0; r < repeat; r++) {
        for(uint32 i = 0; i < count; i++) {
            const uint32 eid = entity_get_id(i);
            add_all_attribs_component(eid, (struct all_attribs){
                                          .col = {.r = i, .g = i + count, .b = 0, .a = 1.0,},
                                          // .pos = {.x=i, .y=i+count, .z=0},
                                          // .scale = {.x=i, .y=i+count},
                                      });
        }
    }

    perf_timer_stop(t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat, PERF_T_ALL
    }));


    // print_components();

    memman_terminate();
}


void
test_perf_ecs_insert_to_comp_helper(uint32 count, uint32 repeat, struct timer* t)
{
    /*************************************************************************/
    // add components in seperate loops

    perf_timer_start(t);
    // add component pos
    for(uint32 r = 0; r < repeat; r++) {
        for(uint32 i = 0; i < count; i++) {
            const uint32 eid = entity_get_id(i);
            add_pos_component(eid, (vec3f){
                .x = i, .y = i + count, .z = 0,
            });
        }
        // add component scale
        for(uint32 i = 0; i < count; i++) {
            const uint32 eid = entity_get_id(i);
            add_scale_component(eid, (vec2f){
                .x = i, .y = i + count,
            });
        }
        // add component col
        for(uint32 i = 0; i < count; i++) {
            const uint32 eid = entity_get_id(i);
            add_col_component(eid, (vec4f){
                .r = i, .g = i + count, .b = 0, .a = 1.0,
            });
        }
    }
    perf_timer_stop(t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat, PERF_T_ALL
    }));
    perf_timer_reset(t);


    /*************************************************************************/
    // all components in 1 loop

    perf_timer_start(t);
    for(uint32 r = 0; r < repeat; r++) {
        for(uint32 i = 0; i < count; i++) {
            const uint32 eid = entity_get_id(i);
            add_pos_component(eid, (vec3f){
                                  .x = i, .y = i + count, .z = 0,
                              });
            // add component scale
            add_scale_component(eid, (vec2f){
                                    .x = i, .y = i + count,
                                });
            // add component col
            add_col_component(eid, (vec4f){
                                  .r = i, .g = i + count, .b = 0, .a = 1.0,
                              });
        }
    }
    perf_timer_stop(t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat, PERF_T_ALL
    }));
    perf_timer_reset(t);

    /*************************************************************************/
    // 1 component for all attributes in 1 loop

    perf_timer_start(t);
    for(uint32 r = 0; r < repeat; r++) {
        for(uint32 i = 0; i < count; i++) {
            const uint32 eid = entity_get_id(i);
            add_all_attribs_component(eid, (struct all_attribs){
                                          .col = {.r = i, .g = i + count, .b = 0, .a = 1.0,},
                                          .pos = {.x = i, .y = i + count, .z = 0},
                                          .scale = {.x = i, .y = i + count},
                                      });
        }
    }

    perf_timer_stop(t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat, PERF_T_ALL
    }));
    perf_timer_reset(t);
}


void
test_perf_ecs_insert_to_comp()
{
    memman_init();
    buffer_init_table();
    timer_init_table();

    init_entity_table();
    init_col_table();
    init_pos_table();
    init_scale_table();
    init_all_attribs_table();

    struct timer* t = timer_create_perf_timer();

    const uint32 repeat = REPEAT;


    // create entities
    for(uint32 i = 0; i < 1024; i++) {
        uint32 eidx = entity_create();
    }


    uint32 count = 64;
    print("Entities count:%u\n", count, repeat);
    test_perf_ecs_insert_to_comp_helper(count, repeat, t);

    count = 128;
    print("Entities count:%u\n", count, repeat);
    test_perf_ecs_insert_to_comp_helper(count, repeat, t);

    count = 256;
    print("Entities count:%u\n", count, repeat);
    test_perf_ecs_insert_to_comp_helper(count, repeat, t);

    count = 512;
    print("Entities count:%u\n", count, repeat);
    test_perf_ecs_insert_to_comp_helper(count, repeat, t);

    count = 1024;
    print("Entities count:%u   repeat: %u\n", count, repeat);
    test_perf_ecs_insert_to_comp_helper(count, repeat, t);

    memman_terminate();
}

void
test_ecs_groups()
{
    memman_init();
    buffer_init_table();
    init_entity_table();

    // create many entities
    for(uint32 i = 0; i < 10; i++) {
        uint32 eidx = entity_create();
    }


    // create group


    memman_terminate();
}
