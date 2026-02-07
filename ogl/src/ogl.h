/*
 * Created by ckalo on 2/3/26.
 */
#ifndef OGL_OGL_H
#define OGL_OGL_H
#include "my_types.h"


struct gfx_context {
    void* window;
    uint32 win_width;
    uint32 win_height;
};


struct gfx_context* ogl_create_context(const uint32 win_width, const uint32 win_height);

void ogl_init(struct gfx_context* const ctx);

void ogl_run(struct gfx_context* ctx);

void ogl_destroy();

#endif //OGL_OGL_H