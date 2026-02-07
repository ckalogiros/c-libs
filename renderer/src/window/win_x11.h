/*
 * Created by ckalo on 2/3/26.
 */
#ifndef WINDOW_WIN_X11_H
#define WINDOW_WIN_X11_H

#include "my_types.h"

struct os_window;

struct os_window* win_x11_create(const uint32 xpos, const uint32 ypos,
                                 const uint32 w, const uint32 h);
#endif //WINDOW_WIN_X11_H