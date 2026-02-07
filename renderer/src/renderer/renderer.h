/*
 * Created by ckalo on 2/4/26.
 */
#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include "my_types.h"

struct os_window* win_x11_init_display();

struct os_window* renderer_create_x11_window(const uint32 xpos, const uint32 ypos, const uint32 w, const uint32 h);

void render(struct os_window* os_win);

#endif //RENDERER_RENDERER_H