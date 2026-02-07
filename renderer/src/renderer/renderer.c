/*
 * Created by ckalo on 2/4/26.
 */

#include "renderer.h"

#include "glad.h"
#include "renderer_internal.h"
#include "win_x11.h"
#include "GL/glx.h"
#include "opengl.h"
#include "logger.h"
#include "shader_build.h"


void init_x11_events()
{

}


struct os_window* renderer_create_x11_window(const uint32 xpos, const uint32 ypos, const uint32 w, const uint32 h)
{
    FILE* file;
    dprint(file, 1, "\n*** renderer.c\n");  // Should NOT be NULL

    struct os_window* os_win = NULL;

    os_win = win_x11_init_display(); // choose/init display

    os_win->visual = init_glx(os_win); // create visual: needs display

    os_win = win_x11_create(600, 100, 800, 600); // create window: needs display and visual



    opengl_create_context(os_win->window, os_win->display, os_win->visual);


    init_x11_events();

    XClearWindow(os_win->display, os_win->window);
    XMapRaised(os_win->display, os_win->window);


    glViewport(xpos, ypos, w, h);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);


    XSelectInput(os_win->display, os_win->window, ExposureMask | KeyPressMask);
    XMapWindow(os_win->display, os_win->window);

    return os_win;

}


void render(struct os_window* os_win)
{
    static const char* vs_src = "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "layout (location = 2) in vec2 aOffset;\n"
        "out vec3 fColor;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos + aOffset, 0.0, 1.0);\n"
        "   fColor = aColor;\n"
        "}\0";


    static const char* fs_src = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "in vec3 fColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(fColor, 1.0);\n"
            "}\n\0";

    const uint32 shader_program = shader_build(vs_src, fs_src);

    const uint32 VAO = generate_temp_gfx();

    // todo XEvent: implement in engine's event system
    XEvent e;
    const char* msg = "Hello there! This is a sample text";

    while (1) {

        XNextEvent(os_win->display, &e);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (e.type == Expose) {
            XWindowAttributes attribs;
            XGetWindowAttributes(os_win->display, os_win->window, &attribs);
            glXSwapBuffers(os_win->display, os_win->window);
        }
        if (e.type == KeyPress) {
            XWindowAttributes attribs;
            XGetWindowAttributes(os_win->display, os_win->window, &attribs);
        }

        glUseProgram(shader_program);
        glBindVertexArray(VAO);


        // draw 100 instanced quads
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
        glBindVertexArray(0);


        glXSwapBuffers(os_win->display, os_win->window);

    }

}