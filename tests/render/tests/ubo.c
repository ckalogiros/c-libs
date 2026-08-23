/*
 * Created by ckalo on 8/15/26.
 */

#include <stdalign.h>

#include "tests.h"
#include "opengl.h"
#include "window.h"
#include "camera.h"
#include "GLOBALS.h"
#include "opengl_debug.h"

#define RGBA1  0.9, 0.2, 0.2, 1.0
#define RGBA2  0.9, 0.6, 0.2, 1.0
#define RGBA3  0.9, 0.8, 0.2, 1.0
#define RGBA4  0.2, 0.9, 0.3, 1.0
#define RGBA5  0.2, 0.9, 0.9, 1.0
#define RGBA6  0.2, 0.6, 0.9, 1.0
#define RGBA7  0.2, 0.3, 0.9, 1.0
#define RGBA8  0.4, 0.2, 0.9, 1.0
#define RGBA9  0.9, 0.2, 0.9, 1.0
#define RGBA10 0.9, 0.2, 0.6, 1.0

static void draw(GLint program, GLint vao, uint32 draw_count)
{
    // const float t = glfwGetTime()*1.;
    gl_use_program(program);                            GFX_CHECK_ERROR();
    gl_bind_vertex_array(vao);                          GFX_CHECK_ERROR();
    gl_draw_arrays(OPENGL_TRIANGLES, 0, draw_count);    GFX_CHECK_ERROR();
    glBindVertexArray(0);
}

static void run(GLint program, GLint vao, uint32 draw_count)
{
    vec4f color = {.r = .1, .g = .1, .b = .1, .a = 1.0};
    while(1) {
        window_poll_events();
        if(window_should_close()) {
            window_terminate();
            break;
        }
        gl_clear();
        gl_clear_color(color);
        draw(program, vao, draw_count);
        window_swap_buffers();
    }
}

static void window_setup()
{
    window_create(SCREEN_RES_X, SCREEN_RES_Y);
    gl_viewport(SCREEN_RES_X, SCREEN_RES_Y);
}

static void opengl_setup()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_MULTISAMPLE);
}

/* !!! IMPORTANT !!!
 * std140 uniform
 * According to the specification, in an array of elements,
 * each elements is interpeted by 16 bytes.
 * So: float arr[3] -> arr[0] is 16 bytes(4 floats)
 *                  -> arr[1] is 16 bytes(4 floats)
 *                  -> arr[2] is 16 bytes(4 floats)
 *
 * Use of layout: packed and shared
 * using any of the 2 layouts, seems we can have arrays of uniform elements
 */

