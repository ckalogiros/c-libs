#include <stdio.h>
#include "my_types.h"
#include "render_object.h"


typedef uint64 sid_t;

/// TODO: unite some flags into one
struct gfx_sid {
    sid_t attr;
    sid_t unif;
    sid_t out;
    sid_t pass;
    sid_t oper;
};


#define SWPOS3 3
#define SWPOS2 2
#define SSCALE2 2
#define SSCALE3 3
#define SROT    2
#define SCOLOR3 3
#define SCOLOR4 4
struct render_object {
    struct gfx_sid sid;
    float wpos[SWPOS3];
    float scale[SSCALE2];
    float rot[SROT];
    float col[SCOLOR4];

    float* vertices;
    uint32 vertex_count;
    uint32 stride;

    uint16 faces;

    int gfx; /// TODO: struct gfx = {start, count, vbid, ...} OR index to array buffer
};


struct render_object robj_buffer[16];
uint32 robj_count = 0;
struct render_object* get_new_robj()
{
    struct render_object* obj = &robj_buffer[robj_count];
    robj_count++;
    return obj;
}


int main(void)
{
    struct render_object* obj1 = get_new_robj();

    obj1.

    printf("Hello, World!\n");
    return 0;
}