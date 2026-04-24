/*
 * Created by ckalo on 4/2/26.
 */

#include "window.h"
// #define GLFW_INCLUDE_NONE
// #include <glfw3.h>
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>

#include "layer.h"
#include "linmath.h"


/***************************************************************************************************
 * TEMPORARY OPENGL GLOBALS
 */
GLFWwindow* window = NULL;


GLFWwindow* window_get() { return window; }
uint32 window_get_width()
{
    uint32 width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return width;
}
uint32 window_get_height()
{
    uint32 width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return width;
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


void window_create()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    /// https://www.glfw.org/docs/latest/window_guide.html
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);

    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

    window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowPos(window, 1760, 500);

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return;
    }

    glfwSwapInterval(0);

    glEnable(GL_DEBUG_OUTPUT);

}

void window_terminate()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

// void window_update(struct layer* layer)
// {
//     vec4f color = {.r = .0, .g = .0, .b = .0, .a = .0};
//     while (!glfwWindowShouldClose(window))
//     {
//         glfwPollEvents();
//
//         int width = window_get_width();
//         int height = window_get_height();
//         gl_viewport(width, height);
//
//         gl_clear();
//         gl_clear_color(color);
//
//         layer_on_update(layer);
//         layer_on_render(layer);
//
//         glfwSwapBuffers(window);
//
//     }
//
//     glfwDestroyWindow(window);
//
//     glfwTerminate();
// }

bool window_should_close()
{
    return glfwWindowShouldClose(window);
}
void window_poll_events()
{
    glfwPollEvents();
}
bool window_swap_buffers()
{
    glfwSwapBuffers(window);
}







//
// void opengl_triangle(void)
// {
//     glfwSetErrorCallback(error_callback);
//
//     if (!glfwInit())
//         exit(EXIT_FAILURE);
//
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//     GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
//     if (!window)
//     {
//         glfwTerminate();
//         exit(EXIT_FAILURE);
//     }
//
//     glfwSetKeyCallback(window, key_callback);
//
//     glfwMakeContextCurrent(window);
//     gladLoadGL(glfwGetProcAddress);
//     glfwSwapInterval(0);
//
//     // NOTE: OpenGL error checks have been omitted for brevity
//
//     GLuint vbo;
//     glGenBuffers(1, &vbo);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//     const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//     glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
//     glCompileShader(vertex_shader);
//
//     const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
//     glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
//     glCompileShader(fragment_shader);
//
//     const GLuint program = glCreateProgram();
//     glAttachShader(program, vertex_shader);
//     glAttachShader(program, fragment_shader);
//     glLinkProgram(program);
//
//     const GLint mvp_location = glGetUniformLocation(program, "MVP");
//     const GLint vpos_location = glGetAttribLocation(program, "vPos");
//     const GLint vcol_location = glGetAttribLocation(program, "vCol");
//
//     GLuint vertex_array;
//     glGenVertexArrays(1, &vertex_array);
//     glBindVertexArray(vertex_array);
//     glEnableVertexAttribArray(vpos_location);
//     glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
//                           sizeof(Vertex), (void*) offsetof(Vertex, pos));
//     glEnableVertexAttribArray(vcol_location);
//     glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
//                           sizeof(Vertex), (void*) offsetof(Vertex, col));
//
//     struct timer* t = timer_create_perf_timer();
//
//
//     while (!glfwWindowShouldClose(window))
//     {
//         // perf_timer_start(t);
//         perf_timer_start_accum(t);
//
//         int width, height;
//         glfwGetFramebufferSize(window, &width, &height);
//         const float ratio = (float) width / (float) height;
//
//         glViewport(0, 0, width, height);
//         glClear(GL_COLOR_BUFFER_BIT);
//
//
//
//         mat4x4 m, p, mvp;
//         mat4x4_identity(m);
//         mat4x4_rotate_Z(m, m, (float) glfwGetTime());
//         mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//         mat4x4_mul(mvp, p, m);
//
//
//
//         glUseProgram(program);
//         glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp);
//         glBindVertexArray(vertex_array);
//         glDrawArrays(GL_TRIANGLES, 0, 3);
//
//
//
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//
//
//         // perf_timer_stop(t);
//         perf_timer_stop_accum(t);
//         // printf("fps:%u\n", perf_timer_get_fps(t));
//         print("fps:%u\n", perf_timer_calc_fps_from_nano(perf_timer_calc_avg(t)));
//         // printf("fps:%u\n", perf_timer_calc_avg(t));
//         // perf_timer_reset(t);
//
//
//     }
//
//     glfwDestroyWindow(window);
//
//     glfwTerminate();
//     // exit(EXIT_SUCCESS);
// }
//
