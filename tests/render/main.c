/*
 * Created by ckalo on 4/8/26.
 */

// #include <glfw3.h>
#include "mem_man_interface.h"
#include "tests.h"



int main(void)
{

    memman_init();

    // triangle_simple();

    // instanced_triangles();
    // instanced_rectangles();

    // ssbo();
    // ssbo_more_attribs();
    // ssbo_with_vecs();
    // ssbo_with_vecs_more_attribs();
    // ssbo_with_vecs_test_perf();

    test_array_uniforms_block();
    // ubo();


    memman_terminate();


    return 0;
}




