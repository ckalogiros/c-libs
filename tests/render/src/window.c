/*
 * Created by ckalo on 4/2/26.
 */

#include "window.h"
#include "opengl.h"
#include "GLOBALS.h"
#include <stdio.h>
#include <stdlib.h>
// #include "glad.h"
// #include "linmath.h"

vec2f64 mouse_pos_temp = {0,0};

/***************************************************************************************************
 * TEMPORARY OPENGL GLOBALS
 */
GLFWwindow* window = NULL;


GLFWwindow*
window_get() { return window; }

uint32
window_get_width()
{
    uint32 width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return width;
}

uint32
window_get_height()
{
    uint32 width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return width;
}

void
error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void io_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void io_key_set_callback(GLFWwindow* window, GLFWkeyfun cbfun){ glfwSetKeyCallback(window, cbfun); }

void io_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    vec2f64 pos = {0};
    glfwGetCursorPos(window, &pos.x, &pos.y);
}
void io_mouse_set_callback(GLFWwindow* window, GLFWcursorposfun cbfun){ glfwSetCursorPosCallback(window, cbfun); }

vec2f64 io_mouse_get_pos()
{
    glfwGetCursorPos(window, &mouse_pos_temp.x, &mouse_pos_temp.y);
    // print("x:%.0f\n", mouse_pos_temp.x);
    return mouse_pos_temp;
}
void window_size_callback(GLFWwindow* window, int width, int height)
{
    // glfwSetWindowSize(window, width, height);
    // gl_viewport(width, height);
    // print("w:%d h:%d\n", width, height);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    gl_viewport(width, height);
    float left   = 0;
    float right  =  width;
    float bottom =  height;
    float top    =  0;
    float near   =  0.1;
    float far    =  100;
}

void
window_create(const uint32 width, const uint32 height)
{
    glfwSetErrorCallback(error_callback);

    if(!glfwInit())
        exit(EXIT_FAILURE);

    /// https://www.glfw.org/docs/latest/window_guide.html
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(width, height, "OpenGL Triangle", NULL, NULL);
    if(!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    /* for using GLFW's own opengl functions via gl.h */
    // if(!gladLoadGL(glfwGetProcAddress)) {
    /* for using our own opengl functions via glad.h.c */
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return;
    }

    glfwSwapInterval(0);

    glfwSetWindowPos(window, 2000, 400);

    io_key_set_callback(window, io_key_callback);

    glfwSetWindowSizeCallback(window, window_size_callback);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

}

void
window_terminate()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool
window_should_close()
{
    return glfwWindowShouldClose(window);
}

void
window_poll_events()
{
    glfwPollEvents();
}

bool
window_swap_buffers()
{
    glfwSwapBuffers(window);
}
