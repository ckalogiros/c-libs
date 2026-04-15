/*
 * Created by ckalo on 3/27/26.
 *
 * Initialize glad and glfw once
 */
#ifndef GLFW_INIT_GLAD_INTERNAL_H
#define GLFW_INIT_GLAD_INTERNAL_H

#if defined(_MSC_VER)
 // Make MS math.h define M_PI
 #define _USE_MATH_DEFINES
#endif

#define GLAD_GL_IMPLEMENTATION
// #include <glad/gl.h>
#define GLFW_INCLUDE_NONE
// #include <glfw3.h>

#endif //GLFW_INIT_GLAD_INTERNAL_H