/*
 * Created by ckalo on 4/22/26.
 */
#ifndef GRAPHICS_TEST_RUN_H
#define GRAPHICS_TEST_RUN_H
#include "gfx_interface.h"

struct render_queue {
    build_t program;
    build_t progidx;
    vbidx_t vao;
    vbidx_t vaidx;
    vbidx_t vbo;
    vbidx_t vbidx;
    ibidx_t ibo;
    ibidx_t ibidx;
    tbidx_t tbo;
    tbidx_t tbidx;

    uint32 draw_count;
};

struct render_queue* render_queue_get(const uint32 rqidx);


void render_queue_add_draw_count(const uint32 rqidx, const uint32 vertex_count);

uint32 renderer_renderqueue_add(const build_t program, const build_t progidx, const vbidx_t vao,
                                              const vbidx_t vaidx, const vbidx_t vbo, const vbidx_t vbidx);


void run();

#endif //GRAPHICS_TEST_RUN_H
