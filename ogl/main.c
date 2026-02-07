/*
 * Created by ckalo on 2/3/26.
 */

#include "logger.h"
#include "src/ogl.h"

int main(void)
{


    struct gfx_context* const ctx =  ogl_create_context(800, 600);

    ogl_init(ctx);

    // create app meshes here

    ogl_run(ctx);

    ogl_destroy();

    return 0;
}
