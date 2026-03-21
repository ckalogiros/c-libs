/*
 * Created by ckalo on 2/4/26.
 */

// #include <string.h>

#include <stdio.h>

#include "glad.h"
#include "GL/glx.h"
#include "renderer_internal.h"
#include "logger.h"
#include "renderer_structs.h"
#include "sys_utils.h"


void opengl_load_proc()
{
    if(!gladLoadGLLoader((GLADloadproc) glXGetProcAddress)) {
        my_exit("Failed to initialize GLAD!\n", __FILE__, __LINE__);
    }
}

void opengl_create_context(uint64 window, void* display, void* visual)
{
    /**********************************************************************************************
     * create opengl context
     */
    GLXContext context = glXCreateContext(display, visual, NULL, GL_TRUE);
    glXMakeCurrent(display, window, context);
    void* proc = glXGetProcAddress((const GLubyte*)"glGetString");


    /**********************************************************************************************
     * load opengl functions
     */
    opengl_load_proc();

    // print some info
    FILE* file;
    dprint(file, 2, "\n*** opengl.c ***\n");

    dprint(file, 2, "glXGetProcAddress test: %p\n", proc);  // Should NOT be NULL

    dprint(file, 2, "GL Vendor: %s\n", glGetString(GL_VENDOR));
    dprint(file, 2, "GL Renderer: %s\n",glGetString(GL_RENDERER));
    dprint(file, 2, "GL Version: %s\n", glGetString(GL_VERSION));
    dprint(file, 2, "GL Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    dprint(file, 2, "OpenGL version %s\n", glGetString(GL_VERSION));
    // dprint(file, 2, "%sOpenGL version %s%s\n", YELLOW, glGetString(GL_VERSION), CLEAR);

}

XVisualInfo* init_glx(struct os_window* win)
{
    GLint glxAttribs[] = {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        GLX_DEPTH_SIZE,     24,
        GLX_STENCIL_SIZE,   8,
        GLX_RED_SIZE,       8,
        GLX_GREEN_SIZE,     8,
        GLX_BLUE_SIZE,      8,
        GLX_SAMPLE_BUFFERS, 0,
        GLX_SAMPLES,        0,
        None
    };

    XVisualInfo* visual = glXChooseVisual(win->display, win->screen_id, glxAttribs);

    if (visual == 0) {
        XCloseDisplay(win->display);
        my_exit("glXChooseVisual FAILED to initialize", __FILE__, __LINE__);
    }

    return visual;
}

uint32
generate_temp_gfx()
{
    // generate a list of 100 quad locations/translation-vectors
    // ---------------------------------------------------------
    struct pos { float x, y; };
    struct pos translations[100];
    int index = 0;
    float offsetx = -1.0f;
    float offsety = 1.0f;
    float move = 0.2f;
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            translations[index].x = offsetx;
            translations[index++].y = offsety;
            offsetx += move;
        }
        offsety -= move;
        offsetx = -1.0f;
    }


    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct pos) * 100, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float quadVertices[] = {
        // positions     // colors
        -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
         0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

        -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
         0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
         0.05f, 0.05f, 0.0f, 1.0f, 1.0f
    };
    unsigned int VAO, quadVBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));

    // also set instance data
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // tell OpenGL this is an instanced vertex attribute.

    return VAO;
}