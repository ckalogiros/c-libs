/*
 * Created by ckalo on 3/20/26.
 */
#ifndef RENDERER_RENDERER_STRUCTS_H
#define RENDERER_RENDERER_STRUCTS_H

#include "my_types.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct os_window {
    Display *display;
    XVisualInfo* visual;
    Window window;
    int32 screen_id;
};


#endif //RENDERER_RENDERER_STRUCTS_H