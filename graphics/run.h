/*
 * Created by ckalo on 4/22/26.
 */
#ifndef GRAPHICS_TEST_RUN_H
#define GRAPHICS_TEST_RUN_H
#include "gfx_interface.h"

struct render_queue {
    progidx_t program;
    progidx_t progidx;
    vbidx_t vao;
    vbidx_t vaidx;
    vbidx_t vbo;
    vbidx_t vbidx;
    ibidx_t ibo;
    ibidx_t ibidx;
    tbidx_t tbo;
    tbidx_t tbidx;
};

struct render_queue* render_queue_get();
struct render_queue* renderer_renderqueue_add(progidx_t program, progidx_t progidx,
                                            vbidx_t vao, vbidx_t vaidx,vbidx_t vbo, vbidx_t vbidx);



void run();

#endif //GRAPHICS_TEST_RUN_H