void test_array_uniforms_block()
{
    window_setup();
    opengl_setup();

    gl_print_context_info();
    gl_debug_enable();

    GLuint program = 0;
    {
        const char* vshader = "\n\
#version 430 core\n\
layout (location = 0) in vec2 vpos2;\n\
layout (location = 1) in vec4 color4;\n\
uniform mat4 projMat;\n\
out vec4 out_color4;\n\
#define TRI_COUNT 6                    \n\
uniform int elems;                     \n\
\n\
layout(shared, binding = 0) uniform data { \n\
    int wpos3_elems_count;\n\
    float wpos3[3*TRI_COUNT];               \n\
};\n\
void main()\n\
{\n\
    int idx     = (gl_VertexID / 3)*wpos3_elems_count;            \n\
    vec4 vpos     = vec4(vpos2.x+wpos3[idx+0],    \n\
                         vpos2.y+wpos3[idx+1],    \n\
                         wpos3[idx+2], 1.0);      \n\
    //vec4 vpos   = vec4(vpos2.x, vpos2.y, wpos3[2], 1.0);\n\
    //vec4 vpos   = vec4(vpos2.x, vpos2.y, -15.0, 1.0);\n\
    gl_Position = projMat * vpos;\n\
    out_color4  = vec4(color4.x, color4.y, color4.z, color4.w);\n\
}\n\
";

        const char* fshader = "\n\
#version 430\n\
in vec4 out_color4;\n\
out vec4 fragment;\n\
void main()\n\
{\n\
    fragment = out_color4;\n\
}\n";
        print("%s %s %s\n", ry, vshader, CLEAR);
        print("%s %s %s\n", ry, fshader, CLEAR);

        const GLuint vertex_shader = gl_create_shader(OPENGL_VERTEX_SHADER);
        gl_shader_source(vertex_shader, 1, &vshader, NULL);
        gl_compile_shader(vertex_shader);
        GFX_CHECK_ERROR();

        const GLuint fragment_shader = gl_create_shader(OPENGL_FRAGMENT_SHADER);
        gl_shader_source(fragment_shader, 1, &fshader, NULL);
        gl_compile_shader(fragment_shader);
        GFX_CHECK_ERROR();

        program = gl_create_program();
        gl_attach_shader(program, vertex_shader);
        gl_attach_shader(program, fragment_shader);
        gl_link_program(program);
        GFX_CHECK_ERROR();

        gl_delete_shader(vertex_shader);
        gl_delete_shader(fragment_shader);
        GFX_CHECK_ERROR();

    }

    float attribs[] = {
        -1,  1, RGBA2,
        -1, -1, RGBA4,
         1, -1, RGBA7,
        -1,  1, RGBA8,
        -1, -1, RGBA9,
         1, -1, RGBA1,
        -1,  1, RGBA10,
        -1, -1, RGBA3,
         1, -1, RGBA7,
        -1,  1, RGBA9,
        -1, -1, RGBA5,
         1, -1, RGBA6,
        -1,  1, RGBA5,
        -1, -1, RGBA4,
         1, -1, RGBA3,
        -1,  1, RGBA5,
        -1, -1, RGBA8,
         1, -1, RGBA7,
    };
#   define tri_count 6
    uint32 typesize  = SIZE_F32,
           stride    = 2+4,
           vertices  = 3*tri_count,
           attribs_count = stride * vertices;


    GLuint vao, vbo;
    {
        gl_gen_vertex_array(1, &vao);
        gl_bind_vertex_array(vao);

        gl_gen_buffers(1, &vbo);
        gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo);
        gl_buffer_data(OPENGL_ARRAY_BUFFER, attribs_count*typesize, attribs, OPENGL_STATIC_DRAW);

        int32 loc = -1, offset = -1, attrib_elems = -1;
        loc = 0, offset = 0, attrib_elems = 2;
        gl_enable_vertex_attrib_array(loc);                                                  GFX_CHECK_ERROR();
        gl_enable_attrib_pointer(loc, attrib_elems, stride * typesize, offset * typesize);   GFX_CHECK_ERROR();
        gl_vertex_attrib_divisor(loc, 0);                                                    GFX_CHECK_ERROR();

        loc = 1, offset = 2, attrib_elems = 4;
        gl_enable_vertex_attrib_array(loc);                                                  GFX_CHECK_ERROR();
        gl_enable_attrib_pointer(loc, attrib_elems, stride * typesize, offset * typesize);   GFX_CHECK_ERROR();
        gl_vertex_attrib_divisor(loc, 0);                                                    GFX_CHECK_ERROR();

        struct uniform_block {
            uint32 wpos3_elems_count;
            float wpos3[tri_count*3];
        };
        struct uniform_block ub_data = {
            .wpos3_elems_count = 3,
            .wpos3 = {
               -5, 0, -15,
               -3, 0, -15,
               -1, 0, -15,
                1, 0, -15,
                3, 0, -15,
                5, 0, -15,
           },
        };

        // 2. Generate, bind, and upload data to the GPU
        GLuint ubo;
        gl_gen_buffers(1, &ubo);
        gl_bind_buffer(GL_UNIFORM_BUFFER, ubo);
        gl_buffer_data(GL_UNIFORM_BUFFER, sizeof(struct uniform_block), &ub_data, GL_STATIC_DRAW);

        // 3. Bind the SSBO to binding point 0
        gl_bind_buffer_base(GL_UNIFORM_BUFFER, 0, ubo);


        // Optional: unbind to avoid accidental changes
        gl_bind_buffer(GL_UNIFORM_BUFFER, 0);


        print("Attribs array size: %zu bytes\n", sizeof(attribs));
        print("Expected size: %d bytes\n", vertices * stride * SIZE_F32);
        print("Vertices: %d, Triangles: %d\n", vertices, tri_count);

        // // Verify UBO data
        // for (int i = 0; i < tri_count; i++) {
        //     print("Triangle %d position: (%.1f, %.1f, %.1f)\n",
        //            i, wpos[i*3], wpos[i*3+1], wpos[i*3+2]);
        // }

        // After linking the program
        GLuint blockIndex = glGetUniformBlockIndex(program, "data");
        if (blockIndex == GL_INVALID_INDEX) {
            print("ERROR: Uniform block 'data' not found!\n");
            return;
        }

        GLint blockSize;
        glGetActiveUniformBlockiv(program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
        print("UBO block size in shader: %d bytes\n", blockSize);
        print("Your uniform_block data size: %zu bytes\n", sizeof(struct uniform_block));

        if (blockSize != sizeof(struct uniform_block)) {
            print("WARNING: Size mismatch! Shader expects %d bytes but you're uploading %zu bytes\n",
                   blockSize, sizeof(struct uniform_block));
        }
    }


    // GLuint blockIndex = glGetUniformBlockIndex(program, "data");
    // glUniformBlockBinding(program, blockIndex, 0);

    gl_use_program(program);

    struct camera camera;
    {
        /// create camera
        float aspect = (float)SCREEN_RES_X / (float)SCREEN_RES_Y;
        float near = 0.01f;
        float far = 100.0f;
        float fov = 45.0f;
        struct camera_projection_matrix projection_matrix =
            scene_create_perspective_projection_matrix(fov, near, far, aspect);

        camera = camera_create(projection_matrix, GL_FALSE);
        GLint uinif_loc = glGetUniformLocation(program, "projMat");
        glUniformMatrix4fv(uinif_loc, 1, camera.is_column_major, &camera.projection_matrix.projection_matrix[0]);
    }

    /// uniforms
    {
        GLint uinif_loc = glGetUniformLocation(program, "elems");
        glUniform1i(uinif_loc, 3);
    }


    run(program, vao, vertices);
#   undef tri_count
}

