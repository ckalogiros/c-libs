/*
 * Created by ckalo on 4/22/26.
 */
#ifndef GRAPHICS_TEST_RUN_H
#define GRAPHICS_TEST_RUN_H
#include "gfx_context.h"

struct timer;

struct render_queue {
    uint32   draw_count;
    sididx_t sid_idx;
    build_t  program;
    build_t  progidx;
    vbidx_t  vao;
    vbidx_t  vaidx;
    vbidx_t  vbo;
    vbidx_t  vbidx;
    ibidx_t  ibo;
    ibidx_t  ibidx;
    tbidx_t  tbo;
    tbidx_t  tbidx;

};

struct command_render_queue_params {
    uint32 rqidx;
    uint32 attribs_idx;
    uint32 vertex_count;
    uint32 index_count;
};

struct render_queue* render_queue_get(const uint32 rqidx);


void render_queue_add_draw_count(struct command_render_queue_params* params);

int16 renderer_renderqueue_add(const sididx_t sid_idx, const build_t program, const build_t progidx,
                                const vbidx_t vao, const vbidx_t vaidx, const vbidx_t vbo, const vbidx_t vbidx);


void run(struct timer* loop_timer);

#endif //GRAPHICS_TEST_RUN_H
