/*
 * Created by ckalo on 2/3/26.
 */
#ifndef WINDOW_WAYLAND_H
#define WINDOW_WAYLAND_H

#include "window.h"

struct os_window* window_create(int w, int h, const char* title);

#endif //WINDOW_WAYLAND_H