void ubo()
{
    window_setup();
    opengl_setup();

    gl_print_context_info();
    gl_debug_enable();

    GLuint program = 0;
    {
        const char* vshader = "\n\
#version 430 core\n\
layout (location = 0) in vec2 vpos2;\n\
layout (location = 1) in vec4 color4;\n\
uniform mat4 projMat;\n\
out vec4 out_color4;\n\
#define TRI_COUNT 2                     \n\
\n\
layout(shared, binding = 0) uniform data { \n\
    float wpos3[3*TRI_COUNT];               \n\
};\n\
void main()\n\
{\n\
    int idx     = (gl_VertexID / 3)*3;            \n\
    vec4 vpos     = vec4(vpos2.x+wpos3[idx+0],    \n\
                         vpos2.y+wpos3[idx+1],    \n\
                         wpos3[idx+2], 1.0);      \n\
    //vec4 vpos   = vec4(vpos2.x, vpos2.y, wpos3[2], 1.0);\n\
    //vec4 vpos   = vec4(vpos2.x, vpos2.y, -15.0, 1.0);\n\
    gl_Position = projMat * vpos;\n\
    out_color4  = vec4(color4.x, color4.y, color4.z, color4.w);\n\
}\n\
";

        const char* fshader = "\n\
#version 430\n\
in vec4 out_color4;\n\
out vec4 fragment;\n\
void main()\n\
{\n\
    fragment = out_color4;\n\
}\n";
        print("%s %s %s\n", ry, vshader, CLEAR);
        print("%s %s %s\n", ry, fshader, CLEAR);

        const GLuint vertex_shader = gl_create_shader(OPENGL_VERTEX_SHADER);
        gl_shader_source(vertex_shader, 1, &vshader, NULL);
        gl_compile_shader(vertex_shader);
        GFX_CHECK_ERROR();

        const GLuint fragment_shader = gl_create_shader(OPENGL_FRAGMENT_SHADER);
        gl_shader_source(fragment_shader, 1, &fshader, NULL);
        gl_compile_shader(fragment_shader);
        GFX_CHECK_ERROR();

        program = gl_create_program();
        gl_attach_shader(program, vertex_shader);
        gl_attach_shader(program, fragment_shader);
        gl_link_program(program);
        GFX_CHECK_ERROR();

        gl_delete_shader(vertex_shader);
        gl_delete_shader(fragment_shader);
        GFX_CHECK_ERROR();

    }

    float attribs[] = {
        -1,  1, RGBA2,
        -1, -1, RGBA4,
         1, -1, RGBA7,
        -1,  1, RGBA8,
        -1, -1, RGBA9,
         1, -1, RGBA1,
        // -1,  1, RGBA2,
        // -1, -1, RGBA4,
        //  1, -1, RGBA7,
        // -1,  1, RGBA2,
        // -1, -1, RGBA4,
        //  1, -1, RGBA7,
        // -1,  1, RGBA2,
        // -1, -1, RGBA4,
        //  1, -1, RGBA7,
        // -1,  1, RGBA2,
        // -1, -1, RGBA4,
        //  1, -1, RGBA7,
    };
#   define tri_count 2
    uint32 typesize  = SIZE_F32,
           stride    = 2+4,
           vertices  = 3*tri_count,
           attribs_count = stride * vertices;


    GLuint vao, vbo;
    {
        gl_gen_vertex_array(1, &vao);
        gl_bind_vertex_array(vao);

        gl_gen_buffers(1, &vbo);
        gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo);
        gl_buffer_data(OPENGL_ARRAY_BUFFER, attribs_count*typesize, attribs, OPENGL_STATIC_DRAW);

        int32 loc = -1, offset = -1, attrib_elems = -1;
        loc = 0, offset = 0, attrib_elems = 2;
        gl_enable_vertex_attrib_array(loc);                                                  GFX_CHECK_ERROR();
        gl_enable_attrib_pointer(loc, attrib_elems, stride * typesize, offset * typesize);   GFX_CHECK_ERROR();
        gl_vertex_attrib_divisor(loc, 0);                                                    GFX_CHECK_ERROR();

        loc = 1, offset = 2, attrib_elems = 4;
        gl_enable_vertex_attrib_array(loc);                                                  GFX_CHECK_ERROR();
        gl_enable_attrib_pointer(loc, attrib_elems, stride * typesize, offset * typesize);   GFX_CHECK_ERROR();
        gl_vertex_attrib_divisor(loc, 0);                                                    GFX_CHECK_ERROR();

        // float wpos = 3.0;
        float wpos[] = {
             0, 0, -15,
            -5, 0, -15,
            // -3, 0, -15,
            // -1, 0, -15,
            //  1, 0, -15,
            //  3, 0, -15,
            //  5, 0, -15,
        };

        // 2. Generate, bind, and upload data to the GPU
        GLuint ubo;
        gl_gen_buffers(1, &ubo);
        gl_bind_buffer(GL_UNIFORM_BUFFER, ubo);
        gl_buffer_data(GL_UNIFORM_BUFFER, sizeof(wpos), &wpos[0], GL_STATIC_DRAW);

        // 3. Bind the SSBO to binding point 0
        gl_bind_buffer_base(GL_UNIFORM_BUFFER, 0, ubo);


        // Optional: unbind to avoid accidental changes
        gl_bind_buffer(GL_UNIFORM_BUFFER, 0);


        // print("Att<ribs array size: %zu bytes\n", sizeof(attribs));
        // print("Expected size: %d bytes\n", vertices * stride * SIZE_F32);
        // print("Vertices: %d, Triangles: %d\n", vertices, tri_count);
        //
        // // Verify UBO data
        // for (int i = 0; i < tri_count; i++) {
        //     print("Triangle %d position: (%.1f, %.1f, %.1f)\n",
        //            i, wpos[i*3], wpos[i*3+1], wpos[i*3+2]);
        // }
        //
        // // After linking the program
        // GLuint blockIndex = glGetUniformBlockIndex(program, "data");
        // if (blockIndex == GL_INVALID_INDEX) {
        //     print("ERROR: Uniform block 'data' not found!\n");
        //     return;
        // }
        //
        // GLint blockSize;
        // glGetActiveUniformBlockiv(program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
        // print("UBO block size in shader: %d bytes\n", blockSize);
        // print("Your wpos data size: %zu bytes\n", sizeof(wpos));
        //
        // if (blockSize != sizeof(wpos)) {
        //     print("WARNING: Size mismatch! Shader expects %d bytes but you're uploading %zu bytes\n",
        //            blockSize, sizeof(wpos));
        // }>
    }


    // GLuint blockIndex = glGetUniformBlockIndex(program, "data");
    // glUniformBlockBinding(program, blockIndex, 0);

    gl_use_program(program);

    struct camera camera;
    {
        /// create camera
        float aspect = (float)SCREEN_RES_X / (float)SCREEN_RES_Y;
        float near = 0.01f;
        float far = 100.0f;
        float fov = 45.0f;
        struct camera_projection_matrix projection_matrix =
            scene_create_perspective_projection_matrix(fov, near, far, aspect);

        camera = camera_create(projection_matrix, GL_FALSE);
        GLint uinif_loc = glGetUniformLocation(program, "projMat");
        if(uinif_loc != -1) {}
        glUniformMatrix4fv(uinif_loc, 1, camera.is_column_major, &camera.projection_matrix.projection_matrix[0]);
    }


    run(program, vao, vertices);
