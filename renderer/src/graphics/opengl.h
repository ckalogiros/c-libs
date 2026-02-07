/*
 * Created by ckalo on 2/4/26.
 */
#ifndef RENDERER_OPENGL_H
#define RENDERER_OPENGL_H

struct os_window;
XVisualInfo* init_glx(struct os_window* win);

void opengl_create_context(uint64 window, void* display, void* visual);

uint32
generate_temp_gfx();

#endif //RENDERER_OPENGL_H