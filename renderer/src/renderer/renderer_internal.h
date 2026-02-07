/*
 * Created by ckalo on 2/4/26.
 */
#ifndef RENDERER_RENDERER_INTERNAL_H
#define RENDERER_RENDERER_INTERNAL_H

#include "my_types.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

#define ERROR_DIR "/home/projects/clion/renderer/errors.txt"

struct os_window {
    Display *display;
    XVisualInfo* visual;
    Window window;
    int32 screen_id;
};




#endif //RENDERER_RENDERER_INTERNAL_H