#   undef tri_count
}

/* in this test we create wpos and color in ubo struct */
void ubo_more_attribs()
{
    window_setup();
    opengl_setup();

    gl_debug_enable();


    GLuint program = 0;
    {
        const char* vshader = "\n\
#version 430 core                       \n\
layout (location = 0) in vec2 vpos2;    \n\
uniform mat4 projMat;                   \n\
out vec4 out_color4;                    \n\
#define TRI_COUNT 6                     \n\
\n\
layout(std430, binding = 0) buffer data { \n\
    float wpos3[3*TRI_COUNT];               \n\
    float color4[4*TRI_COUNT];              \n\
};\n\
void main()\n\
{\n\
    int idxw     = (gl_VertexID / 3) *3;                                \n\
    vec3 p       = vec3(wpos3[idxw+0], wpos3[idxw+1], wpos3[idxw+2]);   \n\
    vec4 vpos    = vec4(vpos2.x+p.x, vpos2.y+p.y, p.z, 1.0);            \n\
    gl_Position  = projMat * vpos;                                      \n\
    int idxc     = (gl_VertexID / 3) *4;                                \n\
    vec4 c       = vec4(color4[idxc+0], color4[idxc+1],                 \n\
                        color4[idxc+2], color4[idxc+3]);                \n\
    out_color4   = vec4(c.x, c.y, c.z, c.w);                            \n\
}\n\
";

        const char* fshader = "\n\
#version 430        \n\
in vec4 out_color4; \n\
out vec4 fragment;  \n\
void main()         \n\
{\n\
    fragment = out_color4;\n\
}\n";
        print("%s %s %s\n", ry, vshader, CLEAR);
        print("%s %s %s\n", ry, fshader, CLEAR);

        const GLuint vertex_shader = gl_create_shader(OPENGL_VERTEX_SHADER);
        gl_shader_source(vertex_shader, 1, &(vshader), NULL);
        gl_compile_shader(vertex_shader);
        GFX_CHECK_ERROR();

        const GLuint fragment_shader = gl_create_shader(OPENGL_FRAGMENT_SHADER);
        gl_shader_source(fragment_shader, 1, &fshader, NULL);
        gl_compile_shader(fragment_shader);
        GFX_CHECK_ERROR();

        program = gl_create_program();
        gl_attach_shader(program, vertex_shader);
        gl_attach_shader(program, fragment_shader);
        gl_link_program(program);
        GFX_CHECK_ERROR();

        gl_delete_shader(vertex_shader);
        gl_delete_shader(fragment_shader);
        GFX_CHECK_ERROR();

    }

    float attribs[] = {
        -1,  1,
        -1, -1,
         1, -1,
        -1,  1,
        -1, -1,
         1, -1,
        -1,  1,
        -1, -1,
         1, -1,
        -1,  1,
        -1, -1,
         1, -1,
        -1,  1,
        -1, -1,
         1, -1,
        -1,  1,
        -1, -1,
         1, -1,
    };

#   define tri_count 6
    uint32 typesize  = SIZE_F32,
           stride    = 2,
           vertices  = 3*tri_count,
           attribs_count = stride * vertices;

    GLuint vao, vbo;
    {
        gl_gen_vertex_array(1, &vao);
        gl_bind_vertex_array(vao);

        gl_gen_buffers(1, &vbo);
        gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo);
        gl_buffer_data(OPENGL_ARRAY_BUFFER, attribs_count*typesize, attribs, OPENGL_STATIC_DRAW);

        int32 loc = -1, offset = -1, attrib_elems = -1;
        loc = 0, offset = 0, attrib_elems = 2;
        gl_enable_vertex_attrib_array(loc);                                                  GFX_CHECK_ERROR();
        gl_enable_attrib_pointer(loc, attrib_elems, stride * typesize, offset * typesize);   GFX_CHECK_ERROR();
        gl_vertex_attrib_divisor(loc, 0);                                                    GFX_CHECK_ERROR();


        struct attribs {
            float wpos[3*tri_count];
            float col[4*tri_count];
        };
        struct attribs data = {
            .wpos = {
                -5, 0, -15,
                -3, 0, -15,
                -1, 0, -15,
                 1, 0, -15,
                 3, 0, -15,
                 5, 0, -15,
            },
            .col = {
                0.9, 0.6, 0.1, 1.0,
                0.1, 0.6, 0.1, 1.0,
                0.1, 0.6, 0.9, 1.0,
                0.9, 0.1, 0.9, 1.0,
                0.9, 0.1, 0.1, 1.0,
                0.1, 0.3, 0.9, 1.0,
            }
        };


        // 2. Generate, bind, and upload data to the GPU
        GLuint ubo;
        gl_gen_buffers(1, &ubo);
        gl_bind_buffer(GL_SHADER_STORAGE_BUFFER, ubo);

        // gl_buffer_data(GL_SHADER_STORAGE_BUFFER, sizeof(struct attribs), &data, GL_DYNAMIC_DRAW);
        gl_buffer_data(GL_SHADER_STORAGE_BUFFER, sizeof(struct attribs), NULL, GL_STATIC_DRAW);

        // Calculate sizes
        size_t wpos_size  = 3 * tri_count * SIZE_F32;
        size_t color_size = 4 * tri_count * SIZE_F32;
        size_t total_size = wpos_size + color_size;
        gl_buffer_sub_data(GL_SHADER_STORAGE_BUFFER, 0, wpos_size, data.wpos);
        gl_buffer_sub_data(GL_SHADER_STORAGE_BUFFER, wpos_size, color_size, data.col);

        // 3. Bind the SSBO to binding point 0
        gl_bind_buffer_base(GL_SHADER_STORAGE_BUFFER, 0, ubo);
        // Optional: unbind to avoid accidental changes
        // gl_bind_buffer(GL_SHADER_STORAGE_BUFFER, 0);
    }


    gl_use_program(program);

    struct camera camera;
    {
        /// create camera
        float aspect = (float)SCREEN_RES_X / (float)SCREEN_RES_Y;
        float near   = 0.01f;
        float far    = 100.0f;
        float fov    = 45.0f;
        struct camera_projection_matrix projection_matrix =
            scene_create_perspective_projection_matrix(fov, near, far, aspect);

        camera = camera_create(projection_matrix, GL_FALSE);
        GLint uinif_loc = glGetUniformLocation(program, "projMat");
        if(uinif_loc != -1) {}
            glUniformMatrix4fv(uinif_loc, 1, camera.is_column_major, &camera.projection_matrix.projection_matrix[0]);
    }


    run(program, vao, vertices);
