/*
 * Created by ckalo on 3/28/26.
 */

#include "app.h"

#include "layer.h"

void app_init()
{
    // set glfw
    // create window
    // ...
}

void app_run()
{
    /*
        while(app_running) {

            if(glfw_should_close())
                exit app

            layer_on_update();

            layer_on_render()

        }
    */

    while(1) {

        // if(glfw_should_close())
        //     exit app

        layer_on_update();

        layer_on_render();

    }
}