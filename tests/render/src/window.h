/*
 * Created by ckalo on 3/28/26.
 */
#ifndef LAYER_STACK_WINDOW_H
#define LAYER_STACK_WINDOW_H

#define GLFW_INCLUDE_NONE
#include <glfw3.h>
#include "my_types.h"

void window_create(const uint32 width, const uint32 height);
void window_terminate();

// void window_update(struct layer* layer);

// TODO: Replace GLFWwindow with abstruct window struct
GLFWwindow* window_get();
uint32 window_get_width();
uint32 window_get_height();
bool window_should_close();
void window_poll_events();
bool window_swap_buffers();

vec2f64 io_mouse_get_pos();

#endif //LAYER_STACK_WINDOW_H