#   undef tri_count
}

/* in this test we set up the wpos as a vector and not as a buffer of floats.
 * The issue is that glsl needs padding for the vec3 wpos,
 * because it gets 16 bytes of data and then switches to the next tri vec3 position
 */
void ubo_with_vecs()
{
    window_setup();
    opengl_setup();

    gl_debug_enable();

    GLuint program = 0;
    {
        /*
 * In the shader we no longer have to access the wpos3 array
 * tri1:000, tri2: 333 but rather
 * tri1:000, tri2: 111, because it is an array of vec3s
 */
        const char* vshader = "\n\
#version 430 core\n\
layout (location = 0) in vec2 vpos2;\n\
layout (location = 1) in vec4 color4;\n\
uniform mat4 projMat;\n\
out vec4 out_color4;\n\
\n\
//layout(std430, binding = 0) buffer data {\n\
layout(std430, binding = 0) buffer data {\n\
    vec3 wpos3[];\n\
};\n\
void main()\n\
{\n\
    int idxw    = (gl_VertexID / 3);        \n\
    vec4 vpos   = vec4(vpos2.x+wpos3[idxw].x,   \n\
                       vpos2.y+wpos3[idxw].y,   \n\
                       wpos3[idxw].z, 1.0);     \n\
    gl_Position = projMat * vpos;\n\
    out_color4  = vec4(color4.x, color4.y, color4.z, color4.w);\n\
}\n\
";

        const char* fshader = "\n\
#version 430\n\
in vec4 out_color4;\n\
out vec4 fragment;\n\
void main()\n\
{\n\
    fragment = out_color4;\n\
}\n";
        print("%s %s %s\n", ry, vshader, CLEAR);
        print("%s %s %s\n", ry, fshader, CLEAR);

        const GLuint vertex_shader = gl_create_shader(OPENGL_VERTEX_SHADER);
        gl_shader_source(vertex_shader, 1, &(vshader), NULL);
        gl_compile_shader(vertex_shader);
        GFX_CHECK_ERROR();

        const GLuint fragment_shader = gl_create_shader(OPENGL_FRAGMENT_SHADER);
        gl_shader_source(fragment_shader, 1, &fshader, NULL);
        gl_compile_shader(fragment_shader);
        GFX_CHECK_ERROR();

        program = gl_create_program();
        gl_attach_shader(program, vertex_shader);
        gl_attach_shader(program, fragment_shader);
        gl_link_program(program);
        GFX_CHECK_ERROR();

        gl_delete_shader(vertex_shader);
        gl_delete_shader(fragment_shader);
        GFX_CHECK_ERROR();

    }

#   define tri_count 6

    float attribs[] = {
        -1,  1, 0.9, 0.6, 0.1, 1.0,
        -1, -1, 0.7, 0.1, 0.9, 1.0,
         1, -1, 0.1, 0.7, 0.9, 1.0,
        -1,  1, 0.9, 0.6, 0.1, 1.0,
        -1, -1, 0.7, 0.1, 0.9, 1.0,
         1, -1, 0.1, 0.7, 0.9, 1.0,
        -1,  1, 0.9, 0.6, 0.1, 1.0,
        -1, -1, 0.7, 0.1, 0.9, 1.0,
         1, -1, 0.1, 0.7, 0.9, 1.0,
        -1,  1, 0.9, 0.6, 0.1, 1.0,
        -1, -1, 0.7, 0.1, 0.9, 1.0,
         1, -1, 0.1, 0.7, 0.9, 1.0,
        -1,  1, 0.9, 0.6, 0.1, 1.0,
        -1, -1, 0.7, 0.1, 0.9, 1.0,
         1, -1, 0.1, 0.7, 0.9, 1.0,
        -1,  1, 0.9, 0.6, 0.1, 1.0,
        -1, -1, 0.7, 0.1, 0.9, 1.0,
         1, -1, 0.1, 0.7, 0.9, 1.0,
    };
    uint32 typesize  = SIZE_F32,
           stride    = 2+4,
           vertices  = 3*tri_count,
           attribs_count = stride * vertices;

    GLuint vao, vbo;
    {
        gl_gen_vertex_array(1, &vao);
        gl_bind_vertex_array(vao);

        gl_gen_buffers(1, &vbo);
        gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo);
        gl_buffer_data(OPENGL_ARRAY_BUFFER, attribs_count*typesize, attribs, OPENGL_STATIC_DRAW);

        int32 loc = -1, offset = -1, attrib_elems = -1;
        loc = 0, offset = 0, attrib_elems = 2;
        gl_enable_vertex_attrib_array(loc);                                                  GFX_CHECK_ERROR();
        gl_enable_attrib_pointer(loc, attrib_elems, stride * typesize, offset * typesize);   GFX_CHECK_ERROR();
        gl_vertex_attrib_divisor(loc, 0);                                                    GFX_CHECK_ERROR();

        loc = 1, offset = 2, attrib_elems = 4;
        gl_enable_vertex_attrib_array(loc);                                                  GFX_CHECK_ERROR();
        gl_enable_attrib_pointer(loc, attrib_elems, stride * typesize, offset * typesize);   GFX_CHECK_ERROR();
        gl_vertex_attrib_divisor(loc, 0);                                                    GFX_CHECK_ERROR();

       /* by default std430 and std140 use 16 bytes alignment,
        * so for the vec3 expects padding of 4 bytes.
        * Here we force alignment to 16 bytes, or we could have padding in the struct
        */
       struct vec3 {
           float x; float y; float z;
       // };
       } __attribute__((aligned(16)));
        struct vec3 wpos[] = {
            {.x=-5, .y=0, .z=-15, },
            {.x=-3, .y=0, .z=-15, },
            {.x=-1, .y=0, .z=-15, },
            {.x= 1, .y=0, .z=-15, },
            {.x= 3, .y=0, .z=-15, },
            {.x= 5, .y=0, .z=-15, },
        };

        // 2. Generate, bind, and upload data to the GPU
        GLuint ubo;
        gl_gen_buffers(1, &ubo);
        gl_bind_buffer(GL_SHADER_STORAGE_BUFFER, ubo);
        gl_buffer_data(GL_SHADER_STORAGE_BUFFER, sizeof(wpos), wpos, GL_STATIC_DRAW);

        // 3. Bind the SSBO to binding point 0
        gl_bind_buffer_base(GL_SHADER_STORAGE_BUFFER, 0, ubo);
        // Optional: unbind to avoid accidental changes
        gl_bind_buffer(GL_SHADER_STORAGE_BUFFER, 0);
    }


    gl_use_program(program);

    struct camera camera;
    {
        /// create camera
        float aspect = (float)SCREEN_RES_X / (float)SCREEN_RES_Y;
        float near   = 0.01f;
        float far    = 100.0f;
        float fov    = 45.0f;
        struct camera_projection_matrix projection_matrix =
            scene_create_perspective_projection_matrix(fov, near, far, aspect);

        camera = camera_create(projection_matrix, GL_FALSE);
        GLint uinif_loc = glGetUniformLocation(program, "projMat");
        if(uinif_loc != -1) {}
            glUniformMatrix4fv(uinif_loc, 1, camera.is_column_major, &camera.projection_matrix.projection_matrix[0]);
    }


    run(program, vao, vertices);

