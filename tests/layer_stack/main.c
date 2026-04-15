#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "app.h"
#include "layer.h"
#include "logger.h"
#include "precision_timer.h"


#include "precision_timer_logger.h"

void test_clear_print_console()
{
    struct timer* t = timer_create_perf_timer();

    perf_timer_start(t);

    for(uint32 i = 0; i< 100000; i++) {
        print("....\n");
        print_move_to_top();
        print_clear_screen();
    }

    perf_timer_stop(t);
    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = 1, .buf_capacity = 1, .flags = PERF_T_USE_REPEAT|PERF_T_ALL
    }));
    perf_timer_reset(t);
}

int
main(void)
{
    /*
     * app_init()
     *
     * layer1 = create_layer()
     * layer_stack_push(layer1)
     *
     * layer2 = create_layer()
     * layer_stack_push(layer2)
     *
     * app_run()
     */

    app_init();

    struct layer layer1 = {.name="layer1",};
    layer_stack_push(layer1);

    struct layer layer2 = {.name="layer2",};
    layer_stack_push(layer2);

    app_run();

    return 0;
}