#   undef tri_count
}

/* same(pass ubo as array of vectors) but color added to the ubo
 */
void ubo_with_vecs_more_attribs()
{
    window_setup();
    opengl_setup();

    gl_debug_enable();

    GLuint program = 0;
    {
        /* here we get the color and wpos as arrays of vectors, not as arrays of floats */
        const char* vshader = "\n\
#version 430 core\n\
layout (location = 0) in vec2 vpos2;\n\
uniform mat4 projMat;\n\
out vec4 out_color4;\n\
#define tri_count 6\n\
\n\
//layout(std430, binding = 0) buffer data {\n\
layout(std430, binding = 0) buffer data {\n\
    vec4 color4[tri_count];\n\
    vec4 wpos3[tri_count];\n\
};\n\
void main()\n\
{\n\
    int idx     = (gl_VertexID / 3);            \n\
    vec4 vpos   = vec4(vpos2.x+wpos3[idx].x,    \n\
                       vpos2.y+wpos3[idx].y,    \n\
                       wpos3[idx].z, 1.0);      \n\
    gl_Position = projMat * vpos;\n\
    out_color4  = vec4(color4[idx].r, color4[idx].g, color4[idx].b, color4[idx].a);                            \n\
}\n\
";

        const char* fshader = "\n\
#version 430\n\
in vec4 out_color4;\n\
out vec4 fragment;\n\
void main()\n\
{\n\
    fragment = out_color4;\n\
}\n";
        print("%s %s %s\n", ry, vshader, CLEAR);
        print("%s %s %s\n", ry, fshader, CLEAR);

        const GLuint vertex_shader = gl_create_shader(OPENGL_VERTEX_SHADER);
        gl_shader_source(vertex_shader, 1, &(vshader), NULL);
        gl_compile_shader(vertex_shader);
        GFX_CHECK_ERROR();

        const GLuint fragment_shader = gl_create_shader(OPENGL_FRAGMENT_SHADER);
        gl_shader_source(fragment_shader, 1, &fshader, NULL);
        gl_compile_shader(fragment_shader);
        GFX_CHECK_ERROR();

        program = gl_create_program();
        gl_attach_shader(program, vertex_shader);
        gl_attach_shader(program, fragment_shader);
        gl_link_program(program);
        GFX_CHECK_ERROR();

        gl_delete_shader(vertex_shader);
        gl_delete_shader(fragment_shader);
        GFX_CHECK_ERROR();

    }

#   define tri_count 6

    float attribs[] = {
        -1,  1,
        -1, -1,
         1, -1,
        -1,  1,
        -1, -1,
         1, -1,
        -1,  1,
        -1, -1,
         1, -1,
        -1,  1,
        -1, -1,
         1, -1,
        -1,  1,
        -1, -1,
         1, -1,
        -1,  1,
        -1, -1,
         1, -1,
    };
    uint32 typesize  = SIZE_F32,
           stride    = 2,
           vertices  = 3*tri_count,
           attribs_count = stride * vertices;

    GLuint vao, vbo;
    {
        gl_gen_vertex_array(1, &vao);
        gl_bind_vertex_array(vao);

        gl_gen_buffers(1, &vbo);
        gl_bind_buffer(OPENGL_ARRAY_BUFFER, vbo);
        gl_buffer_data(OPENGL_ARRAY_BUFFER, attribs_count*typesize, attribs, OPENGL_STATIC_DRAW);

        int32 loc = -1, offset = -1, attrib_elems = -1;
        loc = 0, offset = 0, attrib_elems = 2;
        gl_enable_vertex_attrib_array(loc);                                                  GFX_CHECK_ERROR();
        gl_enable_attrib_pointer(loc, attrib_elems, stride * typesize, offset * typesize);   GFX_CHECK_ERROR();
        gl_vertex_attrib_divisor(loc, 0);                                                    GFX_CHECK_ERROR();

       /* align to 16 bytes
        */
        struct vec4 {
            float x, y, z, w;
        };
       struct my_data {
           struct vec4 color4[tri_count];
           struct vec4 wpos3[tri_count];
       };
        struct my_data data = {
            .color4 = {
                {RGBA1, .w=1.0,},
                {RGBA3, .w=1.0,},
                {RGBA4, .w=1.0,},
                {RGBA6, .w=1.0,},
                {RGBA8, .w=1.0,},
                {RGBA9, .w=1.0,},
            },
            .wpos3 = {
                {.x=-5, .y=0, .z=-15, .w=0,},
                {.x=-3, .y=0, .z=-15, .w=0,},
                {.x=-1, .y=0, .z=-15, .w=0,},
                {.x= 1, .y=0, .z=-15, .w=0,},
                {.x= 3, .y=0, .z=-15, .w=0,},
                {.x= 5, .y=0, .z=-15, .w=0,},
            },
        };

        // 2. Generate, bind, and upload data to the GPU
        GLuint ubo;
        gl_gen_buffers(1, &ubo);
        gl_bind_buffer(GL_SHADER_STORAGE_BUFFER, ubo);
        // gl_buffer_data(GL_SHADER_STORAGE_BUFFER, sizeof(struct my_data), data, GL_STATIC_DRAW);
        // gl_buffer_data(GL_SHADER_STORAGE_BUFFER, sizeof(data), &data, GL_STATIC_DRAW);
        gl_buffer_data(GL_SHADER_STORAGE_BUFFER, 2*tri_count*4*SIZE_F32, &data, GL_STATIC_DRAW);

        // 3. Bind the SSBO to binding point 0
        gl_bind_buffer_base(GL_SHADER_STORAGE_BUFFER, 0, ubo);
        // Optional: unbind to avoid accidental changes
        gl_bind_buffer(GL_SHADER_STORAGE_BUFFER, 0);
    }


    gl_use_program(program);

    struct camera camera;
    {
        /// create camera
        float aspect = (float)SCREEN_RES_X / (float)SCREEN_RES_Y;
        float near   = 0.01f;
        float far    = 100.0f;
        float fov    = 45.0f;
        struct camera_projection_matrix projection_matrix =
            scene_create_perspective_projection_matrix(fov, near, far, aspect);

        camera = camera_create(projection_matrix, GL_FALSE);
        GLint uinif_loc = glGetUniformLocation(program, "projMat");
        if(uinif_loc != -1) {}
            glUniformMatrix4fv(uinif_loc, 1, camera.is_column_major, &camera.projection_matrix.projection_matrix[0]);
    }


    run(program, vao, vertices);

#   undef tri_count
}
